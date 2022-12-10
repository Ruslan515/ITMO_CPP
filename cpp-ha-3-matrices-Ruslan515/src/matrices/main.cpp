#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>
#include <unistd.h>
#include <stdio.h>
#include <limits.h>

using namespace std;


string MUL = "--mult";
string ADD = "--add";

int MAX_SIZE_MAT = 100;


void zero_matrix(double * A, int n, int m) {
  for (auto i = 0; i < n; ++i) {
    for (auto j = 0; j < m; ++j) {
      A[i * m + j] = 0.0;
    }
  }
  return;
}


int main([[maybe_unused]] int argc, [[maybe_unused]] char ** argv)
{
  
  if (argc < 1) {
    printf("no command\n");
    return 1;
  }

  if (argc % 2 != 0) {
    printf("use ./matrices file1 --op file2 ...");
    return 2;
  }

  int n1, n2;
  int m1, m2;
  int n_out, m_out;

  string file_1 = argv[1];
  ifstream in_file_1(file_1);
  if (!in_file_1) {
    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
      printf("dir %s\n", cwd);
    }

    printf("file 1 not open\n");
    return 44;
  }
  if (!in_file_1.is_open()){
    printf("file 1 not IS OPEN\n");
    return 5;
  }

  in_file_1 >> n1 >> m1;
  n_out = n1;
  m_out = m1;

  double *A, *ans;

  A = (double *) malloc(n1 * m1 * sizeof(double));
  ans = (double *) malloc(n1 * MAX_SIZE_MAT * sizeof(double));
  zero_matrix(&ans[0], n1, n1);

  for (auto i = 0; i < n1; i++) {
    for (auto j = 0; j < m1; j++) {
      in_file_1 >> A[i * m_out + j];
      ans[i * m_out + j] = A[i * m_out + j];
    }
  }


  for (auto idx = 2; idx < argc; idx += 2) {    
    string op = argv[idx];
    string file_2 = argv[idx + 1];
    
    if (op != MUL && op != ADD) {
      printf("uncorrect op\n");
      return 3;
    }
    
    ifstream in_file_2(file_2);

    if (!in_file_2) {
      printf("file 2 not open\n");
      return 4;
    }

    if (!in_file_2.is_open()) {
      printf("file 2 not IS OPEN\n");
      return 5;
    }

    in_file_2 >> n2 >> m2;

    if (op == ADD) {
      if (!(n_out == n2 && m_out == m2)) {
        printf("in add not eq shape");
        return 6;
      }
    }
    else {
      if (m_out != n2) {
        printf("in MUL not eq m1 & m2");
        return 7;
      }
    }
    
    // double mat_2[n2][m2];
    double * mat_2;
    mat_2 = (double *) malloc (n2 * m2 * sizeof(double));
    
    if (op == ADD) {
      for (auto i = 0; i < n_out; i++) {
        for (auto j = 0; j < m_out; j++) {
          in_file_2 >> mat_2[i * m_out + j];
          ans[i * m_out + j] += mat_2[i * m_out + j];
        }
      }      


    }
    else {
      for (auto i = 0; i < n2; i++) {
        for (auto j = 0; j < m2; j++) {
          in_file_2 >> mat_2[i * m2 + j];
        }
      }
      
      m_out = m2;

      double *temp_ans;
      temp_ans = (double *) malloc(n_out * m_out * sizeof(double));
      zero_matrix(&temp_ans[0], n_out, m_out);

      for (auto i = 0; i < n_out; ++i) {
        for (auto j = 0; j < m_out; ++j) {
          for (auto k = 0; k < n2; ++k) {
            double temp = ans[i * n2 + k] * mat_2[k * m2 + j];  
            temp_ans[i * m_out + j] += temp;
          }
        }
      }

      for (auto i = 0; i < n_out; ++i) {
        for (auto j = 0; j < m_out; ++j) {
          ans[i * m_out + j] = temp_ans[i * m_out + j];
        }
      }

      free(temp_ans);
      
    }


  }

  cout << n_out << " " << m_out << "\n";
  for (auto i = 0; i < n_out; ++i) {
    for (auto j = 0; j < m_out; ++j) {
      cout << fixed << setprecision(1) << ans[i * m_out + j] << " ";
    }
    cout << "\n";
  }

  free(A);
  free(ans);
  return 0;
}

