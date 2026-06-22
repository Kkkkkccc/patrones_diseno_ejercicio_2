#include "expenses.h"
#include <iostream>

int main() {
    ExpenseList expenses = {
        {"2026-06-01", "food", "lunch", 18.50},
        {"2026-06-02", "transport", "bus", 3.20},
        {"2026-06-03", "books", "cpp", 45.00},
        {"2026-06-04", "food", "dinner", 25.30},
        {"2026-06-05", "transport", "taxi", 15.00}
    };

    std::cout << "=== Personal Expenses Tool ===\n\n";

    std::cout << "Original expenses:\n";
    for (const auto& e : expenses) {
        std::cout << e.date << " | " << e.category << " | " << e.detail << " | $" << e.amount << "\n";
    }
    std::cout << "\n";

    auto by_category = [](const Expense& a, const Expense& b) {
        return a.category < b.category;
    };
    sort_with(expenses, by_category);

    std::cout << "Sorted by category:\n";
    for (const auto& e : expenses) {
        std::cout << e.date << " | " << e.category << " | " << e.detail << " | $" << e.amount << "\n";
    }
    std::cout << "\n";

    auto csv_exporter = make_exporter<CsvExporter>();
    std::cout << "CSV Export:\n" << csv_exporter(expenses) << "\n";

    auto json_exporter = make_exporter<JsonExporter>();
    std::cout << "JSON Export:\n" << json_exporter(expenses) << "\n";

    auto decorated_exporter = AuditedExporter{ SummaryExporter{ CsvExporter{} } };
    std::cout << "Decorated Export (Audited + Summary):\n"
              << decorated_exporter.export_expenses(expenses) << "\n";

    return 0;
}