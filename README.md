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
2. 
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
