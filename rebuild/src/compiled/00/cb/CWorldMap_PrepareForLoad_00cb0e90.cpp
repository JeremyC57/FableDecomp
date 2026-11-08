// Vendor-proof: strmbase-dxsdk-sum2004(BaseClasses/wxutil.cpp) ?GetRequest@CAMThread@@QAEKXZ -- 20 bytes verified against Fable.exe 0x00CB0E90
#include "rebuild_abi.h"
// CWorldMap::PrepareForLoad @ 0x00cb0e90
// push esi;esi=ecx; eax=[esi+4]; push -1; push eax; call [import]; eax=[esi+0xc]; ret
extern "C" __declspec(dllimport) unsigned long FABLE_STDCALL WaitForSingleObject(void *a, unsigned long b);   // IAT 0x0143FEAC = KERNEL32.dll!WaitForSingleObject
class CWorldMap { public: char pad4[4]; void *m_h; char pad[4]; int m_result; int PrepareForLoad(); };
int CWorldMap::PrepareForLoad(){ WaitForSingleObject(m_h, (unsigned long)-1); return m_result; }