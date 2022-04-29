#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <Windows.h>
using namespace std;

#pragma region "Прізвища студентів"
string surnames[] =
{
	"Бурачинський",
	"Кваша",
	"Попенко",
	"Жученко",
	"Пероганич",
	"Карпінський",
	"Дроб\'язко",
	"Боровий",
	"Приступа",
	"Костюк",

	"Гайдук",
	"Грицина",
	"Балога",
	"Величковська",
	"Марченко",
	"Бершеда",
	"Лимаренко",
	"Кравченко",
	"Ластівка",
	"Бельська",
};
#pragma endregion

enum class Spec { KN, II, ME, PI, PE };
string strSpec[] = { "Компютерні науки", "Інформатика", "Математика та економіка", "Фізика та інформатика", "Трудове навчання" };


struct Student
{
	string surname;
	int course;
	Spec spec;
	double mark_physic;
	double mark_math;
	union
	{
		double mark_programic;
		double mark_chisel_method;
		double mark_pedagogic;
	} mark_last;
};

Student Generate()
{
	Student s;

	s.surname = surnames[rand() % 20];
	s.course = 1 + rand() % 6;
	s.spec = Spec(rand() % 4);

	s.mark_physic = (30 + rand() % 20) / 10.0;
	s.mark_math = (30 + rand() % 20) / 10.0;
	s.mark_last.mark_programic = (30 + rand() % 20) / 10.0;
	return s;
}

void generate_to_file(string path, size_t cnt)
{
	fstream f(path, ios::binary | ios::out);
	if (f.is_open())
	{
		f.write((char*)&cnt, sizeof(cnt));
		for (size_t i = 0; i < cnt; i++)
		{
			Student s = Generate();
			size_t l = s.surname.length() + 1;
			f.write((char*)&l, sizeof(l));
			f.write(s.surname.c_str(), l);
			f.write((char*)&s.course, sizeof(int));
			f.write((char*)&s.spec, sizeof(Spec));
			f.write((char*)&s.mark_physic, sizeof(double));
			f.write((char*)&s.mark_math, sizeof(double));
			f.write((char*)&s.mark_last, sizeof(s.mark_last));
		}
	}
	else
		cout << "load_to_file()" << endl;
	f.close();
}

double avgMark(Student s)
{
	return round(((s.mark_last.mark_programic + s.mark_math + s.mark_physic) / 3.0) * 100) / 100;
}

void display(string path, bool avg = false)
{
	fstream f(path, ios::binary | ios::in);
	if (f.is_open())
	{
		cout << "-------------------------------------------------------------------------------------------------------------";
		if (avg)
			cout << "------";
		cout << endl;
		cout << "|  №  | курс |    прізвище    |       спеціальність     | фізика | математика | Прогр. | Чис. мет. | Педаг. |";
		if (avg)
			cout << " avg |";
		cout << endl;
		cout << "-------------------------------------------------------------------------------------------------------------";
		if (avg)
			cout << "------";
		cout << endl;

		cout << left;
		size_t cnt;
		f.read((char*)&cnt, sizeof(size_t));
		for (size_t i = 0; i < cnt; i++)
		{
			Student s;
			size_t c;
			f.read((char*)&c, sizeof(size_t));
			char* data = new char[c];
			f.read(data, c);
			s.surname = data;
			delete[] data;
			f.read((char*)&s.course, sizeof(int));
			f.read((char*)&s.spec, sizeof(Spec));
			f.read((char*)&s.mark_physic, sizeof(double));
			f.read((char*)&s.mark_math, sizeof(double));
			f.read((char*)&s.mark_last, sizeof(s.mark_last));

			cout << "|" << setw(5) << i + 1 << "|"
				<< setw(6) << s.course << "|"
				<< setw(16) << s.surname << "|"
				<< setw(25) << strSpec[int(s.spec)] << "|"
				<< setw(8) << s.mark_physic << "|"
				<< setw(12) << s.mark_math << "|";
			if (s.spec == Spec::KN)
				cout << setw(8) << s.mark_last.mark_programic;
			else
				cout << setw(8) << " ";
			cout << "|";
			if (s.spec == Spec::II)
				cout << setw(11) << s.mark_last.mark_chisel_method;
			else
				cout << setw(11) << " ";
			cout << "|";
			if (s.spec != Spec::KN && s.spec != Spec::II)
				cout << setw(8) << s.mark_last.mark_pedagogic;
			else
				cout << setw(8) << " ";
			cout << "|";
			if (avg)
				cout << setw(5) << avgMark(s) << "|";
			cout << endl;
		}
		cout << "-------------------------------------------------------------------------------------------------------------";
		if (avg)
			cout << "------";
		cout << endl;
	}
	else
		cout << "display()" << endl;
	f.close();
}

