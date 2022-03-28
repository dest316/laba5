#include <iostream>
#include <fstream>
#include <string>

using namespace std;

struct FIO
{
	string name;
	string surname;
	string patronymic;
	FIO(string name, string surname, string patronymic)
	{
		this->name = name;
		this->surname = surname;
		this->patronymic = patronymic;
	}
	FIO()
	{
		name = surname = patronymic = "";
	}
	FIO(string data)
	{
		string temp = "";
		for (int i = 0; i < data.size(); i++)
		{
			if (data[i] == '_')
			{
				if (surname.empty())
				{
					surname = temp;
					temp = "";
				}
				else if (name.empty())
				{
					name = temp;
					temp = "";
				}
			}
			else
			{
				temp += data[i];
			}
		}
		patronymic = temp;
	}
	bool operator==(const FIO& other)
	{
		return (this->name == other.name && this->surname == other.surname && this->patronymic == other.patronymic);
	}
	bool operator>(const FIO& other)
	{
		if (this->surname == other.surname && this->name == other.name)
			return (this->patronymic > other.patronymic);
		else if (this->surname == other.surname)
			return (this->name > other.name);
		else return (this->surname > other.surname);
	}
	bool operator<(const FIO& other)
	{
		return (!(*this > other || *this == other));
	}
};


struct Date
{
	int day = 0;
	int month = 0;
	int year = 0;
	Date()
	{
		day = 0; month = 0; year = 0;
	}
	Date(string data)
	{
		string temp = "";
		for (int i = 0; i < data.size(); i++)
		{
			if (data[i] == '/')
			{
				if (day == 0)
				{
					day = stoi(temp);
					temp = "";
				}
				else if (month == 0)
				{
					month = stoi(temp);
					temp = "";
				}
			}
			else
			{
				temp += data[i];
			}
		}
		year = stoi(temp);
	}
	bool operator==(const Date& other)
	{
		return (this->day == other.day && this->month == other.month && this->year == other.year);
	}
	bool operator>(const Date& other)
	{
		if (this->year == other.year && this->month == other.month)
			return (this->day > other.day);
		else if (this->year == other.year)
			return (this->month > other.month);
		else return (this->year > other.year);
	}
	bool operator<(const Date& other)
	{
		return (!(*this > other || *this == other));
	}
};

struct Student
{
	FIO fio;
	string major;
	string group_number;
	Date date_of_birthday;
	string phone_number;
	Student()
	{
		fio = FIO();
		major = group_number = phone_number = "";
		date_of_birthday = Date();
	}
	Student(string data)
	{
		string temp = "";
		for (int i = 0; i < data.size(); i++)
		{
			if (data[i] == ' ')
			{
				if (fio == FIO())
				{
					fio = FIO(temp);
					temp = "";
				}
				else if (major == "")
				{
					major = temp;
					temp = "";
				}
				else if (group_number == "")
				{
					group_number = temp;
					temp = "";
				}
				else if (date_of_birthday == Date())
				{
					date_of_birthday = Date(temp);
					temp = "";
				}
			}
			else
				temp += data[i];

		}
		phone_number = temp;
	}
	bool operator>(const Student& other)
	{
		if (this->fio > other.fio)
			return true;
		else if (this->fio == other.fio)
			if (this->date_of_birthday > other.date_of_birthday)
				return true;
		return false;
	}
	bool operator<(const Student& other)
	{
		if (this->fio < other.fio)
			return true;
		else if (this->fio == other.fio)
			if (this->date_of_birthday < other.date_of_birthday)
				return true;
		return false;
	}
	bool operator==(const Student& other)
	{
		return (!(*this > other && *this < other));
	}

};

ostream& operator<< (std::ostream& out, const FIO& fio)
{
	out << fio.surname << " " << fio.name << " " << fio.patronymic;
	return out;
}

ostream& operator<< (std::ostream& out, const Date& date)
{
	out << date.day << "." << date.month << "." << date.year;
	return out;
}

ostream& operator<< (std::ostream& out, const Student& student)
{
	out << student.fio << " " << student.major << " " << student.group_number << " " << student.date_of_birthday << " " << student.phone_number << endl;
	return out;
}


