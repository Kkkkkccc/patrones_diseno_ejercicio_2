#include "expenses.h"
#include <ctime>

std::string CsvExporter::export_expenses(const ExpenseList& expenses) const {
    std::ostringstream oss;
    oss << "date,category,detail,amount\n";
    for (const auto& e : expenses) {
        oss << e.date << ","
            << e.category << ","
            << e.detail << ","
            << std::fixed << std::setprecision(2) << e.amount << "\n";
    }
    return oss.str();
}

std::string JsonExporter::export_expenses(const ExpenseList& expenses) const {
    std::ostringstream oss;
    oss << "[\n";
    for (size_t i = 0; i < expenses.size(); ++i) {
        const auto& e = expenses[i];
        oss << "  {\n";
        oss << "    \"date\": \"" << e.date << "\",\n";
        oss << "    \"category\": \"" << e.category << "\",\n";
        oss << "    \"detail\": \"" << e.detail << "\",\n";
        oss << "    \"amount\": " << std::fixed << std::setprecision(2) << e.amount << "\n";
        oss << "  }";
        if (i < expenses.size() - 1) oss << ",";
        oss << "\n";
    }
    oss << "]\n";
    return oss.str();
}

std::string TextExporter::export_expenses(const ExpenseList& expenses) const {
    std::ostringstream oss;
    oss << "EXPENSES REPORT\n";
    oss << "================\n\n";
    for (const auto& e : expenses) {
        oss << "Date: " << e.date << "\n";
        oss << "Category: " << e.category << "\n";
        oss << "Detail: " << e.detail << "\n";
        oss << "Amount: $" << std::fixed << std::setprecision(2) << e.amount << "\n";
        oss << "----------------\n";
    }
    return oss.str();
}

template<ExpenseExporter Inner>
std::string AuditedExporter<Inner>::export_expenses(const ExpenseList& expenses) const {
    std::ostringstream oss;
    oss << "AUDIT REPORT\n";
    oss << "============\n";
    oss << "Generated: " << std::time(nullptr) << "\n";
    oss << "Items: " << expenses.size() << "\n\n";
    oss << inner.export_expenses(expenses);
    return oss.str();
}

template<ExpenseExporter Inner>
std::string SummaryExporter<Inner>::export_expenses(const ExpenseList& expenses) const {
    std::ostringstream oss;
    oss << inner.export_expenses(expenses);
    double total = std::accumulate(expenses.begin(), expenses.end(), 0.0,
        [](double sum, const Expense& e) { return sum + e.amount; });
    oss << "\nTOTAL: $" << std::fixed << std::setprecision(2) << total << "\n";
    return oss.str();
}

template ExportFunction make_exporter<CsvExporter>();
template ExportFunction make_exporter<JsonExporter>();
template ExportFunction make_exporter<TextExporter>();

template struct AuditedExporter<CsvExporter>;
template struct AuditedExporter<JsonExporter>;
template struct AuditedExporter<TextExporter>;

template struct SummaryExporter<CsvExporter>;
template struct SummaryExporter<JsonExporter>;
template struct SummaryExporter<TextExporter>;

template struct AuditedExporter<SummaryExporter<CsvExporter>>;
template struct AuditedExporter<SummaryExporter<JsonExporter>>;
template struct AuditedExporter<SummaryExporter<TextExporter>>;

template struct SummaryExporter<AuditedExporter<CsvExporter>>;
template struct SummaryExporter<AuditedExporter<JsonExporter>>;
template struct SummaryExporter<AuditedExporter<TextExporter>>;

template struct AuditedExporter<AuditedExporter<CsvExporter>>;
template struct SummaryExporter<SummaryExporter<CsvExporter>>;