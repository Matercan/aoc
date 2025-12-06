#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/token_functions.hpp>
#include <boost/tokenizer.hpp>

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
  std::vector<std::pair<bool, std::vector<int>>> grid{};

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
    std::vector<int> line_input{};
    for (const auto &line : result.before) {
      tokenizeV(line, line_input);

      // Generate a grid by splitting vertially instead of horizontally
      for (size_t j = 0; j < line_input.size(); j++) {
        if (grid.size() <= j) {
          grid.push_back(
              {true,
               {line_input[j]}}); // True here represents having a plus operator
        } else {
          grid[j].second.push_back(line_input[j]);
        }
      }

      line_input.clear();
    }
  }

  {
    std::vector<char> operator_input{};
    for (const auto &line : result.after) {
      tokenizeV(line, operator_input);

      for (size_t j = 0; j < operator_input.size(); j++) {

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
