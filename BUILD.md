# ビルド手順

このドキュメントでは、csvparserライブラリのビルド方法について説明します。

## 必要条件

- C++20対応コンパイラ (GCC 10+, Clang 10+, MSVC 2019+)
- CMake 3.14以上
- Git (オプション、GoogleTestのダウンロードに使用)

## ビルド手順

### 1. リポジトリのクローン

```bash
git clone https://github.com/yourusername/csvparser.git
cd csvparser
```

### 2. ビルドディレクトリの作成

```bash
mkdir -p build
cd build
```

### 3. CMakeの実行

```bash
cmake ..
```

オプションの指定:

- テストのビルドを無効にする場合: `-DBUILD_TESTS=OFF`
- サンプルプログラムのビルドを無効にする場合: `-DBUILD_SAMPLE=OFF`

例:

```bash
cmake .. -DBUILD_TESTS=OFF -DBUILD_SAMPLE=ON
```

### 4. ビルドの実行

```bash
cmake --build .
```

または:

```bash
make
```

### 5. テストの実行 (テストを有効にした場合)

```bash
ctest
```

または:

```bash
./tests/csvparser_test
```

### 6. サンプルプログラムの実行 (サンプルを有効にした場合)

```bash
./csvparser_sample
```

## インストール

ライブラリをシステムにインストールするには:

```bash
cmake --build . --target install
```

または:

```bash
sudo make install
```

デフォルトでは、ヘッダファイルは `/usr/local/include` にインストールされます。
インストール先を変更するには、CMakeの実行時に `-DCMAKE_INSTALL_PREFIX=/path/to/install` オプションを指定します。

## ヘッダーオンリーライブラリとしての使用

csvparserはヘッダーオンリーライブラリなので、単に `include/csvparser/csvparse.hpp` ファイルをプロジェクトにコピーして使用することもできます。 