class InputFile {
private:
	fstream file;
	Student* students;
	int recordCount;
	int* CreateBadSymbolsTable(string pattern)
	{
		int patternLength = pattern.length();
		int* badSymbolsTable = new int[patternLength]; // не забыть очистить память
		for (int i = 0; i < patternLength; i++) { badSymbolsTable[i] = -1; }
		for (int i = patternLength - 2; i >= 0; i--)
		{
			if (badSymbolsTable[i] == -1)
			{
				badSymbolsTable[i] = patternLength - 1 - i;
				for (int j = i - 1; j >= 0; j--)
				{
					if (pattern[j] == pattern[i]) { badSymbolsTable[j] = badSymbolsTable[i]; }
				}
			}
		}
		for (int i = 0; i < patternLength - 2; i++)
		{
			if (pattern[patternLength - 1] == pattern[i])
			{
				badSymbolsTable[patternLength - 1] = badSymbolsTable[i];
				break;
			}
		}
		if (badSymbolsTable[patternLength - 1] == -1) { badSymbolsTable[patternLength - 1] = patternLength; }
		return badSymbolsTable;
	}
	int* CreateGoodSuffixTable(string pattern)
	{
		int patternLength = pattern.length();
		int* goodSuffixTable = new int[patternLength];
		for (int i = 0; i < patternLength; i++) { goodSuffixTable[i] = -1; }
		for (int i = patternLength - 1; i > 0; i--)
		{
			string subpattern = "";
			for (int j = i; j < patternLength; j++) { subpattern += pattern[j]; }
			for (int j = i - 1; j >= 0; j--)
			{
				string temp = "";
				for (int k = 0; k < subpattern.length(); k++) { temp += pattern[j + k]; }
				if (temp == subpattern)
				{
					goodSuffixTable[(patternLength - 1) - i] = i - j;
					break;
				}
			}
			if (goodSuffixTable[i] == -1) { goodSuffixTable[i] = patternLength - 1; }
		}
		return goodSuffixTable;
	}
public:
	InputFile(string pathToFile) {

		file.open(pathToFile, ios::in);
		string temp;
		if (file.is_open()) {
			getline(file, temp);			
			recordCount = stoi(temp);
			students = new Student[recordCount];
			for (int i = 0; i < recordCount; i++)
			{
				getline(file, temp);
				if (temp == "") continue;
				students[i] = Student(temp);
			}
		}
		file.close();
	}
	int GetHash(string str)
	{
		const int hashConst = 13;
		const int modConst = 1000;
		int result = 0;
		for (int i = 0; i < str.length(); i++)
		{
			result += (static_cast<int>(str[i])) * static_cast<int>(pow(hashConst, str.length() - (i + 1))) % modConst;
		}
		return result;
	}
	/// <summary>
	/// Функция проверяет, входит ли подстрока pattern в строку text
	/// </summary>
	/// <param name="pattern">шаблон, наличие которого проверяется</param>
	/// <param name="text">текст, в котором ищут шаблон</param>
	/// <returns>1, если входит, -1 если не входит, 0 если произошла непредвиденная ситуация</returns>
	int RabinKarpSearch(string pattern, string text)
	{
		if (pattern == "" || text == "") { return 0; }
		int patternLength = pattern.length();
		int patternHash = this->GetHash(pattern);
		for (int i = 0; i < text.length() - patternLength + 1; i++)
		{
			string temp = "";
			for (int j = 0; j < patternLength; j++) { temp += text[i + j]; }
			if (this->GetHash(temp) == patternHash)
			{
				bool equalityFlag = true;
				for (int j = 0; j < patternLength; j++)
				{
					if (pattern[j] != temp[j]) { equalityFlag = false; }
				}
				if (equalityFlag) { return 1; }
			}
		}
		return -1;
	}
	int TurboBMSearch(string pattern, string text)
	{
		if (pattern == "" || text == "") { return 0; }
		int* badSymbolsTable = CreateBadSymbolsTable(pattern);
		int* goodSuffixTable = CreateGoodSuffixTable(pattern); //буквально чуть-чуть надо разобраться, с последним символом. Чтобы не выводил -1.
		for (int i = 0; i < pattern.length(); i++) { cout << goodSuffixTable[i] << '\t'; }

		return 0; //заглушка
		
	}
};

int main()
{
	setlocale(LC_ALL, "ru");
	InputFile *file = new InputFile("input1.txt");
	file->TurboBMSearch("babbab", "b");
	
	return 0;

}


