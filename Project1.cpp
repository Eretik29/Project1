#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include <clocale>
#include <windows.h>
#include <iostream>
//#include "Header.h"

const char INPUT_FILE_NAME[] = "input.txt";
const char DATA_FILE_NAME[] = "data.txt";
const char OUTPUT_FILE_NAME[] = "output.txt";


bool read_input(const char* filename, char* input_data); //Чтение шаблона из input.txt
int read_data(const char* filename, char data[10][10][1000], char macro[10][1000], int* macro_count); //Чтение данных из data.txt
void replace_macro(char* input_text, char macro[10][1000], int macro_count, char data[10][10][1000], int data_count, char output_text[10][1000]); //Замена шаблонов в тексте на данные
bool write_output(const char* filename, char output_text[10][1000], int data_count); //Запись выходных текстов в output.txt

char* str_replace(char* dst, int num, const char* str, const char* orig, const char* rep); //Поиск и замена всех подстрок orig в тексте str на подстроку rep

using namespace std;

int main()
{
	//Устанавливаем русский язык
	setlocale(LC_ALL, "Russian");

	char input[1000] = {}; //Исходный текст

	char data[10][10][1000]; //Данные
	char macro[10][1000]; //Названия макросов

	char output[10][1000]; //Выходные тексты

	//Считать исходный текст
	bool status = read_input(INPUT_FILE_NAME, input);
	// Если не найден файл шаблона
	if (!status)
	{
		printf("ERROR! %s file read error", INPUT_FILE_NAME); // Вывести сообщение об ошибке
		getchar();
		return -1;
	}

	int macro_count; //Количество макросов
	int data_count; //Количество данных для каждого макроса. Количество выходных текстов

	//Считать макросы и их данные
	data_count = read_data(DATA_FILE_NAME, data, macro, &macro_count);
	// Если не найден файл шаблона
	if (data_count == -1)
	{
		printf("ERROR! %s file read error", DATA_FILE_NAME); // Вывести сообщение об ошибке
		getchar();
		return -1;
	}
	// Если неверно указан макрос (ошибка в написании)
	if (data_count == -2)
	{
		printf("ERROR! Wrong macro in %s", DATA_FILE_NAME); // Вывести сообщение об ошибке
		getchar();
		return -1;
	}
	// Если в файле шаблона отсутствуют макросы
	if (data_count == 0)
	{
		printf("ERROR! Empty %s", DATA_FILE_NAME); // Вывести сообщение об ошибке
		getchar();
		return -1;
	}


	//Замена шаблонов в тексте на данные
	replace_macro(input, macro, macro_count, data, data_count, output);

	//Запись выходных текстов в файл
	status = write_output(OUTPUT_FILE_NAME, output, data_count);
	// Если выходные тексты не были записаны
	if (!status)
	{
		printf("%s file write error", OUTPUT_FILE_NAME); // Вывести сообщение об ошибке
		getchar();
		return -1;
	}

	//Вывести сообщение об успешном выполнении
	printf("Completed");
	getchar();

	return 0;
}

//Чтение шаблона из файла input.txt
bool read_input(const char* filename, char* input)
{
	// Устанавливаем русский язык
	setlocale(LC_ALL, "Russian");

	// Открыть файл шаблона 
	FILE* file = fopen(filename, "r");
	// Если файл шаблона не был открыт
	if (file == NULL)
	{
		return false;//Вернуть, что файл не считан
	}

	char c;
	int i = 0;
	//Пока ?не достигнут конец файла? 
	while ((c = fgetc(file)) != EOF)
	{
		//Считать текущую строку
		input[i] = c;
		i++;
	}

	fclose(file);
	return true;
}

