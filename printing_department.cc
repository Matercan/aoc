#include <array>
#include <fstream>
#include <print>
#include <string>
#include <utility>
#include <vector>

typedef std::vector<std::vector<bool>> grid_t;
typedef std::pair<int, int> pos_t;
#define pos_add(a, b) {a.first + b.first, a.second + b.second}

class Grid {
private:
  grid_t grid{};

  int height() const { return grid.size(); }
  int width() const { return grid[0].size(); }

  bool valid(const pos_t &pos) const {
    if (!grid.at(pos.first).at(pos.second))
      return false;

    std::array<pos_t, 8> poses = {
        {{-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 1}, {1, -1}, {1, 0}, {1, 1}}};

    int count = 0;
    for (auto const &test_pos : poses) {
      pos_t new_pos = pos_add(pos, test_pos);

      if (new_pos.first >= 0 && new_pos.first < height() &&
          new_pos.second >= 0 && new_pos.second < width()) {
        count += grid.at(new_pos.first).at(new_pos.second);
      }

      if (count >= 4)
        return false;
    }

    return true;
  }

public:
  Grid(std::vector<std::string> grid_str) {
    for (const auto &line : grid_str) {
      std::vector<bool> linebits{};
      for (const auto &ch : line) {
        if (ch == '@') // True is a toilet roll
          linebits.push_back(true);
        else
          linebits.push_back(false);
      }
      grid.push_back(linebits);
    }
  }

  long long valid_rolls() {
    int count = 0, count_two = 0;

    do {
      count_two = count;
      for (int x = 0; x < width(); x++) {
        for (int y = 0; y < height(); y++) {
          bool valid_pos = valid({x, y});

          if (valid_pos) {
            count++;
            grid[x][y] = false;
          }
        }
      }

      std::print("Current count: {}\n", count);

    } while (count != count_two);

    return count;
  }
};

int main(int argc, char *argv[]) {
  if (argc < 2) {
    return 1;
  }

  std::string input;
  std::vector<std::string> grid_str;
  std::ifstream file(argv[1]);

  while (std::getline(file, input)) {
    grid_str.push_back(input);
  }

  Grid grid(grid_str);

  std::print("Amount of valid paper rolls: {}\n", grid.valid_rolls());

  return 0;
}
