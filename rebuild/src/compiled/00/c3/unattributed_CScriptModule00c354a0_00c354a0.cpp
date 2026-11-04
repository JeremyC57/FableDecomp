// CInputObject_00c48290::~CInputObject_00c48290() -- EH-framed destructor: the member at +8 cleared out of line (@ 0x00C2A090), an import called with 0x14, the handle at +4 released through an import and nulled, then the member at +8 destroyed (@ 0x00C29FB0)
// Vendor naming V2 (plan phase 1, header-first): PKCSemaphore comes from the shared libpk/PKCSemaphore.h (was a local copy).
// Vendor naming wave V1 (one address, one name): 0x00C29FB0 ??1PKCSemaphore@@QAE@XZ (was ??1CMember_00c354a0@@QAE@XZ); 0x00C2A090 ?Wait@PKCSemaphore@@QAEXXZ (was ?Clear@CMember_00c354a0@@QAEXXZ); 0x00C354A0 ??1CInputObject_00c48290@@QAE@XZ (was ??1CScriptModule_00c354a0@@QAE@XZ)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C354A0, oracle length 105 bytes. Built under /EHsc.
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; push ecx ; push esi ; mov esi,ecx ; push edi ; mov [esp+8],esi ; lea edi,[esi+8] ; mov ecx,edi ; mov dword ptr [esp+14h],0 ; call CMember::Clear ; push 14h ; call [__imp__Sleep@4] ; mov eax,[esi+4] ; push eax ; call [__imp__CloseHandle@4] ; mov ecx,edi ; mov dword ptr [esi+4],0 ; mov dword ptr [esp+14h],-1 ; call ~CMember ; mov ecx,[esp+0Ch] ; pop edi ; pop esi ; mov fs:[0],ecx ; add esp,10h ; ret
// The member's address is kept in edi for both its calls; the imports are `__stdcall`. EH state 0
// covers the body (the member is alive). The class stores no vtable. The manifest label is
// propagated garbage; the class is named by address.
extern "C" __declspec(dllimport) void __stdcall Sleep(unsigned long milliseconds); // IAT 0x0143FE1C = KERNEL32.dll!Sleep
extern "C" __declspec(dllimport) int __stdcall CloseHandle(void* handle);          // IAT 0x0143FE30 = KERNEL32.dll!CloseHandle

#include "libpk/PKCSemaphore.h"

struct CInputObject_00c48290 {
    unsigned char _pad[4];
    void* m_handle;                        // +4
    PKCSemaphore m_member;             // +8
    ~CInputObject_00c48290();
};

CInputObject_00c48290::~CInputObject_00c48290()
{
    m_member.Wait();
    Sleep(0x14);
    CloseHandle(m_handle);
    m_handle = 0;
}