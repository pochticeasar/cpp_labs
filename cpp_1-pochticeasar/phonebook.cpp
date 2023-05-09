
#include "phonebook.h"
#include <string>
using namespace std;

bool phonebook::operator<(const phonebook &first) const
{
	if (this->surname < first.surname)
		return true;
	else if (this->surname > first.surname)
		return false;
	if (this->name < first.name)
		return true;
	else if (this->name > first.name)
		return false;
	if (this->secname < first.secname)
		return true;
	else if (this->secname > first.secname)
		return false;
	if (this->number < first.number)
		return true;
	else if (this->number > first.number)
		return false;
	return false;
}
bool phonebook::operator>(const phonebook &first) const
{
	if (this->surname > first.surname)
		return true;
	else if (this->surname < first.surname)
		return false;
	if (this->name > first.name)
		return true;
	else if (this->name < first.name)
		return false;
	if (this->secname > first.secname)
		return true;
	else if (this->secname < first.secname)
		return false;
	if (this->number > first.number)
		return true;
	else if (this->number < first.number)
		return false;
	return false;
}