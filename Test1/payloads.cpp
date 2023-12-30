#include "payloads.h"

HCRYPTPROV prov;
DWORD xs;

int random() {
	if (prov == NULL)
		if (!CryptAcquireContext(&prov, NULL, NULL, PROV_RSA_FULL, CRYPT_SILENT | CRYPT_VERIFYCONTEXT))
			ExitProcess(1);

	int out;
	CryptGenRandom(prov, sizeof(out), (BYTE*)(&out));
	return out & 0x7fffffff;
}

void SeedXorshift32(DWORD dwSeed) {
	xs = dwSeed;
}

DWORD Xorshift32() {
	xs ^= xs << 13;
	xs ^= xs >> 17;
	xs ^= xs << 5;
	return xs;
}



void ExecutePayload(TS_PAYLOAD payload, int nTime) {
	int dwStartTime = Time;
	for (int i = 0; Time < (dwStartTime + nTime); i++) {
		HDC hdcScreen = GetDC(NULL);
		payload(i, hdcScreen);
		ReleaseDC(NULL, hdcScreen);
		DeleteObject(hdcScreen);
	}
	RedrawWindow(NULL, NULL, NULL, RDW_ERASE | RDW_INVALIDATE | RDW_ALLCHILDREN);
	Sleep(100);
}

void Payload1(int t, HDC hdcScreen) {
	POINT ptScreen = GetVirtualScreenPos();
	SIZE szScreen = GetVirtualScreenSize();
	SelectObject(hdcScreen, CreateSolidBrush(RGB(t % 256, (t / 2) % 256, (t / 2) % 256)));
	PatBlt(hdcScreen, ptScreen.x, ptScreen.y, szScreen.cx, szScreen.cy, DSTINVERT);
	Sleep(15);
}

void Payload2(int t, HDC hdcScreen) {
	int w = GetSystemMetrics(0);
	int h = GetSystemMetrics(1);
	HDC hdc = hdcScreen;
	HBRUSH brush = CreateSolidBrush(RGB(rand() % 255, rand() % 255, rand() % 255));
	SelectObject(hdc, brush);
	BitBlt(hdc, 0, 0, w, h, hdc, -30, 0, 0x1900ac010e);
	BitBlt(hdc, 0, 0, w, h, hdc, w - 30, 0, 0x1900ac010e);
	DeleteObject(brush);
}

void Payload3(int t, HDC hdcScreen) {
	int sw = GetSystemMetrics(0);
	int sh = GetSystemMetrics(1);
	HDC desk = hdcScreen;
	StretchBlt(desk, 0, -20, sw, sh + 40, desk, 0, 0, sw, sh, SRCCOPY);
	ReleaseDC(0, desk);
	Sleep(4);
}

void Payload4(int t, HDC hdc) {
	int x = GetSystemMetrics(0);
	int y = GetSystemMetrics(1);
	StretchBlt(hdc, -10, -10, x + 20, y + 20, hdc, 0, 0, x, y, SRCCOPY);
	StretchBlt(hdc, 10, 10, x - 20, y - 20, hdc, 0, 0, x, y, SRCCOPY);
	ReleaseDC(0, hdc);
}

int RotateDC(HDC hdc, int Angle, POINT ptCenter) {
	int nGraphicsMode = SetGraphicsMode(hdc, GM_ADVANCED);
	XFORM xform;
	if (Angle != 0)
	{
		double fangle = (double)Angle / 180. * 3.1415926;
		xform.eM11 = (float)cos(fangle);
		xform.eM12 = (float)sin(fangle);
		xform.eM21 = (float)-sin(fangle);
		xform.eM22 = (float)cos(fangle);
		xform.eDx = (float)(ptCenter.x - cos(fangle) * ptCenter.x + sin(fangle) * ptCenter.y);
		xform.eDy = (float)(ptCenter.y - cos(fangle) * ptCenter.y - sin(fangle) * ptCenter.x);
		SetWorldTransform(hdc, &xform);
	}
	return nGraphicsMode;
}

