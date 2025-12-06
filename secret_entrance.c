
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  if (argc < 2)
    return 1;

  int n = 0, d = 50, pred = 50;
  char line[16];

  FILE *fptr = fopen(argv[1], "r");

  while (fgets(line, sizeof(line), fptr)) {
    bool l = line[0] == 'L';
    int dist = atoi(line + 1);
    int turns = dist / 100; // full revolutions
    int rem = dist % 100;   // leftover movement

    pred = d;

    n += turns;

    if (l) {
      d = (pred + 100 - rem) % 100;

      if (rem > pred)
        n++;

    } else {
      d = (pred + rem) % 100;

      if (pred + rem >= 100)
        n++;
    }
  }

  printf("Elves, your password is: %d\n", n);
  return 0;
}
