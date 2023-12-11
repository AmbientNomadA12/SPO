// V56
// А) Определить сколько раз встречается чило К во всей матрице и в каждом
// столбце. Входные данные Anxn и K
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <string>
#include <sys/wait.h>
#include <unistd.h>

#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>

#include <sys/msg.h>

using namespace std;

int msgid;

struct mymsg {
  int column;
  int count_K;
};

int **create_array(int N, int K) {
  int **A = new int *[N];
  for (int i = 0; i < N; ++i) {
    A[i] = new int[K];

    for (int j = 0; j < K; j++)
      A[i][j] = rand() % 2;
  }

  return (A);
}

void print_array(int **A, int N, int K) {
  for (int i = 0; i < N; ++i) {
    for (int j = 0; j < K; ++j)
      cout << A[i][j] << " ";
    cout << endl;
  }
}

int find_K(int **A, int N, int collumn, int K) {
  int count = 0;
  for (int i = 0; i < N; i++) {
    if (A[i][collumn] == K)
      ++count;
  }
  return (count);
}

int main() {

  setlocale(LC_ALL, "Rus");
  srand(time(NULL));

  msgid = msgget(IPC_PRIVATE, IPC_CREAT | 0666);
  if (msgid < 0) {
    cout << "Не удалось создать очередь\n";
    return (1);
  }

  int **A, N, K;

  cout << "Введите размер N квадратной матрицы А:\n";
  cin >> N;

  if (N <= 0) {
    cout << "Невозможный размер матрицы!\n";
    return (1);
  }

  A = create_array(N, N);

  print_array(A, N, N);

  cout << "Введите число K:\n";
  cin >> K;

  for (int i = 0; i < N; ++i) {
    if (fork() == 0) {
      mymsg message;
      message.column = i;
      message.count_K = find_K(A, N, i, K);
      msgsnd(msgid, &message, sizeof(mymsg), 0);
      exit(0);
    }
  }

  int count_max_K = 0;

  for (int i = 0; i < N; ++i) {
    mymsg message;
    msgrcv(msgid, &message, 2 * sizeof(int), 0, 0);
    cout << "Колличество чисел " << K << " в столбце " << message.column
         << " равно " << message.count_K << endl;
    count_max_K += message.count_K;
    wait(NULL);
  }

  cout << "Число " << K << " в матрице всего встречается " << count_max_K
       << " раза" << endl;

  for (int i = 0; i < N; i++) {
    delete[] A[i];
  }
  delete[] A;

  msgctl(msgid, IPC_RMID, NULL);

  return (0);
}