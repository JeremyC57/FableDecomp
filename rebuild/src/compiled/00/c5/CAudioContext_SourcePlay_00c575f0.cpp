// CAudioContext::SourcePlay(unsigned long value) -- Resolve and cache the OpenAL entry point before forwarding the argument.
// Leaf: ?SourcePlay@CAudioContext@@QAEXK@Z
// Identity: own literal alSourcePlay and function slot0x6c; actual helper00c4d740 and kernel GetProcAddress0143fe44. OpenAL API identity proven, wrapper class/method spelling descriptive existing convention.
#ifndef _MSC_VER
#define __cdecl
#define __stdcall
#define __declspec(x)
#endif
extern "C" __declspec(dllimport) void* __stdcall GetProcAddress(void*,const char*);
typedef void (__cdecl* SourceFunction)(unsigned long);
typedef void (__cdecl* DistanceFunction)(long);
typedef const char* (__cdecl* StringFunction)(long);
// Layout: no shared header. Actual module+4, source callbacks6c/74/7c/84, stringd0/distanced4, resolver flag104. Original libpk class name unknown; established CAudioContext binding retained. Genuine virtual interface slot0 and untouched fields retained as unmodeled ranges.
struct CAudioContext{virtual void UnusedSlot0();void* Module;unsigned char Unmodeled08[100];SourceFunction pfnSourcePlay;unsigned char Unmodeled70[4];SourceFunction pfnSourcePause;unsigned char Unmodeled78[4];SourceFunction pfnSourceStop;unsigned char Unmodeled80[4];SourceFunction pfnSourceRewind;unsigned char Unmodeled88[72];StringFunction pfnGetString;DistanceFunction pfnDistanceModel;unsigned char UnmodeledD8[44];bool UseALGetProcAddress;void* GetProcAddress(const char*);void DistanceModel(long);const char* GetString(long);void SourceRewind(unsigned long);void SourcePlay(unsigned long);void SourceStop(unsigned long);void SourcePause(unsigned long);};
void CAudioContext::SourcePlay(unsigned long value){if(!pfnSourcePlay){if(UseALGetProcAddress)pfnSourcePlay=(SourceFunction)GetProcAddress("alSourcePlay");else pfnSourcePlay=(SourceFunction)::GetProcAddress(Module,"alSourcePlay");}return pfnSourcePlay(value);}