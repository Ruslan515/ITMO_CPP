#include "arrays.h"

namespace mse {
    /*
    Слияние остортированных подмассивов: 
    1. array_my[p...q]
    2. array_my[q + 1...r]
    Для этого создаем 2 доп массива - left копия array_my[p...q] и right копия array_my[q + 1...r].
    Так же в конец каждого помещаем доп элемент - максимальное возможное целое число, для того что бы в 
    каждом цикле не проводить проверку о выходе за пределы одного из массивов
    */
    void merge(std::vector<std::int32_t> &array_my, int p, int q, int r)
    {
        int n_1 = q - p + 1;  // размер левого массива
        int n_2 = r - q;  // размер левого массива
        int *left, *right;
        left = new int[n_1];
        // int left[n_1];
        right = new int[n_2];
        // int right[n_2];
        // копируем массивы
        for (int i = 0; i < n_1; ++i)
        {
            left[i]= array_my[p + i];
        }
        for (int j = 0; j < n_2; ++j)
        {
            right[j] = array_my[q + j + 1];
        }
        int i = 0;  // счетчик для левого массива
        int j = 0;  // счетчик для правого массива
        int k = p;
        // цикл по основному массиву от p до r включительно
        // for (int k = p; k <= r; ++k)  // r - p + 1
        while (i != n_1 && j != n_2)
        {
            if (left[i] < right[j])  // если меньший элемент в левом массиве
            {
                array_my[k] = left[i]; // вставляем его на k-е место в массиве
                ++i;  // берем следующий элемент в левом массиве
            }
            else  // иначе миниальный элемент в правом массиве. 
            // производим анологичные действия для правого массива
            {
                array_my[k] = right[j];
                ++j;
            }
            ++k;
        }    
        if (i == n_1)
        {
            while (k <= r)
            {
                array_my[k] = right[j];
                ++j;
                ++k;
            }
        }
        else
        {
            while (k <= r)        
            {
                array_my[k] = left[i];
                ++i;
                ++k;                       
            }
            
        }
    /*     for (int i = 0; i < size; ++i)
        {     
            cout << array_my[i] << " ";
        }
        cout << "\n"; */
    }

    /*
    На входе целевой массив array_my и краевые индексы(левый - p и правый r).
    Рекурсивное разделение текущего массива на 2 отсортированных подмассива 
    и создание из этих 2х нового - отсортированного.
    */
    void merge_sort(std::vector<std::int32_t> &array_my, int p, int r, int size)
    {
        if (p < r)  // если между элементами массива с индексами p и r есть хотя бы один элемент
        {
            int q = floor((p + r) / 2);  // вычисляем точку "обрезки"
            merge_sort(array_my, p, q, size); // сортируем левую половинку
            merge_sort(array_my, q + 1, r, size);  // сортируем правую половинку
            merge(array_my, p, q, r);  // сливаем левый и правый отсортированные массивы
        }
    }

    void sort(std::vector<std::int32_t> &elements) {
        int n = elements.size();
        // сортировка слиянием для всего массива
        merge_sort(elements, 0, n - 1, n);
    }
    
    std::int32_t trace(const std::vector<std::vector<std::int32_t>> &matrix) {
        std::int32_t sum = 0;
        int n = matrix.size();
        for (int i = 0; i < n; ++i) {
            sum += matrix[i][i];
        }        
        return sum;
    }

    std::int32_t find_unique_element(const std::vector<std::int32_t> &elements) {
        std::map <std::int32_t, int> mp;
        int n = elements.size();
        for (int i = 0; i < n; ++i) {
            mp[elements[i]] = 0;
        }
        for (int i = 0; i < n; ++i) {
            mp[elements[i]]++;
        }
        
        std::int32_t answer = 0;
        std::map <std::int32_t, int> :: iterator it = mp.begin();
        while (it != mp.end()) {
            if (it->second == 1) {
                answer = it->first;
                break;
            }
            it++;
        }
        
        return answer;
    }

    std::size_t find_max_square(const std::vector<std::vector<std::int8_t>> &matrix) {
        int n = matrix.size();
        int m = matrix[0].size();

        int min_n = n;
        if (n > m) {
            min_n = m;
        }
        

        return min_n;
        
    }


} // namespace mse
