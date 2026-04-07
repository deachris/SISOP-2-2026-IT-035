#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
  pid_t child_id;
  int status;

  child_id = fork();

  if (child_id == 0)
  {
    char *argv[] = {"mkdir", "-p", "brankas_kedai", NULL};
    execv("/bin/mkdir", argv);
  }
  else
  {
  wait(&status);
  if (WIFEXITED(status) == 0 || WEXITSTATUS(status) != 0) {
     printf("[ERROR] Aiyaa! Proses gagal, file atau folder tidak ditemukan.\n");
     exit(EXIT_FAILURE);
  }
  }

  child_id = fork();

  if (child_id == 0) {
    char *argv[] = {"cp", "buku_hutang.csv", "brankas_kedai/", NULL};
    execv("/bin/cp", argv);
  }
  else
  {
  wait(&status);
  if (WIFEXITED(status) == 0 || WEXITSTATUS(status) != 0) {
     printf("[ERROR] Aiyaa! Proses gagal, file atau folder tidak ditemukan.\n");
     exit(EXIT_FAILURE);
  }
}

  child_id = fork();

  if (child_id == 0) {
     execlp("sh", "sh", "-c", "grep 'Belum Lunas' brankas_kedai/buku_hutang.csv > brankas_kedai/daftar_penunggak.txt", NULL);
  }
  else
  {
  wait(&status);
  if (WIFEXITED(status) == 0 || WEXITSTATUS(status) != 0) {
     printf("[ERROR] Aiyaa! Proses gagal, file atau folder tidak ditemukan.\n");
     exit(EXIT_FAILURE);
  }
}

  child_id = fork();
  if (child_id == 0) {
     char *argv[] = {"zip", "-r", "rahasia_muthu.zip", "brankas_kedai", NULL};
     execv("/usr/bin/zip", argv);
  }
  else
  {
  wait(&status);
  if (WIFEXITED(status) == 0 || WEXITSTATUS(status) != 0) {
     printf("[ERROR] Aiyaa! Proses gagal, file atau folder tidak ditemukan.\n");
     exit(EXIT_FAILURE);

  }

  printf("[INFO] Fuhh, selamat! Buku hutang dan daftar penagihan berhasil diamankan.\n");
  }
  return 0;
}

