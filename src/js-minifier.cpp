// 簡易 JS Minifier：讀取 JS 檔，壓縮空白與註解後輸出
#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

static std::string readFile(const std::string& path) {
  std::ifstream f(path);
  if (!f)
    return "";
  std::ostringstream os;
  os << f.rdbuf();
  return os.str();
}

// 將連續空白（含換行）壓成單一空格，並移除單行註解
static std::string minify(const std::string& src) {
  std::string out;
  out.reserve(src.size());
  bool inSingleLineComment = false;
  bool inString = false;
  char stringChar = 0;
  bool inRegex = false;
  bool prevWasSpace = false;
  size_t i = 0;
  auto skipLineComment = [&]() {
    while (i < src.size() && src[i] != '\n')
      i++;
    if (i < src.size())
      i++;
    prevWasSpace = true;
  };

  while (i < src.size()) {
    char c = src[i];
    if (inSingleLineComment) {
      if (c == '\n') {
        inSingleLineComment = false;
        if (!prevWasSpace)
          out += ' ';
        prevWasSpace = true;
      }
      i++;
      continue;
    }
    if (inString) {
      out += c;
      if (c == '\\' && i + 1 < src.size()) {
        out += src[++i];
      } else if (c == stringChar) {
        inString = false;
      }
      i++;
      continue;
    }
    if (c == '/' && i + 1 < src.size()) {
      if (src[i + 1] == '/') {
        inSingleLineComment = true;
        i += 2;
        continue;
      }
      if (src[i + 1] == '*') {
        i += 2;
        while (i + 1 < src.size() && !(src[i] == '*' && src[i + 1] == '/'))
          i++;
        if (i + 1 < src.size())
          i += 2;
        if (!prevWasSpace)
          out += ' ';
        prevWasSpace = true;
        continue;
      }
    }
    if (c == '"' || c == '\'') {
      inString = true;
      stringChar = c;
      out += c;
      i++;
      continue;
    }
    if (c == ' ' || c == '\t' || c == '\n' || c == '\r') {
      if (!prevWasSpace)
        out += ' ';
      prevWasSpace = true;
      i++;
      continue;
    }
    prevWasSpace = false;
    out += c;
    i++;
  }
  // 去掉前後空白
  size_t start = out.find_first_not_of(" \t\r\n");
  size_t end = out.find_last_not_of(" \t\r\n");
  if (start == std::string::npos)
    return "";
  return out.substr(start, end - start + 1);
}

int main(int argc, char* argv[]) {
  std::string inputPath;
  bool outputToStdout = true;
  std::string outputPath;

  for (int i = 1; i < argc; i++) {
    std::string arg = argv[i];
    if (arg == "-o") {
      if (i + 1 >= argc) {
        std::cerr << "js-minifier: -o 需要指定輸出檔\n";
        return EXIT_FAILURE;
      }
      outputPath = argv[++i];
      outputToStdout = (outputPath == "-");
    } else if (!arg.empty() && arg[0] != '-') {
      inputPath = arg;
    }
  }

  if (inputPath.empty()) {
    std::cerr << "用法: js-minifier <input.js> [-o output.js]\n";
    return EXIT_FAILURE;
  }

  std::string content = readFile(inputPath);
  if (content.empty()) {
    std::cerr << "無法讀取: " << inputPath << "\n";
    return EXIT_FAILURE;
  }

  std::string result = minify(content);

  if (outputToStdout) {
    std::cout << result << "\n";
  } else {
    std::ofstream out(outputPath);
    if (!out) {
      std::cerr << "無法寫入: " << outputPath << "\n";
      return EXIT_FAILURE;
    }
    out << result << "\n";
  }
  return EXIT_SUCCESS;
}