//Чтение данных макросов из файла data.txt
int read_data(const char* filename, char data[10][10][1000], char macro[10][1000], int* macro_count)
{
	// Устанавливаем русский язык
	setlocale(LC_ALL, "Russian");
	SetConsoleCP(1251);


	// Открыть файл макросов 
	FILE* file = fopen(filename, "r");
	// Если файл макросов не был открыт
	if (file == NULL)
	{
		return -1; //Вернуть ошибку
	}

	char buf[1000];
	int i = -1;
	int j = 0;

	//Пока не достигнут конец файла
	while (fgets(buf, 1000, file) != NULL)
	{
		//Если последний символ считанного текста - символ переноса строки
		if (buf[strlen(buf) - 1] == '\n')
		{
			//Заменить последний символ на 0
			buf[strlen(buf) - 1] = 0;
		}

		// Если первый символ считанного текста - #
		if (buf[0] == '#')
		{
			//Если последний символ считанного текста - не #
			if (buf[strlen(buf) - 1] != '#')
			{
				//Вернуть ошибку
				return -2;
			}
			j = 0;
			i++;

			//Записать название макроса 
			strcpy(macro[i], buf);
		}
		// Иначе
		else
		{
			//Если до этого макрос не был считан
			if (i == -1)
			{
				//Вернуть ошибку
				return -2;
			}

			//Записать полученный текст в ?выходной текст?
			strcpy(data[i][j], buf);
			j++;
		}
	}

	//Увеличить счетчик макросов
	*macro_count = i + 1;

	fclose(file);
	return j;
}

//Замена шаблонов в тексте на данные
//Количество полученных текстов = data_count
void replace_macro(char* input_text, char macro[10][1000], int macro_count, char data[10][10][1000], int data_count, char output_text[10][1000])
{
	// Устанавливаем русский язык
	setlocale(LC_ALL, "Russian");

	//Для каждого полученного текста
	for (int i = 0; i < data_count; i++)
	{
		strcpy(output_text[i], input_text); // Скопировать текст из файла шаблона в выходной файл 
	}

	//Для каждого макроса
	for (int i = 0; i < macro_count; i++)
	{
		//Для каждого полученного текста
		for (int j = 0; j < data_count; j++)
		{
			//Вызвать функцию поиска и замены подстрок в тексте
			str_replace(output_text[j], strlen(output_text[j]), output_text[j], macro[i], data[i][j]);
		}
	}
}



//Поиск и замена всех подстрок orig в тексте str на подстроку rep
char* str_replace(char* dst, int num, const char* str, const char* orig, const char* rep)
{
	// Устанавливаем русский язык
	setlocale(LC_ALL, "Russian");

	const char* ptr;
	size_t len1 = strlen(orig);
	size_t len2 = strlen(rep);
	char* tmp = dst;

	num -= 1;
	//Пока в строке встречены заменяемые подстроки
	while ((ptr = strstr(str, orig)) != NULL)
	{
		//Отнять от числа необработанных символов строки число символов в подстроке, на которую заменяем 
		num -= (ptr - str) + len2;

		//Если обработана вся строка
		if (num < 0)
			break;//Завершить цикл

		//Заменяем найденную подстроку на нужную
		strncpy(dst, str, (size_t)(ptr - str));
		dst += ptr - str;
		strncpy(dst, rep, len2);
		dst += len2;
		str = ptr + len1;
	}

	//Сдвиг указателей вправо по строкам пока их символы совпадают и есть что сдвигать.
	for (; (*dst = *str) && (num > 0); --num)
	{
		++dst;
		++str;
	}
	return tmp;
}

//Запись выходных текстов в output.txt
bool write_output(const char* filename, char output_text[10][1000], int data_count)
{
	// Устанавливаем русский язык
	setlocale(LC_ALL, "Russian");

	// Открыть выходной файл
	FILE* file = fopen(filename, "w");
	// Если выходной файл не был открыт
	if (file == NULL)
	{
		return false; // Завершить
	}

	// Для каждого текста письма 
	for (int i = 0; i < data_count; i++)
	{
		int j = 0;
		//Пока не записан весь текст
		while (output_text[i][j] != 0)
		{
			//Записать текст письма
			fputc(output_text[i][j], file);
			j++;
		}

		//Если количество полученных текстов больше 1
		if (data_count > 1)
		{
			//Разделить пустой строкой между письмами    
			fputc('\n', file);
			fputc('\n', file);
		}
	}

	fclose(file);
	return true;
}
