#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <signal.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>
#include <time.h>

int running = 1;
const char *contract_path = "/home/ubuntu/SISOP-2-2026-IT-035/soal_2/contract.txt";
const char *work_path = "/home/ubuntu/SISOP-2-2026-IT-035/soal_2/work.log"; 

void stop(int signal) {
  FILE *fptr = fopen("/home/ubuntu/SISOP-2-2026-IT-035/soal_2/work.log", "a");

  if (fptr != NULL) {
     fprintf (fptr, "We really weren't meant to be together\n");
     fclose(fptr);
  }
  running = 0;
}

void restore_contract() {
    FILE *f = fopen("/home/ubuntu/SISOP-2-2026-IT-035/soal_2/contract.txt", "w");
    if (f != NULL) {
        time_t now = time(NULL);
        struct tm *t = localtime(&now);
        fprintf(f, "\"A promise to keep going, even when unseen.\"\n");
        fprintf(f, "\nrestored at: %04d-%02d-%02d %02d:%02d:%02d\n",
                1900+t->tm_year, t->tm_mon+1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec);
        fclose(f);
    }
}

int main() {
  pid_t pid, sid;

  pid = fork();

  // Keluar saat fork gagal
  if (pid < 0) {
     exit(EXIT_FAILURE);
  }

// Keluar saat fork berhasil
 if (pid > 0) {
     exit(EXIT_SUCCESS);
  }

  umask(0);

  sid = setsid();
  if (sid < 0) {
    exit(EXIT_FAILURE);
  }

  if ((chdir("/")) < 0) {
    exit(EXIT_FAILURE);
  }

  close(STDIN_FILENO);
  close(STDOUT_FILENO);
  close(STDERR_FILENO);

  signal(SIGTERM, stop);
  signal(SIGINT, stop);

  srand(getpid());

  int counter = 0;

  char* status[] = {"[awake]", "[drifting]", "[numbness]"};

  while(running) {

  FILE* cek = fopen("/home/ubuntu/SISOP-2-2026-IT-035/soal_2/contract.txt", "r");

   if (cek == NULL) {
      FILE* fptr;
   fptr = fopen("/home/ubuntu/SISOP-2-2026-IT-035/soal_2/contract.txt", "w");

      if (fptr != NULL) {
          time_t now = time(NULL);
          struct tm *t = localtime(&now);

          fprintf(fptr, "\"A promise to keep going, even when unseen.\"\n");

          if (counter == 0) {
              fprintf(fptr, "\ncreated at: %04d-%02d-%02d %02d:%02d:%02d\n",
                 1900+t->tm_year, t->tm_mon+1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec);
          }
          else
          {
          fprintf(fptr, "\nrestored at: %04d-%02d-%02d %02d:%02d:%02d\n",
                 1900+t->tm_year, t->tm_mon+1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec);
          }
          fclose (fptr);
     }
   }
  else
   {
     fclose(cek);
   }

  char current[256] = {0};
  FILE *check = fopen("/home/ubuntu/SISOP-2-2026-IT-035/soal_2/contract.txt", "r");
  if (check != NULL) {
      while (fgets(current + strlen(current), sizeof(current) - strlen(current), cek));
      fclose(check);
  }

  if (strstr(current, "\"A promise to keep going, even when unseen.\"") == NULL) {
      FILE *log = fopen("/home/ubuntu/SISOP-2-2026-IT-035/soal_2/work.log", "a");
      if (log != NULL) {
          fprintf(log, "contract violated\n");
          fclose(log);
      }
      restore_contract();
  }

   if (counter % 5 == 0) {
      FILE* fptr = fopen("/home/ubuntu/SISOP-2-2026-IT-035/soal_2/work.log", "a");

      if (fptr != NULL) {
         int index = rand() % 3;

         fprintf(fptr, "still working... %s\n", status[index]);
      fclose(fptr);
      }
  }

  counter++;
  sleep(1);

}
return 0;
}
