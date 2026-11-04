// CNameTable_00c0ac30::SetName(unsigned long, const char*) -- EH-framed: under a scoped lock over the global @ 0x0143BB14, when the index is below 8 a temporary string is built from the text (@ 0x00C0BFE0, homed in the dead index slot), assigned to the string at 0x3a4 + 4*index (@ 0x00C0C0C0, state 1 while the temporary lives) and released (@ 0x00C0BA40)
// Vendor naming V2 (plan phase 1, header-first): PKCProtectionInstance / PKCCriticalSection come from the shared libpk/PKCProtectionInstance.h (was a local copy).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C0AC30, oracle length 141 bytes. Built under /EHsc.
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; sub esp,8 ; push esi ; push edi ; mov esi,ecx ; push offset g_lock ; lea ecx,[esp+0Ch] ; call PKCProtectionInstance::PKCProtectionInstance ; mov edi,[index] ; cmp edi,8 ; mov dword ptr [esp+18h],0 ; jae done ; mov eax,[text] ; push eax ; lea ecx,[index] ; call CCharString::CCharString ; push eax ; lea ecx,[esi+edi*4+0x3a4] ; mov byte ptr [esp+1Ch],1 ; call CCharString::operator= ; lea ecx,[index] ; mov byte ptr [esp+18h],0 ; call ~CCharString ; done: lea ecx,[esp+8] ; mov dword ptr [esp+18h],-1 ; call ~PKCProtectionInstance ; ...
// The string temporary of the assignment is built in the argument slot the index no longer
// needs. The manifest label is propagated; the class is named by address.
#include "libpk/PKCProtectionInstance.h"
extern PKCCriticalSection g_lock_0143bb14;        // 0x0143BB14


class CString { public:                 // one word: the shared string data
    void* m_pData;
    CString(const char* text);   // 0x00C0BFE0
    const CString& operator=(const CString& other);   // 0x00C0C0C0
    ~CString();                  // 0x00C0BA40
};

struct CNameTable_00c0ac30 {
    unsigned char _pad[0x3a4];
    CString m_names[8];         // 0x3a4
    void SetName(unsigned long index, const char* text);
};

void CNameTable_00c0ac30::SetName(unsigned long index, const char* text)
{
    PKCProtectionInstance lock(&g_lock_0143bb14);
    if (index < 8)
        m_names[index] = CString(text);
}