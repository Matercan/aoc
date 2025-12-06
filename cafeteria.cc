/* NOTE: This algorithmn does not! work for part 2.
 * I will be changing that shortly however. */

#include <algorithm>
#include <fstream>
#include <print>
#include <string>
#include <vector>

#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>

int main(int argc, char *argv[]) {
  if (argc < 2) {
    return 1;
  }

  std::string input{};
  std::vector<std::string> tokens{};
  std::ifstream file(argv[1]);

  std::vector<std::pair<long long, long long>> fresh_ranges{};

  while (std::getline(file, input)) {
    if (input.empty()) {
      break;
    }

    boost::split(tokens, input, boost::is_any_of("-"));

    long long first = std::stoll(tokens[0]);
    long long second = std::stoll(tokens[1]);

    fresh_ranges.push_back({first, second});
  }

  std::sort(fresh_ranges.begin(), fresh_ranges.end(),
            [](const auto &a, const auto &b) { return a.second < b.second; });
  std::vector<std::pair<long long, long long>> merged{};

  for (const auto &range : fresh_ranges) {

    if (merged.empty() || merged.back().second < range.first - 1) {
      merged.push_back(range);
    } else {
      merged.back().second = std::max(merged.back().second, range.second);
    }
  }

  long long count = 0;
  for (const auto &range : merged) {
    count += range.second - range.first + 1;
  }

  std::print("Fresh ingredients: {}\n", count);
  return 0;
}
