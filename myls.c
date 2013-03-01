#include <stdio.h>
#include <dirent.h>
#include <unistd.h>

int main(int argc, char **argv) {
  const int FN_MAX = 256;
  const int DIR_MAX = 256;

  struct dirent *de;
  DIR *d;
  char *check_dir;
  char filename[FN_MAX];
  char current_dir[DIR_MAX];

  // We will only use the first command-line arg,
  // the rest will be ignored.
  if(argc == 1) {
    if(getcwd(current_dir, DIR_MAX) == NULL) {
      fprintf(stderr, "error: couldn't get the working directory");
      return 1;
    }
    check_dir = current_dir;
  } else check_dir = argv[1];

  if((d = opendir(check_dir)) == NULL) {
    fprintf(stderr, "error: couldn't open the directory \"%s\"\n", check_dir);
    return 1;
  }

  while(de = readdir(d)) {
    if(de->d_type == DT_DIR) {
      printf("%s%s\n", "/", de->d_name);
    } else printf("%s\n", de->d_name);
  }

  closedir(d);

  return 0;
}
