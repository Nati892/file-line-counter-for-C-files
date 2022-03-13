/*
This program scanns all .c files and returns the amount of lines of code in each file and in total.
-d argument allows scanning files in inner folders.
 */

#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define C_ENDING ".c"
#define H_ENDING ".h"
#define THIS_FOLDER "."
#define BACK_FOLDER ".."
#define DIG_IN "-d"
#define SLASH '/'
#define EMPTY_STRING ""
int countLinesIfC(char *);
int CountLines(char *name);
int ScanFolder(char *name, int dig_in);
char *newLink(char *trace, char *folder_name);

int main(int argc, char **argv)
{
  int dig_in = 0;
  long int result = 0;
  printf("hi, use the -d argument in order to scan files in inner folders!");
  if (argc >= 2 && !strcmp(argv[1], DIG_IN))
    dig_in = 1;

  result = ScanFolder(".", dig_in); /*start scanning from current line*/
  printf("total lines of code: %ld\n", result);
  return (0);
}

int ScanFolder(char *name, int dig_in)
{
  int i;
  char *new_trace;
  char *new_file_trace;
  int result = 0;
  DIR *d;
  struct dirent *dir;

  if (strcmp(name, THIS_FOLDER)) /*if not in mother folder*/
    printf("scanning folder %s\n", name);

  d = opendir(name);
  if (d)
  { /*loop through all files and folders in current folder*/
    while ((dir = readdir(d)) != NULL)
    {
      if (dir->d_type == 4 && strcmp(dir->d_name, THIS_FOLDER) && strcmp(dir->d_name, BACK_FOLDER) && dig_in)
      {
        if (strcmp(name, THIS_FOLDER)) /*if not in mother folder then recreate folder name*/
        {
          new_trace = newLink(name, dir->d_name);
          result += ScanFolder(new_trace, dig_in);
          free(new_trace);
        }
        else
        {
          result += ScanFolder(dir->d_name, dig_in);
        }
      }
      if (strcmp(name, THIS_FOLDER)) /*if not in mother folder then recreate file name*/
      {
        new_file_trace = newLink(name, dir->d_name);
        result += countLinesIfC(new_file_trace);
        free(new_file_trace);
      }
      else
      {
        result += countLinesIfC(dir->d_name);
      }
    }
    closedir(d);
  }
  return result;
}

/*count all lines if its a .c file*/
int countLinesIfC(char *name)
{
  long int result = 0;
  int temp = 0;
  char *name_ending;
  if (name == NULL || strlen(name) < 2)
    return 0;

  name_ending = (name + strlen(name) - 2);
  if (!strcmp(name_ending, C_ENDING) || !strcmp(name_ending, H_ENDING))
  {
    printf("file name: %s\n", name);
    temp = CountLines((name));
    if (temp > 0)
      result += temp;
    printf("number of lines in code: %d\n", temp);
  }
  return result;
}

/*actually count lines in the file*/
int CountLines(char *name)
{
  int result = 1;
  char curr_char = 0;
  FILE *mfile;
  mfile = fopen(name, "r");
  printf("scaning '%s'\n", name);
  if (mfile != NULL)
  {
    do
    {
      curr_char = getc(mfile);
      /*printf("%c", curr_char);*/
      if (curr_char == '\n')
        result++;
    } while (curr_char != EOF);
  }
  if (mfile != NULL)
    fclose(mfile);
  return result;
}
/*creates a new link*/
char *newLink(char *trace, char *folder_name)
{
  char *new_route;
  char *iterator;
  int i = 0;
  new_route = (char *)malloc((strlen(trace) + strlen(folder_name) + 2));
  iterator = new_route;
  for (i = 0; i < strlen(trace); i++)
  {
    iterator[0] = trace[i];
    iterator++;
  }
  iterator[0] = SLASH;
  iterator++;
  for (i = 0; i < strlen(folder_name); i++)
  {
    iterator[0] = folder_name[i];
    iterator++;
  }
  iterator[0] = 0;
  return new_route;
}