void displayIndex(string data_path, string index_path, bool avg = false)
{
	int* I = nullptr;
	size_t cnt = 0;
	fstream f(index_path, ios::binary | ios::in);
	if (f.is_open())
	{
		int index;
		while (f.read((char*)&index, sizeof(index)))
			cnt++;

		I = new int[cnt];

		int i = 0;
		f.clear();
		f.seekg(0, ios::beg);
		while (f.read((char*)&index, sizeof(index)))
			I[i++] = index;
	}
	else
		cout << "displayIndex() 1" << endl;
	f.close();

	size_t cnt1 = 0;
	Student* stud = nullptr;
	f = fstream(data_path, ios::binary | ios::in);
	if (f.is_open())
	{
		f.read((char*)&cnt1, sizeof(size_t));
		stud = new Student[cnt1];
		for (size_t i = 0; i < cnt1; i++)
		{
			Student s;
			size_t c;
			f.read((char*)&c, sizeof(size_t));
			char* data = new char[c];
			f.read(data, c);
			s.surname = data;
			delete[] data;
			f.read((char*)&s.course, sizeof(int));
			f.read((char*)&s.spec, sizeof(Spec));
			f.read((char*)&s.mark_physic, sizeof(double));
			f.read((char*)&s.mark_math, sizeof(double));
			f.read((char*)&s.mark_last, sizeof(s.mark_last));

			stud[i] = s;
		}
	}
	else
		cout << "displayIndex() 2" << endl;
	f.close();

	if (cnt != cnt1) {
		cout << "displayIndex() 3" << endl;
		return;
	}

	cout << "---------------------------------------------------------------------------------------------";
	if (avg)
		cout << "------";
	cout << endl;
	cout << "|  №  | курс |    прізвище    |       спеціальність     | фізика | математика | інформатика |";
	if (avg)
		cout << " avg |";
	cout << endl;
	cout << "---------------------------------------------------------------------------------------------";
	if (avg)
		cout << "------";
	cout << endl;
	cout << left;

	for (size_t i = 0; i < cnt; i++)
	{
		cout << "|" << setw(5) << I[i] + 1 << "|"
			<< setw(6) << stud[I[i]].course << "|"
			<< setw(16) << stud[I[i]].surname << "|"
			<< setw(25) << strSpec[int(stud[I[i]].spec)] << "|"
			<< setw(8) << stud[I[i]].mark_physic << "|"
			<< setw(12) << stud[I[i]].mark_math << "|"
			<< setw(13) << stud[I[i]].mark_last.mark_programic << "|";
		if (avg)
			cout << setw(5) << avgMark(stud[I[i]]) << "|";
		cout << endl;
	}
	cout << "---------------------------------------------------------------------------------------------";
	if (avg)
		cout << "------";
	cout << endl;

	delete[] I;
	delete[] stud;
}

