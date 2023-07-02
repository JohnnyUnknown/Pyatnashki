#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <chrono> 
#include "windows.h"

using namespace std;
using namespace std::chrono;

// Функция проверки вводимых пользователем значений в меню
int foolproof();
// Функция проверки вводимых пользователем значений при расстановке пятнашек 
int foolproofMix(int size);
// Функция вывода игрового поля в консоль
void showBox(int** box, int size);
// Заполнение поля значениями вручную и автоматически
void mixTile(int** box, int size, int mixing);
// Функция проверки правильности положения клеток, определение окончания игры
int victoryCheck(int** box, int size);
// Функция ручного передвижения клеток
int step(int** box, int size, int& win);
// Функция автоматического передвижения клеток (без логики)
void autoStep(int** box, int size, int& win);

int main()
{
    setlocale(LC_ALL, "Rus");
    srand(time(NULL));
    int size, mixing, builder, win = 0, time, stepCount = 0;
    cout << "\t\t\tП Я Т Н А Ш К И\n\n";
    cout << "Выберите размер поля: 3x3 - нажмите 1; 4x4 - нажмите 2. ";
    size = foolproof() + 2;                // Размер поля больше значения выбора на 2
    // Формирование массива - игрового поля
    int** box = new int* [size];
    for (int i = 0; i < size; i++) 
    {
        box[i] = new int[size];
    }
    cout << endl;

    cout << "Начальное размешивание пятнашек: Вручную - 1; Автоматическое - 2. ";
    mixing = foolproof();
    cout << endl;
    mixTile(box, size, mixing);

    //cout << "Выберите режим игры: Человек собирает - 1; Компьютер собирает (10 ходов) - 2. ";
    //builder = foolproof();
    builder = 1;
    cout << endl;

    auto start = steady_clock::now();       // Начало отсчета времени
    while (!win)
    {
        showBox(box, size);
        win = victoryCheck(box, size);
        if (!win && builder == 1)
        {
            step(box, size, win);
            stepCount++;
        }
        else if (!win && builder == 2)
        {
            autoStep(box, size, win);
            Sleep(1500);
            stepCount++;
            if (stepCount > 10) break;
        }
    }
    auto stop = steady_clock::now();        // Окончание отсчета времени
    time = duration_cast<seconds>(stop - start).count();    // Подсчет времени игры в секундах
    if (win == 1)
    {
        cout << "ПОЗДРАВЛЯЕМ!!! ВЫ ПОБЕДИЛИ\n\nПотрачено времени: " << time / 60 << " мин. " << time % 60 << " сек.\n";
        cout << "Потрачено ходов: " << stepCount << endl << endl;
    }
    else if (win == 2)
    {
        cout << "\nВ СЛЕДУЮЩИЙ РАЗ ОБЯЗАТЕЛЬНО ПОЛУЧИТСЯ.\n\nПотрачено времени: " << time / 60 << " мин. " << time % 60 << " сек.\n";
        cout << "Потрачено ходов: " << stepCount - 1 << endl << endl;
    }
    else if (win == 0)
    {
        cout << "\nВы проиграли.\n\nПотрачено времени: " << time / 60 << " мин. " << time % 60 << " сек.\n";
        cout << "Потрачено ходов: " << stepCount - 1 << endl << endl;
    }


    for (int i = 0; i < size; i++)      
    {
        delete[] box[i];
    }
    delete[] box;

    cout << "Начать заново? 1 - Да; 2 - Нет. ";
    int newGame = foolproof();
    if (newGame == 1)
    {
        system("cls");
        return main();
    }
    else cout << "\nДо свидания\n\n";
}

//-------------------------------------------------------------------------------

int foolproof() 
{
    int choice;
    cin >> choice;
    switch (choice) 
    {
    case 1:
    case 2:
        while (cin.get() != '\n')       // Очистка буфера в случае ввода дробного числа
        {     
            continue;
        }
        return choice;
    default:
        cout << "\nНекорректный ввод. Попробуй еще раз.\n";
        cin.clear();
        while (cin.get() != '\n')       // Очистка буфера в случае ввода символов
        {     
            continue;
        }
        return foolproof();
    }
}

//-------------------------------------------------------------------------------

int foolproofMix(int size) 
{
    int choice;
    cin >> choice;
    if (choice == 99) return 99;
    if (choice > 0 && choice < size * size)      // Очистка буфера в случае ввода дробного числа
    {
        while (cin.get() != '\n') 
        {     
            continue;
        }
        return choice;
    }
    else 
    {
        cout << "\nНекорректный ввод. Попробуй еще раз.\n";
        cin.clear();
        while (cin.get() != '\n')       // Очистка буфера в случае ввода символов
        {     
            continue;
        }
        return foolproofMix(size);
    }
}

//-------------------------------------------------------------------------------

