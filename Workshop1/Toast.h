#include <string>
#include "wintoastlib.h"
#include <AtlBase.h>
#include <atlconv.h>
#include <conio.h>
#include <locale>
#include <codecvt>
#include <string>
#include "Timer.h"


using namespace std;
using namespace WinToastLib;

#ifndef Toast_H
#define Toast_H

class Toast
{
public:
	static void toastingmsg(wstring messaget1, wstring messaget2);
	static std::wstring stringToWstring(const std::string& str);
	void toastingbackup(wstring messaget1, wstring messaget2);

};
#endif

