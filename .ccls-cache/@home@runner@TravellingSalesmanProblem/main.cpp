#include <iostream>

using namespace std;

/* Класс маршрут */
class Route
{                                                   
private:
    int* m_route;                                                       // m_route – массив, представляющий маршрут                                                          
    int m_citiesNumber;                                                 // m_citiesNumber – количество городов                                                  

public:
    Route(int number = 0);                                              // Конструктор (заполнение массива m_route числами от 1 до m_citiesNumber)
    Route(const Route& other);                                          // Конструктор копирования

    ~Route();                                                           // Деструктор

    Route& operator=(const Route& other);                               // Оператор присваивания

    int CalculateRouteCost(int** arr);                                  // Вычисление стоимости маршрута (с помощью матрицы стоимости)
    bool NextRoute();                                                   // Вычисление следующего маршрута (с помощью алгоритма Дейкстры)

    friend ostream& operator<<(ostream& os, const Route& other);        // Вывод маршрута (дружественная функция)
};

/* Конструктор (заполнение массива m_route числами от 1 до m_citiesNumber) */
Route::Route(int number)
{
    /* Объявляем начальный путь */
    m_citiesNumber = number;                                // Количество городов                             
    m_route = new int[number];                              // Массив маршрута

    /* Заполняем массив числами от 1 до number */
    for (int i = 0; i < number; i++)
    {    
        m_route[i] = i + 1;
    }
}

/* Конструктор копирования */
Route::Route(const Route& other)
{
    /* Копируем длину маршрута */
    m_citiesNumber = other.m_citiesNumber;                  // m_citiesNumber - количествово городов, other.m_citiesNumber - количество городов передаваемого массива
    /* Выделяем память, чтобы создать новый массивЮ содержащий копии городов из маршрута other */
    m_route = new int[other.m_citiesNumber];                

    /* Копируем каждый город из other в новый массив m_route */
    for (int i = 0; i < other.m_citiesNumber; i++)
    {
        m_route[i] = other.m_route[i];
    }
}

/* Деструктор */
Route::~Route()
{
    /* Проверяем, существует ли указатель m_route (если не равен nullptr, то выполняется блок кода внутри условия) */
    if (m_route)
    {
        delete[] m_route;
    }

    /* После удаления памяти устанавливаем значение nullptr, чтобы избежать ошибок при работе с памятью */
    m_route = nullptr;
}

/* Оператор присваивания */
Route& Route::operator=(const Route& other)
{
    /* Проверяем, является ли текущий объект тем же самым, который передаётся в качестве аргумента */
    if (this == &other)
    {
        /* Если это так, возвращаем ссылку на текущий объект без выполнения дальнейших действий */
        return *this;
    }

    /* Освобождаем занимаемую память для текущего маршрута */
    delete(m_route);

    /* Присваиваем текущему объекту значение переменной m_citiesNumber из объекта other, переданного при вызове оператора присваивания */
    m_citiesNumber = other.m_citiesNumber;
    /* Выделяем память для нового массива m_route с размером other.m_citiesNumber */
    m_route = new int[other.m_citiesNumber];

    /* Копируем каждый элемент из маршрута объекта other в новый массив m_route текущего объекта */
    for (int i = 0; i < other.m_citiesNumber; i++)
    {
        m_route[i] = other.m_route[i];
    }

    /* Возвращаем ссылку на текущий объект (результат оператора присваивания) */
    return *this;
}

/* Вычисление стоимости маршрута (с помощью матрицы стоимости) */
int Route::CalculateRouteCost(int** arr)
{
    /* Подсчитываем стоимость маршрута */
    int cost = 0;                                                   // cost - цена перестановки
    int nextCity;                                                   // nextCity - номер следующего города

    for (int i = 0; i < m_citiesNumber - 1; i++)
    {
        nextCity = m_route[i + 1];                                  // Номер следующего города
        cost += arr[m_route[i] - 1][nextCity - 1];                  // Стоимость путей, где (m_route[i] - 1) - это строка, а (nextCity - 1) - это столбец
    }

    nextCity = m_route[0];                                          // Начальный город (откуда начался цикл)
    cost += arr[m_route[m_citiesNumber - 1] - 1][nextCity - 1];     // Путь из последнего в первый для цикла

    return cost;                                                    //Возращаем стоимоть всей перестановки
}