void showBox(int** box, int size) 
{
    system("cls");
    for (int i = 0; i < size; i++) 
    {
        for (int j = 0; j < size; j++) 
        {
            if (box[i][j] == 0) cout << "\t[]";
            else
            cout << "\t" << box[i][j];
        }
        cout << endl << endl;
    }
    cout << endl;
}

//-------------------------------------------------------------------------------

void mixTile(int** box, int size, int mixing)
{
    int count = 0, countInternal;
    bool metka = true;
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            if (mixing == 1)        // При ручной первичной расстановке пятнашек
            {
                count++;
                if (count == size * size)
                {
                    box[i][j] = 0;    // Присвоение последней позиции значение '0' (пустая клетка)
                    cout << endl;
                }
                else
                {
                    do
                    {
                        cout << "Введите " << count << " по порядку значение. ";
                        box[i][j] = foolproofMix(size);
                        metka = true;
                        countInternal = 0;
                        for (int x = 0; x <= i; x++)        // Циклы проверки на повторение
                        {
                            for (int y = 0; y < size; y++)
                            {
                                // Внутренний счетчик для ограничения, чтобы цикл не проверял только что введенное значение
                                countInternal++;
                                if (box[i][j] == box[x][y] && countInternal < count)
                                {
                                    cout << "\nЗначения не должны повторяться!\n\n";
                                    metka = false;
                                    break;
                                }
                            }
                            if (!metka) break;
                        }
                    } while (!metka);
                }
            }
            if (mixing == 2)        // При автоматической первичной расстановке пятнашек
            {
                count++;
                if (count == size * size) box[i][j] = 0;    // Присвоение последней позиции значение '0' (пустая клетка)
                else
                {
                    do
                    {
                        box[i][j] = rand() % (size * size - 1) + 1;
                        metka = true;
                        countInternal = 0;
                        for (int x = 0; x <= i; x++)            // Циклы проверки на повторение
                        {
                            for (int y = 0; y < size; y++)
                            {
                                // Внутренний счетчик для ограничения, чтобы цикл не проверял только что введенное значение
                                countInternal++;
                                if (box[i][j] == box[x][y] && countInternal < count)
                                {
                                    metka = false;
                                    break;
                                }
                            }
                            if (!metka) break;
                        }
                    } while (!metka);
                }
            }
        }
    }
}

//-------------------------------------------------------------------------------

int step(int** box, int size, int& win)
{
    int num;
    bool metka = false;
    cout << "Введите значение для перестановки (введите 99, чтобы закончить игру): ";
    num = foolproofMix(size);
    if (num == 99) return win += 2;
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            if (num == box[i][j])       // Проверка на возможность перемещения клеток, перемещение значения на пустую клетку
            {
                if (i < size - 1)  // Ограничение для избежания ошибки при проверке значения на краю поля для каждого направления
                {
                    if (box[i + 1][j] == 0)
                    {
                        swap(box[i][j], box[i + 1][j]);
                        metka = true;
                        break;
                    }
                }
                if (i > 0)
                {
                    if (box[i - 1][j] == 0)
                    {
                        swap(box[i][j], box[i - 1][j]);
                        metka = true;
                        break;
                    }
                }
                if (j < size - 1)
                {
                    if (box[i][j + 1] == 0)
                    {
                        swap(box[i][j], box[i][j + 1]);
                        metka = true;
                        break;
                    }
                }
                if (j > 0)
                {
                    if (box[i][j - 1] == 0)
                    {
                        swap(box[i][j], box[i][j - 1]);
                        metka = true;
                        break;
                    }
                }
                if (!metka)             // Если в конце проверки не произошло перемещения клеток
                {
                    cout << "\nНедопустимый ввод. Значение должно находиться рядом с пустой клеткой.\n\n";
                    return step(box, size, win);
                }
            }
        }
        if (metka) break;
    }
}

//-------------------------------------------------------------------------------

void autoStep(int** box, int size, int& win)
{
    int num;
    bool metka = false;
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            if (box[i][j] == 0)
            {
                while (!metka)
                {
                    num = rand() % 4;
                    // Ограничение для избежания ошибки при проверке значения на краю поля для каждого направления
                    if (num == 0 && i < size - 1)
                    {
                        swap(box[i][j], box[i + 1][j]);
                        metka = true;
                        break;
                    }
                    if (num == 1 && i > 0)
                    {
                        swap(box[i][j], box[i - 1][j]);
                        metka = true;
                        break;
                    }
                    if (num == 2 && j < size - 1)
                    {
                        swap(box[i][j], box[i][j + 1]);
                        metka = true;
                        break;
                    }
                    if (num == 3 && j > 0)
                    {
                        swap(box[i][j], box[i][j - 1]);
                        metka = true;
                        break;
                    }
                }
            }
            if (metka) break;
        }
        if (metka) break;
    }
}

//-------------------------------------------------------------------------------

int victoryCheck(int** box, int size)
{
    int check = 1;
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            if (box[i][j] == check) check++;
            else if (box[i][j] == 0) continue;
            else return 0;
            if (check == size * size) return 1;
        }
    }
}