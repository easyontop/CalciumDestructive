#pragma once

#include <Windows.h>
#include <cmath>
#include "def.h"
#include "timer.h"
#include "virtual_screen.h"

#pragma  comment(lib,"WinMM.lib")
#pragma  comment(lib,"Msimg32.lib")

extern void SeedXorshift32(DWORD dwSeed);
extern DWORD Xorshift32();


extern void ExecutePayload(TS_PAYLOAD payload, int nTime);
extern void AudioPayloadThread(AUDIO_SEQUENCE_PARAMS pAudioSequences[AUDIO_NUM]);

// Payloads
extern void Payload1(int t, HDC hdcScreen);
extern void Payload2(int t, HDC hdcScreen);
extern void Payload3(int t, HDC hdcScreen);
extern void Payload4(int t, HDC hdcScreen);
extern void Payload5(int t, HDC hdcScreen);
extern void Payload6(int t, HDC hdcScreen);

// Audios
extern void AudioSequence1(int nSamplesPerSec, int nSampleCount, PSHORT psSamples);
extern void AudioSequence2(int nSamplesPerSec, int nSampleCount, PSHORT psSamples);
extern void AudioSequence3(int nSamplesPerSec, int nSampleCount, PSHORT psSamples);
extern void AudioSequence4(int nSamplesPerSec, int nSampleCounr, PSHORT psSamples);