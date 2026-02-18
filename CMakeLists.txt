cmake_minimum_required(VERSION 3.10)
project(JSMinifier)

# 建置 js-minifier 可執行檔（%jsm 會替換成此程式的路徑）
add_executable(js-minifier src/js-minifier.cpp)

# 加入 test 子目錄，會執行 configure_file 產生 lit.site.cfg.py
add_subdirectory(test)