void Payload5(int t, HDC hdcScreen) {
	// Skidded from Hydrogen.exe (entire payload :troll:)
	POINT ptScreen = GetVirtualScreenPos();
	SIZE szScreen = GetVirtualScreenSize();

	HDC hcdc = CreateCompatibleDC(hdcScreen);
	HBITMAP hBitmap = CreateCompatibleBitmap(hdcScreen, szScreen.cx, szScreen.cy);
	SelectObject(hcdc, hBitmap);
	BitBlt(hcdc, 0, 0, szScreen.cx, szScreen.cy, hdcScreen, 0, 0, SRCCOPY);

	POINT p = { 0 };
	p.x = (szScreen.cx / 2);
	p.y = (szScreen.cy / 2);

	BLENDFUNCTION blf = { 0 };
	blf.BlendOp = AC_SRC_OVER;
	blf.BlendFlags = 0;
	blf.SourceConstantAlpha = 128;
	blf.AlphaFormat = 0;

	RotateDC(hdcScreen, 10, p);
	AlphaBlend(hdcScreen, 0, t, szScreen.cx, szScreen.cy, hcdc, 0, 0, szScreen.cx, szScreen.cy, blf);

	DeleteObject(hcdc);
	DeleteObject(hBitmap);


	SelectObject(hcdc, hBitmap);
	BitBlt(hcdc, 0, 0, szScreen.cx, szScreen.cy, hdcScreen, 0, 0, SRCCOPY);

	POINT xp = { 0 };
	xp.x = (szScreen.cx / 2);
	xp.y = (szScreen.cy / 2);

	BLENDFUNCTION xblf = { 0 };
	xblf.BlendOp = AC_SRC_OVER;
	xblf.BlendFlags = 0;
	xblf.SourceConstantAlpha = 128;
	xblf.AlphaFormat = 0;

	SetBkColor(hdcScreen, RGB(random() % 256, random() % 256, random() % 256));
	SetTextColor(hdcScreen, RGB(random() % 256, random() % 256, random() % 256));

	TextOut(hdcScreen, random() % szScreen.cx, random() % szScreen.cy, L"CALCIUM.EXE", 8);

	AlphaBlend(hdcScreen, 0, 0, szScreen.cx, szScreen.cy, hcdc, 0, 0, szScreen.cx, szScreen.cy, blf);

	DeleteObject(hcdc);
	DeleteObject(hBitmap);
}

void Payload6(int t, HDC hdcScreen) { // Another Skidded Payload cuz im too bad sorry :(
	POINT ptScreen = GetVirtualScreenPos();
	SIZE szScreen = GetVirtualScreenSize();

	t *= 30;

	RedrawWindow(NULL, NULL, NULL, RDW_ERASE | RDW_INVALIDATE | RDW_ALLCHILDREN);

	HDC hcdc = CreateCompatibleDC(hdcScreen);
	HBITMAP hBitmap = CreateCompatibleBitmap(hdcScreen, szScreen.cx, szScreen.cy);
	SelectObject(hcdc, hBitmap);
	Sleep(50);
	BitBlt(hcdc, 0, 0, szScreen.cx, szScreen.cy, hdcScreen, 0, 0, NOTSRCCOPY);
	SelectObject(hdcScreen, CreatePatternBrush(hBitmap));

	Ellipse(hdcScreen, t % szScreen.cx + 20, t % szScreen.cy + 20, t % szScreen.cx + t % 101 + 180, t % szScreen.cy + t % 101 + 180);
	BitBlt(hcdc, 0, 0, szScreen.cx, szScreen.cy, hdcScreen, 0, 0, NOTSRCCOPY);
	SelectObject(hdcScreen, CreatePatternBrush(hBitmap));
	Ellipse(hdcScreen, t % szScreen.cx + 10, t % szScreen.cy + 10, t % szScreen.cx + t % 101 + 190, t % szScreen.cy + t % 101 + 190);
	Ellipse(hdcScreen, t % szScreen.cx, t % szScreen.cy, t % szScreen.cx + t % 101 + 200, t % szScreen.cy + t % 101 + 200);
	BitBlt(hcdc, 0, 0, szScreen.cx, szScreen.cy, hdcScreen, 0, 0, NOTSRCCOPY);
	SelectObject(hdcScreen, CreatePatternBrush(hBitmap));
	Ellipse(hdcScreen, t % szScreen.cx, t % szScreen.cy, t % szScreen.cx + t % 101 + 200, t % szScreen.cy + t % 101 + 200);

	SetBkColor(hdcScreen, RGB(random() % 256, random() % 256, random() % 256));
	SetTextColor(hdcScreen, RGB(random() % 256, random() % 256, random() % 256));

	for (int i = 0; i < 5; i++) {
		TextOut(hdcScreen, random() % szScreen.cx, random() % szScreen.cy, L"     ", 5);
	}

	DeleteObject(hcdc);
	DeleteObject(hBitmap);
}

