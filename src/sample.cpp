#include <csvparser/csvparse.hpp>
#include <iostream>
#include <string>
#include <iomanip>

int main() {
    std::cout << "csvparse さんぷる" << std::endl;
    std::cout << "===============================" << std::endl << std::endl;

    // 基本的なCSV文字列のパース
    {
        std::cout << "1. 基本的なCSVのパース:" << std::endl;
        massns::csvparse<char, std::string, std::string_view> parser;
        
        std::string csv = "aaa,bbb,ccc";
        std::cout << "入力: " << csv << std::endl;
        
        parser.set_string(csv);
        
        std::cout << "カラム数: " << parser.size() << std::endl;
        for (size_t i = 0; i < parser.size(); ++i) {
            std::cout << "カラム " << i << ": " << std::quoted(parser[i]) << std::endl;
        }
        std::cout << std::endl;
    }
    
    // 引用符付きCSVのパース
    {
        std::cout << "2. 引用符付きCSVのパース:" << std::endl;
        massns::csvparse<char, std::string, std::string_view> parser;
        
        std::string csv = "\"aaa\",\"b,bb\",\"ccc\"";
        std::cout << "入力: " << csv << std::endl;
        
        parser.set_string(csv);
        
        std::cout << "カラム数: " << parser.size() << std::endl;
        for (size_t i = 0; i < parser.size(); ++i) {
            std::cout << "カラム " << i << ": " << std::quoted(parser[i]) 
                    << (parser.is_quoat(i) ? " (引用符付き)" : "") << std::endl;
        }
        std::cout << std::endl;
    }
    
    // エスケープされた引用符を含むCSVのパース
    {
        std::cout << "3. エスケープされた引用符を含むCSVのパース:" << std::endl;
        massns::csvparse<char, std::string, std::string_view> parser;
        
        std::string csv = "\"a\"\"aa\"\"\",bbb,\"c\"\"cc\"";
        std::cout << "入力: " << csv << std::endl;
        
        parser.set_string(csv);
        
        std::cout << "カラム数: " << parser.size() << std::endl;
        for (size_t i = 0; i < parser.size(); ++i) {
            std::cout << "カラム " << i << ": " << std::quoted(parser[i]) 
                    << (parser.is_quoat(i) ? " (引用符付き)" : "") << std::endl;
        }
        std::cout << std::endl;
    }
    
    // 不完全なCSVと追加
    {
        std::cout << "4. 不完全なCSVと追加:" << std::endl;
        massns::csvparse<char, std::string, std::string_view> parser;
        
        std::string csv1 = "\"aaa";
        std::cout << "入力 (前半): " << csv1 << std::endl;
        
        parser.set_string(csv1);
        std::cout << "完全か: " << (parser.is_complete() ? "はい" : "いいえ") << std::endl;
        
        std::string csv2 = "\",bbb,ccc";
        std::cout << "入力 (後半): " << csv2 << std::endl;
        
        parser.add_string(csv2);
        std::cout << "完全か: " << (parser.is_complete() ? "はい" : "いいえ") << std::endl;
        
        std::cout << "カラム数: " << parser.size() << std::endl;
        for (size_t i = 0; i < parser.size(); ++i) {
            std::cout << "カラム " << i << ": " << std::quoted(parser[i]) 
                    << (parser.is_quoat(i) ? " (引用符付き)" : "") << std::endl;
        }
        std::cout << std::endl;
    }
    
    // カスタム区切り文字とクォーテーション
    {
        std::cout << "5. 指定した区切り文字とクォーテーション:" << std::endl;
        massns::csvparse<char, std::string, std::string_view> parser;
        
        parser.set_separator("|");
        parser.set_quoat('\'');
        
        std::string csv = "aaa|'b|bb'|ccc";
        std::cout << "入力: " << csv << std::endl;
        std::cout << "区切り文字: '" << parser.get_separator() << "', 引用符: '" << parser.get_quoat() << "'" << std::endl;
        
        parser.set_string(csv);
        
        std::cout << "カラム数: " << parser.size() << std::endl;
        for (size_t i = 0; i < parser.size(); ++i) {
            std::cout << "カラム " << i << ": " << std::quoted(parser[i]) 
                    << (parser.is_quoat(i) ? " (引用符付き)" : "") << std::endl;
        }
        std::cout << std::endl;
    }
    
    // 最大分割数
    {
        std::cout << "6. 最大分割数:" << std::endl;
        massns::csvparse<char, std::string, std::string_view> parser(2);
        
        std::string csv = "aaa,bbb,ccc,ddd";
        std::cout << "入力: " << csv << " (最大分割数: 2)" << std::endl;
        
        parser.set_string(csv);
        
        std::cout << "カラム数: " << parser.size() << std::endl;
        for (size_t i = 0; i < parser.size(); ++i) {
            std::cout << "カラム " << i << ": " << std::quoted(parser[i]) << std::endl;
        }
        std::cout << std::endl;
    }
    
    // 改行を含むCSV
    {
        std::cout << "7. 改行を含むCSV:" << std::endl;
        massns::csvparse<char, std::string, std::string_view> parser;
        
        std::string csv = "\"aaa\nbbb\",ccc,\"ddd\neee\"";
        std::cout << "入力: 改行を含むCSV文字列" << std::endl;
        
        parser.set_string(csv);
        
        std::cout << "カラム数: " << parser.size() << std::endl;
        for (size_t i = 0; i < parser.size(); ++i) {
            std::cout << "カラム " << i << ": ";
            
            // 改行を表示用に置換
            std::string value(parser[i]);
            size_t pos = 0;
            while ((pos = value.find('\n', pos)) != std::string::npos) {
                value.replace(pos, 1, "\\n");
                pos += 2;
            }
            
            std::cout << std::quoted(value) 
                    << (parser.is_quoat(i) ? " (引用符付き)" : "") << std::endl;
        }
        std::cout << std::endl;
    }
    
    // 文字列の再構築
    {
        std::cout << "8. 文字列の再構築:" << std::endl;
        massns::csvparse<char, std::string, std::string_view> parser;
        
        std::string csv = "aaa,\"b,bb\",ccc";
        std::cout << "入力: " << csv << std::endl;
        
        parser.set_string(csv);
        
        std::string_view rebuilt = parser.line_build();
        std::cout << "再構築: " << rebuilt << std::endl;
        std::cout << std::endl;
    }
    
    // ワイド文字
    {
        std::cout << "9. ワイド文字:" << std::endl;
        massns::csvparse<wchar_t, std::wstring, std::wstring_view> parser;
        
        std::wstring csv = L"aaa,\"b,bb\",ccc";
        std::wcout << L"入力: " << csv << std::endl;
        
        parser.set_string(csv);
        
        std::wcout << L"カラム数: " << parser.size() << std::endl;
        for (size_t i = 0; i < parser.size(); ++i) {
            std::wcout << L"カラム " << i << L": " << parser[i] << std::endl;
        }
        std::cout << std::endl;
    }
    
    return 0;
} 