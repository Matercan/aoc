#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <string>

#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/token_functions.hpp>
#include <boost/tokenizer.hpp>

#include <fstream>
#include <print>
#include <vector>

class Range {
  static bool is_invalid(long long number) {
    const std::string str = std::to_string(number);
    const int str_length = str.length();

    std::vector<std::string> substrings{};

    for (int i = 1; i <= str_length / 2; i++) {
      const std::string substring = str.substr(0, i);
      std::string constructed_string{};

      for (int j = 0; j < str_length / i; j++) {
        constructed_string += substring;
      }

      const int constructed_string_length = constructed_string.length();
      if (constructed_string_length != str_length)
        continue; // Can't be because constructed string length is not the
                  // string length

      if (constructed_string == str) {
        std::print("Constructed string: {}\n", constructed_string);
        return true;
      } // If the constructed string (the same i characters
        // repeated) is the string, it's invalid
    }

    return false; // Not invalid
  }

public:
  long long start_number, end_number;

  Range(long long start, long long end)
      : start_number(start), end_number(end) {};

  long long count_invalid_numbers() const {
    long long count = 0;
    for (long long i = start_number; i <= end_number; i++) {
      if (is_invalid(i)) {
        count += i;
      }
    }

    return count;
  }
};

template <class T> void tokenizeV(const std::string &s, std::vector<T> &o) {
  typedef boost::tokenizer<boost::escaped_list_separator<char>> tok_t;

  tok_t tok(s);
  for (tok_t::iterator j(tok.begin()); j != tok.end(); ++j) {
    std::string f(*j);
    boost::trim(f);
    o.push_back(boost::lexical_cast<T>(f));
  }
}

int main(int argc, char *argv[]) {
  std::ifstream file(argv[1]);

  if (argc < 2)
    return 1;

  std::string input;
  std::getline(file, input);
  std::vector<std::string> tokens;

  tokenizeV(input, tokens);
  long long count = 0;
  for (auto const &token : tokens) {

    std::vector<std::string> rangeStr;
    boost::algorithm::split(rangeStr, token, boost::is_any_of("-"));

    if (rangeStr.size() != 2) {
      continue;
    }

    Range range(std::stoll(rangeStr[0]), std::stoll(rangeStr[1]));
    long long range_count = range.count_invalid_numbers();
    count += range_count;
  }

  std::print("The amount of invalid ids: {}\n", count);
}
