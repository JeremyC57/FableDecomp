#ifndef FABLE_KERNEL_STREAMING_H
#define FABLE_KERNEL_STREAMING_H
#include "EAX40.h"
#include "CImaAdpcmCodec.h"
// Original public Windows KS, mmreg and Win32 definitions, confirmed by SDK COFF.
typedef void* HANDLE;
struct HINSTANCE__;typedef HINSTANCE__* HMODULE;
struct HWND__;typedef HWND__* HWND;
typedef struct _SECURITY_ATTRIBUTES {DWORD nLength;void* lpSecurityDescriptor;BOOL bInheritHandle;} SECURITY_ATTRIBUTES;
typedef struct _OVERLAPPED {
 DWORD Internal,InternalHigh;
 union {struct {DWORD Offset,OffsetHigh;};void* Pointer;};
 HANDLE hEvent;
} OVERLAPPED;
struct _RTL_CRITICAL_SECTION_DEBUG;
typedef struct _RTL_CRITICAL_SECTION {
 _RTL_CRITICAL_SECTION_DEBUG* DebugInfo;
 long LockCount,RecursionCount;
 HANDLE OwningThread,LockSemaphore;
 unsigned long SpinCount;
} CRITICAL_SECTION;
typedef struct { union {struct {GUID Set;DWORD Id;DWORD Flags;};__int64 Alignment;};} KSIDENTIFIER;
typedef struct {DWORD PriorityClass,PrioritySubClass;} KSPRIORITY;
typedef struct {KSIDENTIFIER Interface,Medium;DWORD PinId;HANDLE PinToHandle;KSPRIORITY Priority;} KSPIN_CONNECT;
typedef union {struct {DWORD FormatSize,Flags,SampleSize,Reserved;GUID MajorFormat,SubFormat,Specifier;};__int64 Alignment;} KSDATAFORMAT;
typedef struct tWAVEFORMATEXTENSIBLE {
 WAVEFORMATEX Format;
 union {WORD wValidBitsPerSample,wSamplesPerBlock,wReserved;} Samples;
 DWORD dwChannelMask;
 GUID SubFormat;
} WAVEFORMATEXTENSIBLE;
typedef struct {__int64 Time;DWORD Numerator,Denominator;} KSTIME;
typedef struct {DWORD Size,TypeSpecificFlags;KSTIME PresentationTime;__int64 Duration;DWORD FrameExtent,DataUsed;void* Data;DWORD OptionsFlags;} KSSTREAM_HEADER;
enum KSSTATE {KSSTATE_STOP,KSSTATE_ACQUIRE,KSSTATE_PAUSE,KSSTATE_RUN};
typedef int (__stdcall* FARPROC)();
typedef DWORD (__stdcall* LPTHREAD_START_ROUTINE)(void*);
extern "C" {
 __declspec(dllimport) HMODULE __stdcall LoadLibraryA(const char*);
 __declspec(dllimport) FARPROC __stdcall GetProcAddress(HMODULE,const char*);
 __declspec(dllimport) BOOL __stdcall FreeLibrary(HMODULE);
 __declspec(dllimport) HANDLE __stdcall CreateEventA(SECURITY_ATTRIBUTES*,BOOL,BOOL,const char*);
 __declspec(dllimport) BOOL __stdcall SetThreadPriority(HANDLE,int);
}
struct __declspec(uuid("1a8766a0-62ce-11cf-a5d6-28db04c10000")) KSINTERFACESETID_Standard;
struct __declspec(uuid("4747b320-62ce-11cf-a5d6-28db04c10000")) KSMEDIUMSETID_Standard;
struct __declspec(uuid("73647561-0000-0010-8000-00aa00389b71")) KSDATAFORMAT_TYPE_AUDIO;
struct __declspec(uuid("00000001-0000-0010-8000-00aa00389b71")) KSDATAFORMAT_SUBTYPE_PCM;
struct __declspec(uuid("05589f81-c356-11ce-bf01-00aa0055595a")) KSDATAFORMAT_SPECIFIER_WAVEFORMATEX;
#endif
