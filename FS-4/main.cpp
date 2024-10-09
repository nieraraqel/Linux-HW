#include <stdio.h>
#include <iostream>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

void initialize(int argc, char** argv){

  int file = open(argv[1], O_RDONLY);

  dup2(file, 0);
  if(file == -1) {
    printf("Error\n");
    exit(errno);
  }

}

int main(int argc, char** argv)
{
  // initialize the program based on the requirements
  initialize(argc, argv);

  // read the string
  std::string input;
  std::cin >> input;

  // logic to get the reversed string
  std::string reversed(input.rbegin(), input.rend());
  std::string s = reversed;

  // print to the console
  std::cout << s << std::endl;

  return 0;
}
