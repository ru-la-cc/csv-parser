# csvparse

メモリ上の CSV 文字列をパースするだけ。  
大昔に書いたコードをchatgptにリファクタリングさせただけのものです。  
ビルド環境構築は全部cursorのagentに丸投げしました。

## 結局、本体はこれだけ
`include/csvparser/csvparse.hpp`

## ビルド方法

### macOS/Linux
```bash
mkdir build && cd build
cmake ..
make
```

### Windows

#### MinGW (推奨)
```cmd
build.bat
```

#### MSVC
```cmd
build-msvc.bat
```

#### 手動ビルド
```cmd
mkdir build
cd build
cmake .. -G "MinGW Makefiles"
cmake --build .
```

## テスト実行
```bash
# macOS/Linux
cd build && ctest

# Windows
cd build && ctest --output-on-failure
```

## サンプル実行
```bash
# macOS/Linux
./build/csvparser_sample

# Windows
.\build\csvparser_sample.exe
```

## その他
 ~~悪いなのび太、Macでしかテストしてないんだ~~  
Windowsでもテストした