void RealSort(string path)
{
	fstream f(path, ios::binary | ios::in);
	size_t cnt = 0;
	Student* stud = nullptr;
	if (f.is_open())
	{
		f.read((char*)&cnt, sizeof(size_t));
		stud = new Student[cnt];
		for (size_t i = 0; i < cnt; i++)
		{
			Student s;
			size_t c;
			f.read((char*)&c, sizeof(size_t));
			char* data = new char[c];
			f.read(data, c);
			s.surname = data;
			delete[] data;
			f.read((char*)&s.course, sizeof(int));
			f.read((char*)&s.spec, sizeof(Spec));
			f.read((char*)&s.mark_physic, sizeof(double));
			f.read((char*)&s.mark_math, sizeof(double));
			f.read((char*)&s.mark_last, sizeof(s.mark_last));

			stud[i] = s;
		}
	}
	else
		cout << " RealSort() 1" << endl;
	f.close();

	for (size_t i = 0; i < cnt; i++)
	{
		for (size_t j = 0; j < cnt - i - 1; j++)
		{
			double j0 = avgMark(stud[j]);
			double j1 = avgMark(stud[j + 1]);
			if (j0 < j1 || j0 == j1 && stud[j].mark_last.mark_programic < stud[j + 1].mark_last.mark_programic
				|| j0 == j1 && stud[j].mark_last.mark_programic == stud[j + 1].mark_last.mark_programic && stud[j].surname < stud[j + 1].surname)
				swap(stud[j], stud[j + 1]);
		}
	}

	f = fstream(path, ios::binary | ios::out);
	if (f.is_open())
	{
		f.write((char*)&cnt, sizeof(size_t));
		for (size_t i = 0; i < cnt; i++)
		{
			size_t l = stud[i].surname.length() + 1;
			f.write((char*)&l, sizeof(l));
			f.write(stud[i].surname.c_str(), l);
			f.write((char*)&stud[i].course, sizeof(int));
			f.write((char*)&stud[i].spec, sizeof(Spec));
			f.write((char*)&stud[i].mark_physic, sizeof(double));
			f.write((char*)&stud[i].mark_math, sizeof(double));
			f.write((char*)&stud[i].mark_last, sizeof(stud[i].mark_last));
		}
	}
	else
		cout << "RealSort() 2" << endl;
	delete[] stud;
	f.close();
}

int* IndexSort(string path, string new_path)
{
	fstream f(path, ios::binary | ios::in);
	size_t cnt = 0;
	Student* stud = nullptr;
	if (f.is_open())
	{
		f.read((char*)&cnt, sizeof(size_t));
		stud = new Student[cnt];
		for (size_t i = 0; i < cnt; i++)
		{
			Student s;
			size_t c;
			f.read((char*)&c, sizeof(size_t));
			char* data = new char[c];
			f.read(data, c);
			s.surname = data;
			delete[] data;
			f.read((char*)&s.course, sizeof(int));
			f.read((char*)&s.spec, sizeof(Spec));
			f.read((char*)&s.mark_physic, sizeof(double));
			f.read((char*)&s.mark_math, sizeof(double));
			f.read((char*)&s.mark_last, sizeof(s.mark_last));

			stud[i] = s;
		}
	}
	else
		cout << " IndexSort() 1" << endl;
	f.close();

	int* I = new int[cnt];
	for (size_t i = 0; i < cnt; i++)
		I[i] = i;

	int i, j, value;
	for (size_t i = 1; i < cnt; i++)
	{
		value = I[i];
		for (j = i - 1;
			j >= 0 && (avgMark(stud[I[j]]) < avgMark(stud[value]) ||
				avgMark(stud[I[j]]) == avgMark(stud[value]) && stud[j].mark_last.mark_programic < stud[j + 1].mark_last.mark_programic ||
				avgMark(stud[I[j]]) == avgMark(stud[value]) && stud[j].mark_last.mark_programic == stud[j + 1].mark_last.mark_programic && stud[j].surname < stud[j + 1].surname);
			j--)
		{
			I[j + 1] = I[j];
		}
		I[j + 1] = value;
	}

	delete[] stud;

	f = fstream(new_path, ios::binary | ios::out);
	if (f.is_open())
	{
		for (size_t i = 0; i < cnt; i++)
			f.write((char*)&I[i], sizeof(I[i]));
	}
	else
		cout << "IndexSort() 2" << endl;
	delete[] I;
	f.close();

	return I;
}

