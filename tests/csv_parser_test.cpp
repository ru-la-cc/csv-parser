#include <gtest/gtest.h>
#include <csvparser/csvparse.hpp>
#include <string>
#include <vector>

// 基本的なCSVパース機能のテスト
TEST(CsvParseTest, BasicParsing) {
    massns::csvparse<char, std::string, std::string_view> parser;
    
    parser.set_string("aaa,bbb,ccc");
    EXPECT_TRUE(parser.is_complete());
    EXPECT_EQ(parser.size(), 3);
    EXPECT_EQ(parser[0], "aaa");
    EXPECT_EQ(parser[1], "bbb");
    EXPECT_EQ(parser[2], "ccc");
    
    // 引用符確認
    EXPECT_FALSE(parser.is_quoat(0));
    EXPECT_FALSE(parser.is_quoat(1));
    EXPECT_FALSE(parser.is_quoat(2));
}

// 引用符付きCSVのテスト
TEST(CsvParseTest, QuotedParsing) {
    massns::csvparse<char, std::string, std::string_view> parser;
    
    parser.set_string("\"aaa\",\"bbb\",\"ccc\"");
    EXPECT_TRUE(parser.is_complete());
    EXPECT_EQ(parser.size(), 3);
    EXPECT_EQ(parser[0], "aaa");
    EXPECT_EQ(parser[1], "bbb");
    EXPECT_EQ(parser[2], "ccc");
    
    // 引用符確認
    EXPECT_TRUE(parser.is_quoat(0));
    EXPECT_TRUE(parser.is_quoat(1));
    EXPECT_TRUE(parser.is_quoat(2));
}

// 引用符内にカンマを含むCSVのテスト
TEST(CsvParseTest, QuotedWithComma) {
    massns::csvparse<char, std::string, std::string_view> parser;
    
    parser.set_string("\"a,aa\",bbb,\"c,cc\"");
    EXPECT_TRUE(parser.is_complete());
    EXPECT_EQ(parser.size(), 3);
    EXPECT_EQ(parser[0], "a,aa");
    EXPECT_EQ(parser[1], "bbb");
    EXPECT_EQ(parser[2], "c,cc");
    
    // 引用符確認
    EXPECT_TRUE(parser.is_quoat(0));
    EXPECT_FALSE(parser.is_quoat(1));
    EXPECT_TRUE(parser.is_quoat(2));
}

// 引用符内に引用符を含むCSVのテスト
TEST(CsvParseTest, EscapedQuotes) {
    massns::csvparse<char, std::string, std::string_view> parser;
    
    parser.set_string("\"a\"\"aa\"\"\",bbb,\"c\"\"cc\"");
    EXPECT_TRUE(parser.is_complete());
    EXPECT_EQ(parser.size(), 3);
    EXPECT_EQ(parser[0], "a\"aa\"");
    EXPECT_EQ(parser[1], "bbb");
    EXPECT_EQ(parser[2], "c\"cc");
    
    // 引用符確認
    EXPECT_TRUE(parser.is_quoat(0));
    EXPECT_FALSE(parser.is_quoat(1));
    EXPECT_TRUE(parser.is_quoat(2));
}

// 空の項目を含むCSVのテスト
TEST(CsvParseTest, EmptyFields) {
    massns::csvparse<char, std::string, std::string_view> parser;
    
    parser.set_string("aaa,,ccc");
    EXPECT_TRUE(parser.is_complete());
    EXPECT_EQ(parser.size(), 3);
    EXPECT_EQ(parser[0], "aaa");
    EXPECT_EQ(parser[1], "");
    EXPECT_EQ(parser[2], "ccc");
    
    // 引用符確認
    EXPECT_FALSE(parser.is_quoat(0));
    EXPECT_FALSE(parser.is_quoat(1));
    EXPECT_FALSE(parser.is_quoat(2));
}

// 不完全なCSV（閉じられていない引用符）のテスト
TEST(CsvParseTest, IncompleteQuote) {
    massns::csvparse<char, std::string, std::string_view> parser;
    
    parser.set_string("\"aaa,bbb,ccc");
    EXPECT_FALSE(parser.is_complete());
}

