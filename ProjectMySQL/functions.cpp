#include "functions.h"
#include <iostream>

using namespace std;

bool IntCheck(string option) {
	for (int i = 0; i < option.length(); i++)
	{
		if (isdigit(option[i]) == false)
		{
			return false;
		}
		else
			return true;
	}
}
bool IsCorrect(string username, string password) {
	int unlength = username.size();
	int pwlength = password.size();
	if (unlength > 8 && unlength < 20) {
		return true;
	}
	if (unlength > 8 && unlength < 20) {
		return true;
	}
	else return false;
}
bool IsFilled(string string) {
	int unstring = string.size();
	if (unstring > 0) {
		return true;
	}
	else return false;
}