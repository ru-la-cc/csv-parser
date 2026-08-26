# UTF-8 BOMに変換するPowerShellスクリプト
Write-Host "Converting source files to UTF-8 BOM..." -ForegroundColor Green

# 変換対象のファイル
$files = @(
    "src/sample.cpp",
    "tests/csv_parser_test.cpp",
    "include/csvparser/csvparse.hpp"
)

foreach ($file in $files) {
    if (Test-Path $file) {
        Write-Host "Converting $file..." -ForegroundColor Yellow
        
        # ファイルを読み込み
        $content = Get-Content $file -Raw -Encoding UTF8
        
        # UTF-8 BOMで保存
        $utf8BOM = New-Object System.Text.UTF8Encoding $true
        [System.IO.File]::WriteAllText($file, $content, $utf8BOM)
        
        Write-Host "Converted $file to UTF-8 BOM" -ForegroundColor Green
    } else {
        Write-Host "File not found: $file" -ForegroundColor Red
    }
}

Write-Host "Conversion completed!" -ForegroundColor Green 