/* Вычисление следующего маршрута (с помощью алгоритма Дейкстры) */
bool Route::NextRoute()
{
    int i;                                                          // Переменная i для алгоритма Дейкстры
    int k;                                                          // Переменная k для алгоритма Дейкстры

    bool flag = false;                                              // Флаг для проверки выполняемости условий

    /* поиск максимального i, подоходящему по условиям */
    for (i = m_citiesNumber - 2; i > -1; i--)
    {
        /* Если нашли, устанавливаем флаг в true и переходим к следующему шагу алгоритма */
        if (m_route[i] < m_route[i + 1])
        {
            flag = true; 

            break;
        }
    }

    /* Если нашли максимальное i, то ищем максимальное k */
    if (flag)
    {                     
        for (k = m_citiesNumber - 1; k > i; k--)
        {
            /* Если нашли, выходим из цикла */
            if (m_route[i] < m_route[k])
            {
                break;
            }
        }

        /* Меняем i-ый и j-ый элементы местами */
        int tmp;
        tmp = m_route[i];
        m_route[i] = m_route[k];
        m_route[k] = tmp;

        /* Упорядочиваем "хвост" по возрастанию (инвертируем порядок) */
        if (i < m_citiesNumber - 2)
        {        
            int left = i + 1;
            int right = m_citiesNumber - 1;

            while (left < right)
            {
                tmp = m_route[left];
                m_route[left] = m_route[right];
                m_route[right] = tmp;

                left++;
                right--;
            }
        }

        /* Отсеиваем перестановки со смещением */
        if (m_route[0] != 1)
        {
            flag = false;
        }

        return flag;
    }
    /* Если i не нашлось, то следующей перестановки нет */
    else                            
    {
        return flag;
    }
}

/* Вывод маршрута (дружественная функция) */
ostream& operator<<(ostream& os, const Route& other)
{
    /* Проходимся по каждому элементу маршрута в объекте other */
    for (int i = 0; i < other.m_citiesNumber; i++)
    {
        /* Выводим каждый элемент маршрута через поток os */
        os << other.m_route[i] << " ";
    }

    /* Возвращаем поток os для поддержки цепочки операций вывода */
    return os;
}

/* Решение задачи коммивояжера */
Route TravelingSalesman(int number)
{       
    /* Выделяем память под матрицу стоимости */
    int** matrix = new int* [number];

    for (int i = 0; i < number; i++)
    {
        matrix[i] = new int[number];
    }

    /* Заполняем матрицу размерностью number на number */
    for (int i = 0; i < number; i++)
    {         
        for (int k = 0; k < number; k++)
        {
            cin >> matrix[i][k];
        }
    }

    cout << "\n";

    /* Выводим матрицу */
    for (int i = 0; i < number; i++)
    {          
        for (int k = 0; k < number; k++)
        {
            cout << matrix[i][k] << " ";
        }

        cout << "\n";
    }

    Route initialRoute(number);                                                    // Начальный путь по возрастанию (1, 2, ..., number)
    Route minimumRoute = initialRoute;                                             // Минимальный маршрут

    int initialCost = initialRoute.CalculateRouteCost(matrix);                     // Стоимость первоначального маршрута

    cout << "\n" << initialRoute << " : " << initialCost << "\n" << endl;          // Вывод начального пути и его стоимости

    /* Пока существует следующая перестановка */
    while (initialRoute.NextRoute())
    {        
        int nextPermutationCost = initialRoute.CalculateRouteCost(matrix);         // Стоимость следующей перестановки
        cout << initialRoute << " : " << nextPermutationCost << endl;              // Вывод перестановки и её стоимости

        /* Если путь дешевле, то выбираем его */
        if (initialCost > nextPermutationCost && nextPermutationCost > 0)
        {     
            initialCost = nextPermutationCost;
            minimumRoute = initialRoute;
        }

    }

    cout << "\n";

    /* Возвращаем путь с наименьшей стоимостью (лучший путь) */
    return minimumRoute;
}

int main()
{
    setlocale(LC_ALL, "Rus");

    cout << "Введите количество строк (столько же будет и столбцов): ";
    int costMatrixSize;
    cin >> costMatrixSize;

    cout << "Введите матрицу размерностью " << costMatrixSize << " на " << costMatrixSize << "элемента(-ов):" << endl;
    cout << TravelingSalesman(costMatrixSize);

    return 0;
}