#pragma once

#include <string>
#include <vector>
#include <functional>
#include <algorithm>
#include <ranges>
#include <numeric>
#include <sstream>
#include <iomanip>

struct Expense {
    std::string date;
    std::string category;
    std::string detail;
    double amount{};
};

using ExpenseList = std::vector<Expense>;

template<class T>
concept ExpenseExporter = requires(T exporter, const ExpenseList& expenses) {
    { exporter.export_expenses(expenses) } -> std::same_as<std::string>;
};

using ExportFunction = std::function<std::string(const ExpenseList&)>;

template<class R, class Cmp>
concept SortStrategy = std::ranges::random_access_range<R> &&
    std::sortable<std::ranges::iterator_t<R>, Cmp>;

template<class R, class Cmp>
void sort_with(R& values, Cmp cmp) requires SortStrategy<R, Cmp> {
    std::ranges::sort(values, cmp);
}

template<ExpenseExporter E, class... Args>
ExportFunction make_exporter(Args&&... args) {
    return [exporter = E(std::forward<Args>(args)...)](const ExpenseList& expenses) {
        return exporter.export_expenses(expenses);
    };
}

struct CsvExporter {
    std::string export_expenses(const ExpenseList& expenses) const;
};

struct JsonExporter {
    std::string export_expenses(const ExpenseList& expenses) const;
};

struct TextExporter {
    std::string export_expenses(const ExpenseList& expenses) const;
};

template<ExpenseExporter Inner>
struct AuditedExporter {
    Inner inner;
    std::string export_expenses(const ExpenseList& expenses) const;
};

template<ExpenseExporter Inner>
struct SummaryExporter {
    Inner inner;
    std::string export_expenses(const ExpenseList& expenses) const;
};