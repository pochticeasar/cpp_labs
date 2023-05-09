

#ifndef CPP1_PHONEBOOK_H
#define CPP1_PHONEBOOK_H
#include <string>
using namespace std;
struct phonebook
{
	string surname;
	string name;
	string secname;
	long long number;
	bool operator<(const phonebook &second) const;
	bool operator>(const phonebook &second) const;
};
#endif	  // CPP1_PHONEBOOK_H
