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


bool read_input(const char* filename, char* input_data); //������ ������� �� input.txt
int read_data(const char* filename, char data[10][10][1000], char macro[10][1000], int* macro_count); //������ ������ �� data.txt
void replace_macro(char* input_text, char macro[10][1000], int macro_count, char data[10][10][1000], int data_count, char output_text[10][1000]); //������ �������� � ������ �� ������
bool write_output(const char* filename, char output_text[10][1000], int data_count); //������ �������� ������� � output.txt

char* str_replace(char* dst, int num, const char* str, const char* orig, const char* rep); //����� � ������ ���� �������� orig � ������ str �� ��������� rep

using namespace std;

int main()
{
	//������������� ������� ����
	setlocale(LC_ALL, "Russian");

	char input[1000] = {}; //�������� �����

	char data[10][10][1000]; //������
	char macro[10][1000]; //�������� ��������

	char output[10][1000]; //�������� ������

	//������� �������� �����
	bool status = read_input(INPUT_FILE_NAME, input);
	// ���� �� ������ ���� �������
	if (!status)
	{
		printf("ERROR! %s file read error", INPUT_FILE_NAME); // ������� ��������� �� ������
		getchar();
		return -1;
	}

	int macro_count; //���������� ��������
	int data_count; //���������� ������ ��� ������� �������. ���������� �������� �������

	//������� ������� � �� ������
	data_count = read_data(DATA_FILE_NAME, data, macro, &macro_count);
	// ���� �� ������ ���� �������
	if (data_count == -1)
	{
		printf("ERROR! %s file read error", DATA_FILE_NAME); // ������� ��������� �� ������
		getchar();
		return -1;
	}
	// ���� ������� ������ ������ (������ � ���������)
	if (data_count == -2)
	{
		printf("ERROR! Wrong macro in %s", DATA_FILE_NAME); // ������� ��������� �� ������
		getchar();
		return -1;
	}
	// ���� � ����� ������� ����������� �������
	if (data_count == 0)
	{
		printf("ERROR! Empty %s", DATA_FILE_NAME); // ������� ��������� �� ������
		getchar();
		return -1;
	}


	//������ �������� � ������ �� ������
	replace_macro(input, macro, macro_count, data, data_count, output);

	//������ �������� ������� � ����
	status = write_output(OUTPUT_FILE_NAME, output, data_count);
	// ���� �������� ������ �� ���� ��������
	if (!status)
	{
		printf("%s file write error", OUTPUT_FILE_NAME); // ������� ��������� �� ������
		getchar();
		return -1;
	}

	//������� ��������� �� �������� ����������
	printf("Completed");
	getchar();

	return 0;
}

//������ ������� �� ����� input.txt
bool read_input(const char* filename, char* input)
{
	// ������������� ������� ����
	setlocale(LC_ALL, "Russian");

	// ������� ���� ������� 
	FILE* file = fopen(filename, "r");
	// ���� ���� ������� �� ��� ������
	if (file == NULL)
	{
		return false;//�������, ��� ���� �� ������
	}

	char c;
	int i = 0;
	//���� ?�� ��������� ����� �����? 
	while ((c = fgetc(file)) != EOF)
	{
		//������� ������� ������
		input[i] = c;
		i++;
	}

	fclose(file);
	return true;
}

