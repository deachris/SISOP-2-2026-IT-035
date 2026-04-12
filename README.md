# SISOP-2-2026-IT-035

**Dikerjakan oleh: Dea Chrisna Butarbutar - 5027251035**

## Reporting

### Soal 1
**Kasbon Warga Kampung Durian Runtuh**

#### Penjelasan
Langkah pertama adalah mengunduh file `buku_hutang.csv` dalam folder saat ini.
```bash
$ wget -O buku_hutang.csv "https://docs.google.com/spreadsheets/d/1NHmyS6wRO7To7ta-NLOOLHkPS6valvNaX7tawsv1zfE/export?format=csv&gid=0](https://drive.google.com/uc?export=download&id=144hsFJGoAM5lbvBQjkTfZaD3GOil4yEw)"
```

Setelah file hutang sudah ada, maka selanjutnya adalah membuat program c dengan nama file `kasir_muthu.c` untuk mengekstrak data di dalamnya.

1. Program memanggil child process pertama untuk membuat folder `brankas_kedai`. Program harus menunggu pembuatan folder selesai dibuat.
```bash
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
```

Pada program di atas, parent process akan menunggu child process selesai dijalankan dengan adanya penggunaan `wait()` dan kemudian menghasilkan output sesuai status exit.
Jika exit status != 0, maka proses gagal. Sebaliknya, jika exit status = 0, maka folder brankas berhasil dibuat dan lanjut ke proses berikutnya.
Perintah yang digunakan adalah `mkdir` untuk membuat file.

2. Program memanggil child process kedua untuk menyalin file `buku_hutang.csv` ke dalam folder `brankas_kedai`. Program harus menunggu proses sampai selesai.
```bash
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
```
Prosesnya sama seperti child process pertama, tetapi di sini perintah yang digunakan adalah `cp buku_hutang.csv brankas_kedai/` untuk menyalin file tersebut.

3. Program memanggil child process ketiga untuk mencari data dengan status "Belum Lunas" pada file `buku_hutang.csv`. Setelah dicari, hasilnya disimpan ke file `daftar_penunggak.txt` di dalam folder yang sama. Program juga harus menunggu sampai proses selesai.
```bash
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
```

Pada program di atas, perintah yang digunakan adalah dengan menjalankan perintah shell untuk mencari teks "Belum Lunas" di dalam file `buku_hutang.csv`. Hasilnya dimasukkan ke dalam file `daftar_penunggak.txt` dengan menggunakan perintah `>`.

4. Untuk child process keempat adalah mengompres brankas ke dalam file zip `rahasia_muthu.zip`. Program harus menunggu proses sampai selesai.
```bash
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
```
Pada kode di atas, perintah yang digunakan adalah `zip -r` untuk mengompres semua isi folder `brankas_kedai` ke dalam zip.
Setelah semua proses berhasil dijalankan, maka program akan mengeluarkan output tanda berhasil.

### Soal 2
**The World Never Stops, Even When You Feel Tired**

1. Langkah pertama adalah membuat program c dengan nama file `contract_daemon.c`.

2. Membuat program daemon untuk menulis log dan ditulis ke dalam file `work.log`

- Pertama, terdapat fungsi `stop()` untuk mengeluarkan output ketika daemon dihentikan atau `running = 0`. Output tersebut ditulis ke dalam file `work.log` dengan menggunakan perintah `a` agar memperbarui isinya setiap ditulis.
```bash
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
```

- Kemudian, terdapat fungsi `restore_contract` untuk mengembalikan isi file `contract.txt` dan menunjukkan timestamp ketika isi file diubah. Fungsi ini akan dipanggil ketika isi file `contract_txt` diubah. Langkah pertama adalah membuka file, kemudian jika berhasil dan isinya tidak kosong, maka akan mengeluarkan output dan timestamp. Timestamp di sini menggunakan localtime dengan command sebagai berikut. 
```bash
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
```

- Selanjutnya di fungsi main, terdapat command untuk membuat proses menjadi daemon. Child process lanjut menjadi daemon dan akan berjalan di background.
```bash
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
```

- Untuk membuat isi dari program daemon:
```bash
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
```
Pada kode di atas, daemon harus menghasilkan status acak yang sudah ditentukan dalam variabel `status`. Fungsi dari `srand` adalah untuk mengacak status agar tidak sama setiap program dijalankan.
Kemudian, terdapat loop yang dijalankan setiap detik. Pertama, file dibuka memakai mode `r` yaitu read. Jika file tidak ada, maka file akan dibuat. Jika tidak ada, file akan dioverwrite (menggunakan mode `w`). Setelah berhasil dibuka, program akan mencetak output. Ketika file baru pertama kali dijalankan, berarti akan mengeluarkan output created. Kemudian, untuk timestamp menggunakan localtime.