// add_string機能のテスト
TEST(CsvParseTest, AddString) {
    massns::csvparse<char, std::string, std::string_view> parser;
    
    parser.set_string("\"aaa");
    EXPECT_FALSE(parser.is_complete());
    
    parser.add_string("\",bbb,ccc");
    EXPECT_TRUE(parser.is_complete());
    EXPECT_EQ(parser.size(), 3);
    EXPECT_EQ(parser[0], "aaa");
    EXPECT_EQ(parser[1], "bbb");
    EXPECT_EQ(parser[2], "ccc");
    
    // 引用符確認
    EXPECT_TRUE(parser.is_quoat(0));
    EXPECT_FALSE(parser.is_quoat(1));
    EXPECT_FALSE(parser.is_quoat(2));
}

// 最大分割数のテスト
TEST(CsvParseTest, MaxSplit) {
    massns::csvparse<char, std::string, std::string_view> parser(2);
    
    parser.set_string("aaa,bbb,ccc,ddd");
    EXPECT_TRUE(parser.is_complete());
    EXPECT_EQ(parser.size(), 2);
    EXPECT_EQ(parser[0], "aaa");
    EXPECT_EQ(parser[1], "bbb,ccc,ddd");
}

// カスタム区切り文字のテスト
TEST(CsvParseTest, CustomSeparator) {
    massns::csvparse<char, std::string, std::string_view> parser;
    parser.set_separator("|");
    
    parser.set_string("aaa|bbb|ccc");
    EXPECT_TRUE(parser.is_complete());
    EXPECT_EQ(parser.size(), 3);
    EXPECT_EQ(parser[0], "aaa");
    EXPECT_EQ(parser[1], "bbb");
    EXPECT_EQ(parser[2], "ccc");
}

// カスタム引用符のテスト
TEST(CsvParseTest, CustomQuote) {
    massns::csvparse<char, std::string, std::string_view> parser;
    parser.set_quoat('\'');
    
    parser.set_string("'aaa','bbb','ccc'");
    EXPECT_TRUE(parser.is_complete());
    EXPECT_EQ(parser.size(), 3);
    EXPECT_EQ(parser[0], "aaa");
    EXPECT_EQ(parser[1], "bbb");
    EXPECT_EQ(parser[2], "ccc");
    
    // 引用符確認
    EXPECT_TRUE(parser.is_quoat(0));
    EXPECT_TRUE(parser.is_quoat(1));
    EXPECT_TRUE(parser.is_quoat(2));
}

// 改行を含むCSVのテスト
TEST(CsvParseTest, NewlinesInQuotes) {
    massns::csvparse<char, std::string, std::string_view> parser;
    
    parser.set_string("\"aaa\nbbb\",ccc,\"ddd\neee\"");
    EXPECT_TRUE(parser.is_complete());
    EXPECT_EQ(parser.size(), 3);
    EXPECT_EQ(parser[0], "aaa\nbbb");
    EXPECT_EQ(parser[1], "ccc");
    EXPECT_EQ(parser[2], "ddd\neee");
}

// line_build関数のテスト
TEST(CsvParseTest, LineBuild) {
    massns::csvparse<char, std::string, std::string_view> parser;
    
    parser.set_string("aaa,\"b,bb\",ccc");
    EXPECT_TRUE(parser.is_complete());
    
    // 再構築された文字列を取得
    std::string_view rebuilt = parser.line_build();
    EXPECT_EQ(rebuilt, "aaa,\"b,bb\",ccc");
}

// ワイド文字でのテスト
TEST(CsvParseTest, WideCharTest) {
    massns::csvparse<wchar_t, std::wstring, std::wstring_view> parser;
    
    parser.set_string(L"aaa,bbb,ccc");
    EXPECT_TRUE(parser.is_complete());
    EXPECT_EQ(parser.size(), 3);
    EXPECT_EQ(parser[0], L"aaa");
    EXPECT_EQ(parser[1], L"bbb");
    EXPECT_EQ(parser[2], L"ccc");
}

// get_lineのテスト
TEST(CsvParseTest, GetLine) {
    massns::csvparse<char, std::string, std::string_view> parser;
    
    parser.set_string("aaa,\"b,bb\",ccc");
    EXPECT_TRUE(parser.is_complete());
    
    // 生成された文字列を取得
    std::string_view line = parser.get_line();
    EXPECT_EQ(line, "aaa,\"b,bb\",ccc");
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
} 