void ExecuteAudioSequence(int nSamplesPerSec, int nSampleCount, AUDIO_SEQUENCE pAudioSequence) {
	HANDLE hHeap = GetProcessHeap();
	PSHORT psSamples = (PSHORT)HeapAlloc(hHeap, 0, nSampleCount * 2);
	WAVEFORMATEX waveFormat = { WAVE_FORMAT_PCM, 1, nSamplesPerSec, nSamplesPerSec * 2, 2, 16, 0 };
	WAVEHDR waveHdr = { (PCHAR)psSamples, nSampleCount * 2, 0, 0, 0, 0, NULL, 0 };
	HWAVEOUT hWaveOut;
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &waveFormat, 0, 0, 0);

	pAudioSequence(nSamplesPerSec, nSampleCount, psSamples);

	waveOutPrepareHeader(hWaveOut, &waveHdr, sizeof(waveHdr));
	waveOutWrite(hWaveOut, &waveHdr, sizeof(waveHdr));

	Sleep(nSampleCount * 1000 / nSamplesPerSec);

	while (!(waveHdr.dwFlags & WHDR_DONE)) {
		Sleep(1);
	}

	waveOutReset(hWaveOut);
	waveOutUnprepareHeader(hWaveOut, &waveHdr, sizeof(waveHdr));
	HeapFree(hHeap, 0, psSamples);
}

void ExecuteAudioSequenceParams(PAUDIO_SEQUENCE_PARAMS pAudioParams) {
	ExecuteAudioSequence(pAudioParams->nSamplesPerSec, pAudioParams->nSampleCount, (AUDIO_SEQUENCE*)pAudioParams->pAudioSequence);
}

void AudioPayloadThread(AUDIO_SEQUENCE_PARAMS pAudioSequences[AUDIO_NUM]) {
	for (;;) {
		for (int i = 0; i < AUDIO_NUM; i++) {
			ExecuteAudioSequenceParams(&pAudioSequences[i]);
		}
	}
}

void AudioSequence1(int nSamplesPerSec, int nSampleCount, PSHORT psSamples) {
	int a = 8; // 8
	int b = 12; // 13
	int c = 8; // 8
	int d = 12; // 13
	int e = 8; // 8
	for (INT t = 0; t < nSampleCount * 2; t++) {
		BYTE bFreq = (BYTE)((((t & t >> a) - (t >> b & t)) & ((t & t >> c) - (t >> d))) ^ (t >> e & t));
		((BYTE*)psSamples)[t] = bFreq;
	}
}

void AudioSequence2(
	int nSamplesPerSec,
	int nSampleCount,
	PSHORT psSamples
) {
	// Credits to u.exe
	int a = 12; // 12
	int b = 8; // 8
	int c = 63; // 63
	int d = 4; // 4
	for (INT t = 0; t < nSampleCount * 2; t++) {
		BYTE bFreq = (BYTE)(t * ((t >> a | t >> b) & 63 & c >> d));
		((BYTE*)psSamples)[t] = bFreq;
	}
}

void AudioSequence3(
	int nSamplesPerSec,
	int nSampleCount,
	PSHORT psSamples
) {
	for (INT t = 0; t < nSampleCount * 2; t++) {
		BYTE bFreq = (BYTE)(100 * ((t << 2 | t >> 5 | t ^ 63) & (t << 10 | t >> 11)));
		((BYTE*)psSamples)[t] = bFreq;
	}
}

void AudioSequence4(int nSamplesPerSec, int nSampleCount, PSHORT psSamples) {
	for (INT t = 0; t < nSampleCount * 2; t++) {
		BYTE bFreq = (BYTE)(10 * (t >> 7 | 3 * t | t >> (t >> 15)) + (t >> 8 & 5));
		((BYTE*)psSamples)[t] = bFreq;
	}
}