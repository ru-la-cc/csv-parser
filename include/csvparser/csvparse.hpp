#ifndef __MASSNS_CSVPARSE_HPP__
#define __MASSNS_CSVPARSE_HPP__

#include <string>
#include <string_view>
#include <vector>
#include <concepts>
#include <type_traits>
#include <algorithm>

namespace massns {

template <typename T>
concept CharType = std::same_as<T, char> || 
                    std::same_as<T, char16_t> || 
                    std::same_as<T, char32_t> || 
                    std::same_as<T, wchar_t>;

template <typename T, typename Char>
concept StringType = std::same_as<T, std::basic_string<Char>>;

template <typename T, typename Char>
concept StringViewType = std::same_as<T, std::basic_string_view<Char>>;

template <typename TChar, typename TString, typename TView>
requires CharType<TChar> && 
        StringType<TString, TChar> && 
        StringViewType<TView, TChar>
class csvparse final {
private:
    size_t m_max_split;
    TString m_separator;
    TChar m_quoat;

    TString m_line;
    std::vector<TString> m_extracted_columns;
    std::vector<bool> m_quoated;
    
    bool m_is_complete;
    
    TString m_rebuilt_line;
    TView m_rebuilt_line_view;
public:
    csvparse(size_t max_split = 0, 
            const TString& separator = TString(1, ','), 
            TChar quoat = '"')
        : m_max_split(max_split)
        , m_separator(separator)
        , m_quoat(quoat)
        , m_is_complete(true) {
    }

    csvparse(const csvparse& other) = default;
    csvparse(csvparse&& other) noexcept = default;
    csvparse& operator=(const csvparse& other) = default;
    csvparse& operator=(csvparse&& other) noexcept = default;
    ~csvparse() = default;

    void set_separator(TString separator) {
        m_separator = std::move(separator);
    }

    TString get_separator() const {
        return m_separator;
    }

    void set_quoat(TChar quoat) {
        m_quoat = quoat;
    }

    TChar get_quoat() const {
        return m_quoat;
    }

    void set_string(TString line, bool is_trim_crlf = true) {
        m_quoated.clear();
        m_extracted_columns.clear();
        m_line.clear();
        
        if (is_trim_crlf) {
            trim_crlf(line);
        }

        m_line = std::move(line);
        parse_line();
    }

    void add_string(TString line, bool is_trim_crlf = true) {
        if (is_trim_crlf) {
            trim_crlf(line);
        }

        m_line.append(line);
        m_quoated.clear();
        m_extracted_columns.clear();
        parse_line();
    }

    bool is_complete() const {
        return m_is_complete;
    }

    TView operator[](size_t idx) {
        if (idx >= m_extracted_columns.size()) {
            static TString empty_string;
            return TView(empty_string);
        }
        return TView(m_extracted_columns[idx]);
    }

    const TView operator[](size_t idx) const {
        if (idx >= m_extracted_columns.size()) {
            static const TString empty_string;
            return TView(empty_string);
        }
        return TView(m_extracted_columns[idx]);
    }

    bool is_quoat(size_t idx) const {
        if (idx >= m_quoated.size()) {
            return false;
        }
        return m_quoated[idx];
    }

    const TView& line_build() {
        m_rebuilt_line.clear();
        
        for (size_t i = 0; i < m_extracted_columns.size(); ++i) {
            if (i > 0) {
                m_rebuilt_line.append(m_separator);
            }
            
            const TString& column = m_extracted_columns[i];
            bool needs_quoat = m_quoated[i] || 
                            column.find(m_separator) != TString::npos || 
                            column.find(m_quoat) != TString::npos || 
                            column.find('\n') != TString::npos || 
                            column.find('\r') != TString::npos;
            
            if (needs_quoat) {
                m_rebuilt_line.push_back(m_quoat);
                
                size_t pos = 0;
                TString temp_column(column);
                while ((pos = temp_column.find(m_quoat, pos)) != TString::npos) {
                    m_rebuilt_line.append(temp_column.substr(0, pos));
                    m_rebuilt_line.append(TString(2, m_quoat));
                    temp_column = temp_column.substr(pos + 1);
                    pos = 0;
                }
                
                m_rebuilt_line.append(temp_column);
                m_rebuilt_line.push_back(m_quoat);
            } else {
                m_rebuilt_line.append(column);
            }
        }

        m_rebuilt_line_view = TView(m_rebuilt_line);
        return m_rebuilt_line_view;
    }

    const TView& get_line() {
        if (m_rebuilt_line.empty()) {
            line_build();
        }
        return m_rebuilt_line_view;
    }

    size_t size() const {
        return m_extracted_columns.size();
    }

private:
    void trim_crlf(TString& line) {
        while (!line.empty() && (line.back() == '\r' || line.back() == '\n')) {
            line.pop_back();
        }
    }

    void parse_line() {
        m_quoated.clear();
        m_extracted_columns.clear();

        size_t pos = 0;
        bool in_quotes = false;
        size_t current_column_start = 0;
        m_is_complete = true;

        size_t remaining_splits = (m_max_split > 0) ? m_max_split - 1 : std::numeric_limits<size_t>::max();

        while (pos < m_line.size()) {
            if (in_quotes) {
                if (m_line[pos] == m_quoat) {
                    if (pos + 1 < m_line.size() && m_line[pos + 1] == m_quoat) {
                        pos += 2;
                    } else {
                        in_quotes = false;
                        ++pos;
                    }
                } else {
                    ++pos;
                }
            }
            else {
                if (m_line.compare(pos, m_separator.size(), m_separator) == 0) {
                    if (m_extracted_columns.size() < m_max_split - 1 || m_max_split == 0) {
                        TString column = extract_column(current_column_start, pos);
                        m_extracted_columns.push_back(column);
                        
                        pos += m_separator.size();
                        current_column_start = pos;
                    } else {
                        ++pos;
                    }
                }
                else if (m_line[pos] == m_quoat) {
                    in_quotes = true;
                    ++pos;
                }
                else {
                    ++pos;
                }
            }
        }

        if (current_column_start <= m_line.size()) {
            if (m_max_split > 0 && m_extracted_columns.size() >= m_max_split) {
                return;
            }

            TString column = extract_column(current_column_start, m_line.size());
            m_extracted_columns.push_back(column);
        }

        if (in_quotes) {
            m_is_complete = false;
        }
    }

    TString extract_column(size_t start, size_t end) {
        TString column = m_line.substr(start, end - start);
        bool is_quoted = false;

        if (!column.empty() && column.front() == m_quoat && column.back() == m_quoat) {
            is_quoted = true;
            column = column.substr(1, column.size() - 2);

            size_t pos = 0;
            while ((pos = column.find(TString(2, m_quoat), pos)) != TString::npos) {
                column.erase(pos, 1);
                pos += 1;
            }
        }

        m_quoated.push_back(is_quoted);
        return column;
    }
};

}

#endif // __MASSNS_CSVPARSE_HPP__
