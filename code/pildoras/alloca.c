#include <stdio.h>
#include <alloca.h>

int func () {
  char *ptr;
  ptr = alloca (1024);
  memset (ptr, 0, 1024);
  //puts ("Memoria reservada!");
  return ptr;
}

int main () {
  puts ("Probando alloca");
  func ();
  return 0;
}
