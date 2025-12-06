#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/token_functions.hpp>
#include <boost/tokenizer.hpp>

#include <algorithm>
#include <fstream>
#include <print>
#include <string>
#include <vector>

template <class T> void tokenizeV(const std::string &s, std::vector<T> &o) {
  typedef boost::tokenizer<boost::char_separator<char>> tok_t;

  boost::char_separator<char> sep(" ");
  tok_t tok(s, sep);

  for (tok_t::iterator j(tok.begin()); j != tok.end(); ++j) {
    std::string f(*j);
    boost::trim(f);
    o.push_back(boost::lexical_cast<T>(f));
  }
}

struct FileSplit {
  std::vector<std::string> before;
  std::string delimiter_line;
  std::vector<std::string> after;
  bool found = false;
};

int main(int argc, char *argv[]) {
  if (argc < 2)
    return 1;

  std::ifstream file(argv[1]);
  std::vector<std::pair<bool, std::vector<long long>>> grid{};

  FileSplit result{};
  {
    std::string line{};
    bool delimiter_found = false;

    while (std::getline(file, line)) {
      if (!delimiter_found) {
        if (line.find('+') != std::string::npos ||
            line.find('*') != std::string::npos) {

          result.delimiter_line = line;
          result.found = true;
          delimiter_found = true;
          result.after.push_back(line);
        } else {
          result.before.push_back(line);
        }
      } else {
        result.after.push_back(line);
      }
    }
  }

  {
    // First tokenize each line into number strings
    std::vector<std::vector<std::string>> rows{};
    for (const auto &line : result.before) {
      std::vector<std::string> line_tokens{};
      tokenizeV(line, line_tokens);
      rows.push_back(line_tokens);
    }

    // Find max number of columns
    size_t max_cols = 0;
    for (const auto &row : rows) {
      max_cols = std::max(max_cols, row.size());
    }

    // Process columns RIGHT TO LEFT
    for (int col = max_cols - 1; col >= 0; col--) {
      // Collect all numbers in this column from all rows
      std::vector<std::string> column_nums{};
      for (const auto &row : rows) {
        if ((size_t)col < row.size()) {
          column_nums.push_back(row[col]);
        }
      }

      // Find max digit length in this column
      size_t max_len = 0;
      for (const auto &num : column_nums) {
        max_len = std::max(max_len, num.size());
      }

      // Read each digit position from RIGHT TO LEFT
      std::vector<long long> problem_numbers{};
      for (int digit_pos = max_len - 1; digit_pos >= 0; digit_pos--) {
        std::string vertical_num = "";

        // Read this digit position from all numbers in column (top to bottom)
        for (const auto &num : column_nums) {
          if ((size_t)digit_pos < num.size()) {
            vertical_num += num[digit_pos];
          }
        }

        if (!vertical_num.empty()) {
          problem_numbers.push_back(
              boost::lexical_cast<long long>(vertical_num));
        }
      }

      grid.push_back({true, problem_numbers});
    }
  }

  {
    std::vector<char> operator_input{};
    for (const auto &line : result.after) {
      tokenizeV(line, operator_input);

      // Operators are also right-to-left, so reverse them
      std::reverse(operator_input.begin(), operator_input.end());

      for (size_t j = 0; j < operator_input.size() && j < grid.size(); j++) {
        if (operator_input[j] == '+')
          grid[j].first = true;
        else
          grid[j].first = false;
      }
    }
  }

  long long total = 0;
  {
    for (const auto &column : grid) {
      long long result = 0;

      if (column.first) {
        for (const auto &num : column.second) {
          result += num;
        }
      } else {
        result = 1;
        for (const auto &num : column.second) {
          result *= num;
        }
      }
      total += result;
    }
  }

  std::print("The grand total is: {}\n", total);
}
