#include <unistd.h>
#include <sys/types.h>
#include <time.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>
#include <sys/prctl.h>

  // METODE BASE64
  char base64[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
  // encode
  char* base64_encode(char* plain) {
    char counts = 0;
    char buffer[3];
    char* cipher = malloc(strlen(plain) * 4/3 + 4);
    int i = 0, c = 0;

    for(i = 0; plain[i] != '\0'; i++) {
       buffer[counts++] = plain[i];
       if(counts == 3) {
         cipher[c++] = base64[buffer[0] >> 2];
         cipher[c++] = base64[((buffer[0] & 0x03) << 4) + (buffer[1] >> 4)];
         cipher[c++] = base64[((buffer[1] & 0x0f) << 2) + (buffer[2] >> 6)];
         cipher[c++] = base64[buffer[2] & 0x3f];
         counts = 0;
      }
   }

  if(counts > 0) {
        cipher[c++] = base64[buffer[0] >> 2];
        if(counts == 1) {
            cipher[c++] = base64[(buffer[0] & 0x03) << 4];
            cipher[c++] = '=';
        }
        else
        {// if counts == 2
            cipher[c++] = base64[((buffer[0] & 0x03) << 4) + (buffer[1] >> 4)];
            cipher[c++] = base64[(buffer[1] & 0x0f) << 2];
        }
        cipher[c++] = '=';
    }

    cipher[c] = '\0';
    return cipher;
}

// CIPHER = kalimat setelah diencode
// PLAIN = kalimat asli (setelah didecode)
char* base64_decode(char* cipher) {

    int counts = 0;
    int buffer[4];
    int length = strlen(cipher);
    int p = 0;

    if(length % 4 != 0) {
      return NULL;
    }

    char *plain = malloc(length * 3 / 4 + 1);
    if (!plain) return NULL;

    for(int i = 0; i < length; i++) {
       if(cipher[i] == '=') {
          buffer[counts++] = 64;
        }
        else
        {
        int k;
        for(k = 0 ; k < 64 && base64[k] != cipher[i]; k++);
        if(k == 64) {
           free(plain);
           return NULL;
        }

        buffer[counts++] = k;
        }
        if(counts == 4) {
            plain[p++] = (buffer[0] << 2) + (buffer[1] >> 4);
            if(buffer[2] != 64)
                plain[p++] = (buffer[1] << 4) + (buffer[2] >> 2);
            if(buffer[3] != 64)
                plain[p++] = (buffer[2] << 6) + buffer[3];
            counts = 0;
        }
    }

    plain[p] = '\0';
    return plain;
}

// timestamp
void wlog(char *process, char *status) {
  FILE *f = fopen("ethereal.log", "a");
  if (f == NULL) return;

  time_t now = time(NULL);
  struct tm *t = localtime(&now);

  fprintf(f, "[%02d:%02d:%04d]-[%02d:%02d:%02d]_%s_%s\n",
        t->tm_mday, t->tm_mon+1, 1900+t->tm_year, t->tm_hour, t->tm_min, t->tm_sec, process, status);

  fclose(f);
}

int main(int argc, char *argv[]) {

  if(argc < 2) {
    printf("Penggunaan:\n");
    printf("./angel -daemon\n");
    printf("./angel -decrypt\n");
    printf("./angel -kill\n");
    return 1;
  }

  // proses decrypt
  if (strcmp(argv[1], "-decrypt") == 0) {
        FILE *f = fopen("LoveLetter.txt", "r");
        if (f == NULL) {
            printf("File tidak ditemukan\n");
            wlog("decrypt", "ERROR");
            return 1;
        }

        char buffer[1024];
        fgets(buffer, sizeof(buffer), f);
        fclose(f);

        // hapus newline
        buffer[strcspn(buffer, "\n")] = 0;
        buffer[strcspn(buffer, "\r")] = 0;

        char *decoded = base64_decode(buffer);
        printf("%s\n", decoded);

        free(decoded);
        wlog("decrypt", "SUCCESS");
        return 0;
  }
  // proses kill
  if (strcmp(argv[1], "-kill") == 0) {

    int hasil = system("pkill maya");

    if (hasil == 0) {
        wlog("kill", "SUCCESS");
        printf("Daemon dihentikan.\n");
    } else {
        wlog("kill", "ERROR");
        printf("Daemon gagal dihentikan.\n");
    }

    return 0;
}

  // proses daemon
  if(strcmp(argv[1], "-daemon") == 0)
 {
  pid_t pid, sid;
  // FORK
  pid = fork();

  if (pid < 0) {
    exit(EXIT_FAILURE);
  }

  if (pid > 0) {
    exit(EXIT_SUCCESS);
  }

  umask(0);

  sid = setsid();
  if (sid < 0) {
    exit(EXIT_FAILURE);
  }

  if ((chdir(".")) < 0) {
    exit(EXIT_FAILURE);
  }

  close(STDIN_FILENO);
  close(STDOUT_FILENO);
  close(STDERR_FILENO);

  srand(time(NULL)); // fitur secret

  // Untuk ubah nama proses
  prctl(PR_SET_NAME, "maya", 0, 0, 0);
  memset(argv[0], 0, strlen(argv[0]));
  strcpy(argv[0], "maya");

  // Untuk fitur secret
  char *kalimat[] ={"1. aku akan fokus pada diriku sendiri", "2. aku mencintaimu dari sekarang hingga selamanya", "3. aku akan menjauh darimu, hingga takdir mempertemukan kita di versi kita yang terbaik.", "4. kalau aku dilahirkan kembali, aku tetap akan terus menyayangimu"};

  int jumlah = sizeof(kalimat) / sizeof(kalimat[0]);

  wlog("daemon", "RUNNING");

  while (1) {
    FILE* fptr = fopen("LoveLetter.txt", "w");
    if (fptr != NULL) {
       int index = rand() % jumlah;
       fprintf(fptr, "%s", kalimat[index]);
       fclose(fptr);
       wlog("secret", "SUCCESS");
    }
    else
    {
      wlog("secret", "ERROR");
    }

   char buffer[1024] = {0};
   FILE *fr = fopen("LoveLetter.txt", "r");
   if (fr != NULL) {
      fgets(buffer, sizeof(buffer), fr);
      fclose(fr);
   }

   char *encoded = base64_encode(buffer);

   FILE *fw = fopen("LoveLetter.txt", "w");
   if (fw != NULL) {
      fprintf(fw, "%s\n", encoded);
      fclose(fw);
      wlog("surprise", "SUCCESS");
   }
   else
   {
      wlog("surprise", "ERROR");
   }

   free(encoded);
   sleep(10);
  }
}
return 0;
}
