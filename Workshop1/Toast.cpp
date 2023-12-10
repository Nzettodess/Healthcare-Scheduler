#include "Toast.h"
using namespace WinToastLib;

class WinToastHandler : public IWinToastHandler {
public:
	void toastActivated() const {
		//std::wcout << L"The user clicked in this toast" << std::endl;
		//exit(0);
	}

	void toastActivated(int actionIndex) const {
		//std::wcout << L"The user clicked on action #" << actionIndex << std::endl;
		//exit(16 + actionIndex);
	}

	void toastDismissed(WinToastDismissalReason state) const {
		switch (state) {
		case UserCanceled:
			//std::wcout << L"The user dismissed this toast" << std::endl;
			//exit(1);
			break;
		case TimedOut:
			//std::wcout << L"The toast has timed out" << std::endl;
			//exit(2);
			break;
		case ApplicationHidden:
			//std::wcout << L"The application hid the toast using ToastNotifier.hide()" << std::endl;
			//exit(3);
			break;
		default:
			//std::wcout << L"Toast not activated" << std::endl;
			//exit(4);
			break;
		}
	}

	void toastFailed() const {
		//std::wcout << L"Error showing current toast" << std::endl;
		//exit(5);
	}
};

enum Results {
	ToastClicked,             // user clicked on the toast
	ToastDismissed,           // user dismissed the toast
	ToastTimeOut,             // toast timed out
	ToastHided,               // application hid the toast
	ToastNotActivated,        // toast was not activated
	ToastFailed,              // toast failed
	SystemNotSupported,       // system does not support toasts
	UnhandledOption,          // unhandled option
	MultipleTextNotSupported, // multiple texts were provided
	InitializationFailure,    // toast notification manager initialization failure
	ToastNotLaunched          // toast could not be launched
};

std::wstring Toast::stringToWstring(const std::string& str) {
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	return converter.from_bytes(str);
}

void Toast::toastingmsg(wstring messaget1, wstring messaget2) {
	if (!WinToast::isCompatible()) {
		std::wcerr << L"Error, your system in not supported!" << std::endl;
		//return Results::SystemNotSupported;
	}


	std::wstring appName = L"Healthcare Appointment Scheduler";
	std::wstring appUserModelID = L"Healthcare Appointment Scheduler";

	std::wstring textbold = messaget1;

	std::wstring textnormal = messaget2;
	std::wstring imagePath = L"C:/Users/Nzettodess/OneDrive - Universiti Teknikal Malaysia Melaka/Pictures/Screenshots/Screenshot (133).jpg";
	std::wstring attribute = L"via Nzettodess";
	std::vector<std::wstring> actions;
	actions.push_back(L"Okay!!");
	INT64 expiration = 0;

	WinToastTemplate::AudioOption audioOption = WinToastTemplate::AudioOption::Default;

	WinToast::instance()->setAppName(appName);
	WinToast::instance()->setAppUserModelId(appUserModelID);


	if (!WinToast::instance()->initialize()) {
		std::wcerr << L"Error, your system in not compatible!" << std::endl;
		//return Results::InitializationFailure;
	}

	WinToastTemplate templ(!imagePath.empty() ? WinToastTemplate::ImageAndText02 : WinToastTemplate::Text02);
	templ.setTextField(textbold, WinToastTemplate::FirstLine);
	templ.setTextField(textnormal, WinToastTemplate::SecondLine);
	templ.setAudioOption(audioOption);
	templ.setAttributionText(attribute);
	templ.setImagePath(imagePath);

	for (auto const& action : actions) {
		templ.addAction(action);
	}

	if (expiration) {
		templ.setExpiration(expiration);
	}

	if (WinToast::instance()->showToast(templ, new WinToastHandler()) < 0) {
		std::wcerr << L"Could not launch your toast notification!";
		//return Results::ToastFailed;
	}
	//Timer::getInstance().stopTimer();

}

void Toast::toastingbackup(wstring messaget1, wstring messaget2) {
	if (!WinToast::isCompatible()) {
		std::wcerr << L"Error, your system in not supported!" << std::endl;
		//return Results::SystemNotSupported;
	}



	std::wstring appName = L"Healthcare Appointment Scheduler";
	std::wstring appUserModelID = L"WinToast Console Example";
	/*std::wstring textbold = L"hello";*/
	std::wstring textbold = messaget1;
	/*std::wstring textnormal = L"hello";*/
	std::wstring textnormal = messaget2;
	std::wstring imagePath = L"C:/Users/Nzettodess/OneDrive - Universiti Teknikal Malaysia Melaka/Documents/Workshop1/Workshop1/Screenshot(130).png";
	std::wstring attribute = L"via Nzettodess";
	std::vector<std::wstring> actions;
	actions.push_back(L"Okay!!");
	INT64 expiration = 0;

	WinToastTemplate::AudioOption audioOption = WinToastTemplate::AudioOption::Default;

	WinToast::instance()->setAppName(appName);
	WinToast::instance()->setAppUserModelId(appUserModelID);


	if (!WinToast::instance()->initialize()) {
		std::wcerr << L"Error, your system in not compatible!" << std::endl;
		//return Results::InitializationFailure;
	}

	WinToastTemplate templ(!imagePath.empty() ? WinToastTemplate::ImageAndText02 : WinToastTemplate::Text02);
	templ.setTextField(textbold, WinToastTemplate::FirstLine);
	templ.setTextField(textnormal, WinToastTemplate::SecondLine);
	templ.setAudioOption(audioOption);
	templ.setAttributionText(attribute);
	templ.setImagePath(imagePath);

	for (auto const& action : actions) {
		templ.addAction(action);
	}

	if (expiration) {
		templ.setExpiration(expiration);
	}

	if (WinToast::instance()->showToast(templ, new WinToastHandler()) < 0) {
		std::wcerr << L"Could not launch your toast notification!";
		//return Results::ToastFailed;
	}
}