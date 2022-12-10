#include "numbers.h"

namespace mse {
    std::size_t sum_digits(std::int32_t number) {
        std::int32_t x;
        std::size_t sum = 0;
        if (number < 0) {
            number *= -1;
        }
        while (number != 0)
        {
            x = number % 10;
            sum += x;
            number /= 10;
        }
        printf("sum == %ld\n", sum);
        return sum;
        
    }

    std::size_t count_primes(std::uint32_t limit) {
        if (limit < 2)
            return 0;
        int *arr;
        arr = (int *) malloc(limit * sizeof(int));
        for(size_t i = 0; i < limit; ++i){
            arr[i] = i;
        }
        
        int marker = -1;
        for (size_t i = 2; i < limit; ++i) {
            int current_num = arr[i];
            for (size_t j = current_num * 2; j < limit; j += current_num){
                arr[j] = marker;
            }
        }

        size_t count = 0;
        for (size_t i = 2; i < limit; ++i) {
            if (arr[i] != -1) {
                count++;
            }
        }

        free(arr);

        return count;
    }

    std::int32_t divide(std::int32_t a, std::int32_t b) {
    std::int32_t c = 0;
    std::int32_t max_x = __INT_MAX__;
    std::int32_t min_x = -__INT_MAX__ - 1;
    if ((a == min_x) & (b == max_x))
    {
        c = -1;
    }
    
    else if (llabs(a) >= llabs(b)) {
        if ((labs(a) == abs(b)) & (((labs(a) != a) & (labs(b) == b)) || ((labs(a) == a) & (labs(b) != b)))) {
            c = -1;
        }
        else if (labs(a) == abs(b))
        {
            c = 1;
        }
        else if (labs(b) == 1)
        {
            if (!(labs(a) != a) != !(labs(b) != b)) {
                if (b == 1) {
                    c = a;
                }
                else {
                    c = -a;
                }
                    
            }
            else {
                if (b == 1) {
                    c = a;
                }
                else {
                    if (llabs(a) > max_x) {
                        c = max_x;
                    }
                    else {
                        c = -a;
                    }                    
                }                
            }
        }
        
        
        else if (!(labs(a) != a) != !(labs(b) != b))
        {
            int i = 0;
            std::int32_t step = b;
            for (; abs(b) <= abs(a); i--) {
                b += step;
            }
            c = i;            
        }
        
        else {
            int i = 0;
            std::int32_t step = b;
            for (; abs(b) <= abs(a); i++) {
                b += step;
            }
            c = i;            
        }
    }
    return c;

    }

    std::int32_t roman_to_integer(const std::string &number) {
        // словарь соот-я римских цифр их арабским вариантам
        std::map <char, int> roman_numerals = {
            {'I', 1}, 
            {'V', 5}, 
            {'X', 10}, 
            {'L', 50}, 
            {'C', 100},
            {'D', 500},
            {'M', 1000}
            };
        int result = 0;  // пока рез-т нулевой
        char symbol;  // временная переменная в которую будем считвать символы из римской цифры
        int len_num = number.length();  // сколько символов в римской цифре
        for(int i = 0; i < len_num; ++i)
        {
            symbol = number[i];  // текущий символ
            // дошли до конца или текущий символ не меньше чем следующий
            if ((i + 1) == len_num || (roman_numerals[symbol] >= roman_numerals[number[i + 1]]))
            {
                result += roman_numerals[symbol];  // увеличиваем рез-т
            }
            else  // текущий элемент меньше следующего
            {
                result -= roman_numerals[symbol];  // уменьшаем рез-т
            }
        }
    
        return result;
    }

} // namespace mse
