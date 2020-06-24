#include "stdafx.h"
#include "TabletFilter.h"

#include <Windows.h>

// From Oskar Dahlberg's post at:
// http://stackoverflow.com/a/31411628/4725495

static NTSTATUS(__stdcall *NtDelayExecution)(BOOL Alertable, PLARGE_INTEGER DelayInterval) = (NTSTATUS(__stdcall*)(BOOL, PLARGE_INTEGER)) GetProcAddress(GetModuleHandleW(L"ntdll"), "NtDelayExecution");

static NTSTATUS(__stdcall *ZwSetTimerResolution)(IN ULONG RequestedResolution, IN BOOLEAN Set, OUT PULONG ActualResolution) = (NTSTATUS(__stdcall*)(ULONG, BOOLEAN, PULONG)) GetProcAddress(GetModuleHandleW(L"ntdll"), "ZwSetTimerResolution");

static bool once = true;

void SleepShort(float milliseconds) {
    if (once) {
        ULONG actualResolution;
        ZwSetTimerResolution(1, true, &actualResolution); 
	once = false;
    }

    LARGE_INTEGER interval;
    interval.QuadPart = -1 * (int)(milliseconds * 10000.0f);
    NtDelayExecution(false, &interval);
}


TabletFilter::TabletFilter() {
	timerInterval = 2.f;
	isValid = false;
	isEnabled = false;
}

DWORD WINAPI TimerLoop(void *v) {
	TabletFilter *filter = (TabletFilter *)v;	
	while(1) {
		filter->callback();
		SleepShort(filter->timerInterval);
	}
 }

//
// Start Timer
//
bool TabletFilter::StartTimer() {
    thread = CreateThread(NULL, 0, TimerLoop, (void *)this, 0, NULL);
	return true;
}


//
// Stop Timer
//
bool TabletFilter::StopTimer() {
	bool result = TerminateThread(thread, 0);
	return result;
}