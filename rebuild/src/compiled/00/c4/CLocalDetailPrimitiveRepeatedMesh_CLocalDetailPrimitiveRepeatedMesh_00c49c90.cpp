// CFrameStack::CFrameStack(long) -- EH-framed constructor: out-of-line base constructor (@ 0x00C1C810), the count stored at +0x18, a block of count*4 bytes allocated (@ 0x00BFEB22) into +0x1c, two members zeroed
// Vendor naming V2 (plan phase 1, header-first): PKCProtectionInstance / PKCCriticalSection come from the shared libpk/PKCProtectionInstance.h (was a local copy).
// Vendor naming wave V1 (one address, one name): 0x00C1C810 ??0PKCCriticalSection@@QAE@XZ (was ??0CLocalDetailPrimitiveBase_00c49c90@@QAE@XZ); 0x00C49C90 ??0CFrameStack@@QAE@J@Z (was ??0CLocalDetailPrimitiveRepeatedMesh@@QAE@J@Z)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C49C90, oracle length 98 bytes. Built under /EHsc.
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; push ecx ; push esi ; mov esi,ecx ; mov [esp+4],esi ; call CBase::CBase ; mov eax,[count] ; mov [esi+18h],eax ; shl eax,2 ; push eax ; mov dword ptr [esp+14h],0 ; call Allocate ; mov ecx,[esp+0Ch] ; mov [esi+1Ch],eax ; add esp,4 ; mov dword ptr [esi+20h],0 ; mov dword ptr [esi+24h],0 ; mov eax,esi ; pop esi ; mov fs:[0],ecx ; add esp,10h ; ret 4
// EH state 0 is entered once the base exists (its destructor is out of line); the body stores the
// count, allocates the index block through the `__cdecl` allocator and zeroes two members. The
// class comes from the manifest label; the base and allocator are descriptive.

void* __cdecl operator new[](unsigned int size);      // 0x00BFEB22 MSVCR71 ??_U@YAPAXI@Z (import thunk); VC7.1 predeclares only scalar new/delete

#include "libpk/PKCProtectionInstance.h"

struct CFrameStack : PKCCriticalSection {
    long m_count;                          // +0x18
    void* m_pIndices;                      // +0x1c
    long m_used;                           // +0x20
    void* m_pMesh;                         // +0x24
    CFrameStack(long count);
};

CFrameStack::CFrameStack(long count)
{
    m_count = count;
    m_pIndices = ::operator new[](count * 4);              // 0x00BFEB22 MSVCR71 operator new[] (import thunk)
    m_used = 0;
    m_pMesh = 0;
}