int BinSearch(string path, string surname, double avg, double last_mark)
{
	fstream f(path, ios::binary | ios::in);
	int index = 0;
	size_t cnt = 0;
	Student* s = nullptr;
	if (f.is_open()) {
		f.read((char*)&cnt, sizeof(size_t));
		s = new Student[cnt];
		for (size_t i = 0; i < cnt; i++)
		{
			Student stud;
			size_t c;
			f.read((char*)&c, sizeof(size_t));
			char* data = new char[c];
			f.read(data, c);
			stud.surname = data;
			delete[] data;
			f.read((char*)&stud.course, sizeof(int));
			f.read((char*)&stud.spec, sizeof(Spec));
			f.read((char*)&stud.mark_physic, sizeof(double));
			f.read((char*)&stud.mark_math, sizeof(double));
			f.read((char*)&stud.mark_last, sizeof(stud.mark_last));

			s[i] = stud;
		}
	}
	else
		cout << "BinSearch()" << endl;

	int l = 0, r = cnt - 1, m;
	do
	{
		m = (l + r) / 2;
		if (s[m].surname == surname && avgMark(s[m]) * 100 == avg * 100 && s[m].mark_last.mark_programic == last_mark) {
			delete[] s;
			return m;
		}
		int a = avgMark(s[m]) * 100;
		if (a > avg * 100 || a == avg * 100 && s[m].mark_last.mark_programic > last_mark || a == avg * 100 && s[m].mark_last.mark_programic == last_mark && s[m].surname > surname)
			l = m + 1;
		else
			r = m - 1;
	} while (l < r);

	delete[] s;
	return -1;
}

int main()
{
	srand(time(0));
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);


	size_t cnt = 0;
	Student* s = new Student[0];

	int choice;
	do {

		cout << "1 - Відобразити масив" << endl;
		cout << "2 - Фізичне впорядкування" << endl;
		cout << "3 - Індексне впорядкування" << endl;
		cout << "4 - Бінарний пошук" << endl;
		cout << "5 - Згенерувати файл" << endl;
		cout << "6 - Відображення індексного масива" << endl;
		cout << "0 - Вихід" << endl;
		cout << "choice: ";

		cin >> choice;
		switch (choice)
		{
		case 1:
		{
			string path;
			cout << "Введіть файл: ";
			cin.ignore();
			getline(cin, path);
			display(path, true);
			break;
		}
		case 2:
		{
			string path;
			cout << "Введіть файл: ";
			cin.ignore();
			getline(cin, path);
			RealSort(path);
			break;
		}
		case 3: {
			string path, new_path;
			cout << "Введіть файл даних: ";
			cin.ignore();
			getline(cin, path);
			cout << "Введіть файл індексний: ";
			cin.sync();
			getline(cin, new_path);
			IndexSort(path, new_path);
			break;
		}
		case 4:
		{
			string path;
			cout << "Введіть файл: ";
			cin.get();
			getline(cin, path);
			string surname;
			double avg, last;
			cout << "прізвище: "; cin >> surname;
			cout << "середню оцінку: "; cin >> avg;
			cout << "остання оцінка: "; cin >> last;
			int index = BinSearch(path, surname, avg, last);
			if (index == -1)
				cout << "Нема" << endl;
			else
				cout << "Є " << index << endl;
			break;
		}
		case 5:
		{
			string path;
			cout << "Введіть файл: ";
			cin.get();
			getline(cin, path);
			size_t cnt;
			cout << "Введіть кількість: "; cin >> cnt;
			generate_to_file(path, cnt);
			break;
		}
		case 6:
		{
			string data_path, index_path;
			cout << "Введіть файл даних: ";
			cin.ignore();
			getline(cin, data_path);
			cout << "Введіть файл індексний: ";
			cin.sync();
			getline(cin, index_path);
			displayIndex(data_path, index_path, true);
			break;
		}
		}
	} while (choice != 0);

	delete[] s;

	return 0;
}