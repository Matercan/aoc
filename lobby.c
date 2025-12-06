#include <stdio.h>

long long *max(long long *arr, int dist) {
  long long *cd = arr, *cur = arr;

  for (; *(cur + dist) != '\0'; cur++) {
    if (*cur > *cd) {
      cd = cur;
    }
  }

  return cd;
}

long long power(long long a) {
  int ret = 1, tmp = a;

  while (tmp-- != 0) {
    ret *= 10;
  }

  return ret;
}

int main(int argc, char *argv[]) {
  if (argc < 2)
    return 1;

  long long n = 0, numbers[512];
  char line[512];

  FILE *fptr = fopen(argv[1], "r");

  while (fgets(line, sizeof(line), fptr)) {
    long long *digits[12];
    long long *current = numbers;

    int count = 0;
    for (char *num = line; *num != '\n'; num++) {
      numbers[count] = (*num - 0x30);
      count++;
    }
    numbers[count] = '\0';

    for (int i = 0; i < 12; i++) {
      digits[i] = max(current, 12 - i - 1);
      current = digits[i] + 1;
    }

    long long newnum = 0;
    for (int i = 0; i < 12; i++) {
      newnum += *(digits[i]) * power(12 - i - 1);
    }

    n += newnum;
  }

  printf("The max number of joltage: %lld\n", n);
  fclose(fptr);
}
