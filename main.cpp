#include <iostream>
#include <future>
#include <chrono>
#include <vector>
#include <random>
using namespace std;

bool make_thread = true;

/* 
list - список элемнгтов дл€ сортировки
 begin - начало сортируемой части
end - конец сортируемой части
*/

/*---ќпределение---*/

void mergeSort(vector<int>& list, int begin, int end);

void merge(vector<int>& list, int begin, int m, int end);

void getSortedArray(const vector<int>& list);

int main()
{
    setlocale(LC_ALL, "rus");
    vector<int> arr;

    for (int i = 0; i <= 300; ++i)
        arr.push_back(rand() % 500);

    // многопоточный запуск
    auto start = chrono::high_resolution_clock::now();

    mergeSort(arr, 0, arr.size() -1);

    auto end = chrono::high_resolution_clock::now();

    chrono::duration<float> second = end - start;
    cout << "ћногопоточный запуск, врем€ работы = " << second.count() << endl;

    cout << "ќтсортированный массив: " << endl;

    getSortedArray(arr);

    arr.clear();

    //cout << "-------------------" << endl;

    for (int i = 0; i <= 300; ++i)
        arr.push_back(rand() % 500);

    // однопоточный запуск
    make_thread = false;

    start = chrono::high_resolution_clock::now();

    mergeSort(arr, 0, arr.size() - 1);

    end = chrono::high_resolution_clock::now();
    second = end - start;

    cout << "ќднопоточный запуск, врем€ работы = " << second.count() << endl;
    cout << "ќтсортированный массив: " << endl;

    getSortedArray(arr);

    arr.clear();
}

void mergeSort(vector<int>& list, int begin, int end)
{
	if (begin >= end) // условие выхода из рекурсии
		return;

	int m = (begin + end - 1) / 2; // –азбиваем вход€щий массив на два

    /*---¬ыполн€ем рекурсивный вызов дл€ этих двух частей---*/
    //if (make_thread == true && (begin + m - 1 > 100))
    if (make_thread && (begin + m - 1 > 100))
    {
        future<void> f = async(launch::async, [&]() 
            {
                mergeSort(list, begin, m);
            });

        mergeSort(list, m + 1, end);
        merge(list, begin, m, end);
    }
    else
    {
        mergeSort(list, begin, m);
        mergeSort(list, m + 1, end);
        merge(list, begin, m, end);
    }
}

void merge(vector<int>& list, int begin, int m, int end)
{
    int nl = m - begin + 1;
    int nr = end - m;

    // создаем временные массивы
    int* left = new int[nl];
    int* right = new int[nr];

    // копируем данные во временные массивы
    for (int i = 0; i < nl; i++)
        left[i] = list[begin + i];
    for (int j = 0; j < nr; j++)
        right[j] = list[m + 1 + j];

    int i = 0, j = 0;
    int k = begin;  // начало левой части

    while (i < nl && j < nr) {
        // записываем минимальные элементы обратно во входной массив
        if (left[i] <= right[j]) {
            list[k] = left[i];
            i++;
        }
        else {
            list[k] = right[j];
            j++;
        }
        k++;
    }
    // записываем оставшиес€ элементы левой части
    while (i < nl) {
        list[k] = left[i];
        i++;
        k++;
    }
    // записываем оставшиес€ элементы правой части
    while (j < nr) {
        list[k] = right[j];
        j++;
        k++;
    }

    delete[] left;
    delete[] right;
}

void getSortedArray(const vector<int>& list)
{
    for (int item : list)
        cout << item << " ";

    cout << endl;
}
