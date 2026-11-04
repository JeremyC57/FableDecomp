// CClothingSuitMap_00c19c10::Resolve(void*, void*) -- EH-framed: under a scoped lock over the global @ 0x0143BB14, -1 when the key is not found (@ 0x00C196A0), else the entry resolved out of line with the second argument (@ 0x00C198C0); the result survives the lock's destructor in esi
// Leaf: ?Resolve@CClothingSuitMap_00c19c10@@QAEJPAX0@Z
// Vendor naming V2 (plan phase 1, header-first): PKCProtectionInstance / PKCCriticalSection come from the shared libpk/PKCProtectionInstance.h (was a local copy).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C19C10, oracle length 153 bytes. Built under /EHsc.
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; sub esp,8 ; push esi ; mov esi,ecx ; push offset g_lock ; lea ecx,[esp+8] ; call PKCProtectionInstance::PKCProtectionInstance ; mov eax,[key] ; push eax ; mov ecx,esi ; mov dword ptr [esp+18h],0 ; call Find ; test eax,eax ; jne hit ; lea ecx,[esp+4] ; mov dword ptr [esp+14h],-1 ; call ~PKCProtectionInstance ; or eax,-1 ; pop esi ; ... ret 8 ; hit: mov ecx,[arg] ; push ecx ; push eax ; mov ecx,esi ; call ResolveEntry ; lea ecx,[esp+4] ; mov esi,eax ; mov dword ptr [esp+14h],-1 ; call ~PKCProtectionInstance ; mov ecx,[esp+0Ch] ; mov eax,esi ; pop esi ; ...
// The two-argument sibling of 0x00C19B70. The manifest label is propagated; the class is named
// by address.
#include "libpk/PKCProtectionInstance.h"
extern PKCCriticalSection g_lock_0143bb14;        // 0x0143BB14


struct CEntry_00c19c10;

struct CClothingSuitMap_00c19c10 {
    CEntry_00c19c10* Find(void* key);      // 0x00C196A0
    long ResolveEntry(CEntry_00c19c10* entry, void* argument);   // 0x00C198C0
    long Resolve(void* key, void* argument);
};

long CClothingSuitMap_00c19c10::Resolve(void* key, void* argument)
{
    PKCProtectionInstance lock(&g_lock_0143bb14);
    CEntry_00c19c10* entry = Find(key);
    if (!entry)
        return -1;
    return ResolveEntry(entry, argument);
}