- Untuk membaca isi file: 
```bash
char current[256] = {0};
  FILE *check = fopen("/home/ubuntu/SISOP-2-2026-IT-035/soal_2/contract.txt", "r");
  if (check != NULL) {
      while (fgets(current + strlen(current), sizeof(current) - strlen(current), cek));
      fclose(check);
  }
```
Pada kode di atas, ketika file sudah berhasil dibuka dengan mode read, maka setiap baris yang terdapat di dalamnya di simpan ke variabel `current`. 

- Untuk mengecek isi file `contract.txt` ada perubahan atau tidak.
```bash
 if (strstr(current, "\"A promise to keep going, even when unseen.\"") == NULL) {
      FILE *log = fopen("/home/ubuntu/SISOP-2-2026-IT-035/soal_2/work.log", "a");
      if (log != NULL) {
          fprintf(log, "contract violated\n");
          fclose(log);
      }
      restore_contract();
  }
```
Pada kode di atas, isi dicek dengan menggunakan `strstr`. Jika berubah, maka program akan mencetak output bahwa isi file berubah dan akan memanggil fungsi `restore_contract()` untuk menambah output yang tadi ke dalam file.

- Membuat isi file log beserta status acak.
```bash
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
```
Pada kode di atas, isi dari variabel `index` adalah angka 0, 1, dan 2 sesuai indeks status. Kemudian, output akan dicetak beserta index yang dijalankan secara acak. Output ini akan dicetak setiap 5 detik. 

### Soal 2
**One Letter for Destiny**
1. Membuat program daemon `angel.c`.

- Membuat proses daemon untuk menjalankan command `-daemon` dari terminal.
```bash
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
```
Kode di atas berisi kode program daemon untuk menjalankan daemon. 

- Program harus mengubah nama proses dari daemon yang berjalan.
```bash
  prctl(PR_SET_NAME, "maya", 0, 0, 0);
  memset(argv[0], 0, strlen(argv[0]));
  strcpy(argv[0], "maya");
```
Kode di atas berfungsi untuk mengubah nama proses menjadi "maya". Nama proses yang lama dihapus dengan perintah `memset` yaitu mengosongkan string dari `argv[0]`. Setelah dihapus, nama proses pun diganti dengan "maya" dengan perintah `strcpy`.

- Program untuk menjalankan perulangan mencetak kalimat takdir yang dipilih dari daftar kalimat. 
```bash
  char *kalimat[] ={"1. aku akan fokus pada diriku sendiri", "2. aku mencintaimu dari sekarang hingga selamanya", "3. aku akan menjauh darimu, hingga takdir mempertemukan kita di versi kita yang terbaik.", "4. kalau aku dilahirkan kembali, aku tetap akan terus menyayangimu"};

  int jumlah = sizeof(kalimat) / sizeof(kalimat[0]); // MENGHITUNG JUMLAH KALIMAT

  wlog("daemon", "RUNNING"); // DAEMON BERJALAN

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
```
Pada kode di atas, file `LoveLetter.txt` dibuat jika belum ada dengan mode overwrite (jika sudah berisi, akan ditimpa dengan yang baru). Setelah berhasil dibuat, maka program akan memilih kalimat random dan mencetaknya di dalam file tersebut. Kemudian, output bahwa fitur secret sukses dijalankan akan dicetak di bawahnya. Hasil dari secret ini adalah kalimat asli yang diacak dari daftar kalimat. Namun, jika gagal, akan mengeluarkan output "ERROR". 
Proses encoding dari kode ini, kalimat asli yang sudah dicetak pada file dibaca dan disimpan ke dalam variabel `buffer`, kemudian diencode dengan memanggil fungsi `base64_encode`.
Setelah diencode, kalimat akan ditulis kembali ke dalam file dengan kode sebagai berikut.
```bash
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
```
Jika proses berhasil, maka program akan mencetak bahwa fitur surprise sukses dijalankan. Hasil dari surprise ini adalah kalimat yang sudah dienkripsi menggunakan metode base64 sebelumnya.

2. Metode base64

Metode base64 adalah metode untuk mengenkripsi isi file `LoveLetter.txt` menjadi data biner yang dibuat ke dalam 64 bentuk karakter teks. 
```bash
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
```
