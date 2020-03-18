#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

#define TAM 10

typedef struct {
  int *a;
  int *b;
  int *c;
  int begin_sec;
  int end_sec;
  int id_sec;
} thread_data_t;

void init_random(int *vector);

void *vecsum(void *arg);

void vecshow(int *vec, int tam, char *name);

int main(int argc, char *argv[])
{
  srand((unsigned) time(NULL));

  int rc, begin, end;
  int num_threads;
  int a[TAM], b[TAM], c[TAM];

  num_threads = atoi(argv[1]);

  init_random(a);
  init_random(b);

  pthread_t thr[num_threads];

  thread_data_t thr_data[num_threads];

  int tam_sector = TAM / num_threads;

  for (int i = 0; i < num_threads; ++i) {
    begin = i * tam_sector;
    end = begin + tam_sector;
    thr_data[i].id_sec = i;
    thr_data[i].begin_sec = begin;
    thr_data[i].end_sec = end;
    thr_data[i].a = a;
    thr_data[i].b = b;
    thr_data[i].c = c;
    if ((rc = pthread_create(&thr[i], NULL, vecsum, &thr_data[i]))) {
      fprintf(stderr, "error: pthread_create, rc: %d\n", rc);
      return EXIT_FAILURE;
    }
  }

  for (int i = 0; i < num_threads; ++i) {
    pthread_join(thr[i], NULL);
  }

  vecshow(a, TAM, "A");
  vecshow(b, TAM, "B");
  vecshow(c, TAM, "C");

  return 0;
}

void init_random(int *vector)
{
  for (int i = 0; i < TAM; ++i) {
    vector[i] = rand() % 100;
  }
}

void *vecsum(void *arg)
{
  thread_data_t *thr_data = (thread_data_t *) arg;
  int id = thr_data->id_sec;
  int begin = thr_data->begin_sec;
  int end = thr_data->end_sec;

  for (int i = begin; i < end; ++i) {
    thr_data->c[i] = thr_data->a[i] + thr_data->b[i];
  }
  
  pthread_exit(NULL);
}

void vecshow(int *vec, int tam, char *name)
{
  printf("%s: ", name);

  for (int i = 0; i < tam; ++i) {
    printf("%4d", vec[i]);
  }
  printf("\n");
}
