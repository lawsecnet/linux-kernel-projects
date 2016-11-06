#include <stdio.h>

int main(void)
{
  FILE *myfile;
  char tempstring[1024];

  if(!(myfile=fopen("/etc/passwd", "r")))
  {
    fprint(stderr, "Cannot open file!");
    exit(1);
  }

  while(!feof(myfile))
  {
    fscanf(myfile, "%s", tempstring);
    fprint(stdout, "%s", tempstring);
  }

  exit(0);
}