//������ ������ �������� �� ����� data.txt
int read_data(const char* filename, char data[10][10][1000], char macro[10][1000], int* macro_count)
{
	// ������������� ������� ����
	setlocale(LC_ALL, "Russian");
	SetConsoleCP(1251);


	// ������� ���� �������� 
	FILE* file = fopen(filename, "r");
	// ���� ���� �������� �� ��� ������
	if (file == NULL)
	{
		return -1; //������� ������
	}

	char buf[1000];
	int i = -1;
	int j = 0;

	//���� �� ��������� ����� �����
	while (fgets(buf, 1000, file) != NULL)
	{
		//���� ��������� ������ ���������� ������ - ������ �������� ������
		if (buf[strlen(buf) - 1] == '\n')
		{
			//�������� ��������� ������ �� 0
			buf[strlen(buf) - 1] = 0;
		}

		// ���� ������ ������ ���������� ������ - #
		if (buf[0] == '#')
		{
			//���� ��������� ������ ���������� ������ - �� #
			if (buf[strlen(buf) - 1] != '#')
			{
				//������� ������
				return -2;
			}
			j = 0;
			i++;

			//�������� �������� ������� 
			strcpy(macro[i], buf);
		}
		// �����
		else
		{
			//���� �� ����� ������ �� ��� ������
			if (i == -1)
			{
				//������� ������
				return -2;
			}

			//�������� ���������� ����� � ?�������� �����?
			strcpy(data[i][j], buf);
			j++;
		}
	}

	//��������� ������� ��������
	*macro_count = i + 1;

	fclose(file);
	return j;
}

//������ �������� � ������ �� ������
//���������� ���������� ������� = data_count
void replace_macro(char* input_text, char macro[10][1000], int macro_count, char data[10][10][1000], int data_count, char output_text[10][1000])
{
	// ������������� ������� ����
	setlocale(LC_ALL, "Russian");

	//��� ������� ����������� ������
	for (int i = 0; i < data_count; i++)
	{
		strcpy(output_text[i], input_text); // ����������� ����� �� ����� ������� � �������� ���� 
	}

	//��� ������� �������
	for (int i = 0; i < macro_count; i++)
	{
		//��� ������� ����������� ������
		for (int j = 0; j < data_count; j++)
		{
			//������� ������� ������ � ������ �������� � ������
			str_replace(output_text[j], strlen(output_text[j]), output_text[j], macro[i], data[i][j]);
		}
	}
}



//����� � ������ ���� �������� orig � ������ str �� ��������� rep
char* str_replace(char* dst, int num, const char* str, const char* orig, const char* rep)
{
	// ������������� ������� ����
	setlocale(LC_ALL, "Russian");

	const char* ptr;
	size_t len1 = strlen(orig);
	size_t len2 = strlen(rep);
	char* tmp = dst;

	num -= 1;
	//���� � ������ ��������� ���������� ���������
	while ((ptr = strstr(str, orig)) != NULL)
	{
		//������ �� ����� �������������� �������� ������ ����� �������� � ���������, �� ������� �������� 
		num -= (ptr - str) + len2;

		//���� ���������� ��� ������
		if (num < 0)
			break;//��������� ����

		//�������� ��������� ��������� �� ������
		strncpy(dst, str, (size_t)(ptr - str));
		dst += ptr - str;
		strncpy(dst, rep, len2);
		dst += len2;
		str = ptr + len1;
	}

	//����� ���������� ������ �� ������� ���� �� ������� ��������� � ���� ��� ��������.
	for (; (*dst = *str) && (num > 0); --num)
	{
		++dst;
		++str;
	}
	return tmp;
}

//������ �������� ������� � output.txt
bool write_output(const char* filename, char output_text[10][1000], int data_count)
{
	// ������������� ������� ����
	setlocale(LC_ALL, "Russian");

	// ������� �������� ����
	FILE* file = fopen(filename, "w");
	// ���� �������� ���� �� ��� ������
	if (file == NULL)
	{
		return false; // ���������
	}

	// ��� ������� ������ ������ 
	for (int i = 0; i < data_count; i++)
	{
		int j = 0;
		//���� �� ������� ���� �����
		while (output_text[i][j] != 0)
		{
			//�������� ����� ������
			fputc(output_text[i][j], file);
			j++;
		}

		//���� ���������� ���������� ������� ������ 1
		if (data_count > 1)
		{
			//��������� ������ ������� ����� ��������    
			fputc('\n', file);
			fputc('\n', file);
		}
	}

	fclose(file);
	return true;
}
