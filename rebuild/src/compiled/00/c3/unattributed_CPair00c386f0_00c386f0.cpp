// CPair_00c386f0::~CPair_00c386f0() -- EH-framed empty destructor: member at +0x1c (@ 0x00C29FB0) then base @ 0x00C1C820
// Vendor naming V2 (plan phase 1, header-first): PKCSemaphore comes from the shared libpk/PKCSemaphore.h (was a local copy).
// Vendor naming V2 (plan phase 1, header-first): PKCProtectionInstance / PKCCriticalSection come from the shared libpk/PKCProtectionInstance.h (was a local copy).
// Vendor naming wave V1 (one address, one name): 0x00C29FB0 ??1PKCSemaphore@@QAE@XZ (was ??1CMember_00c29fb0@@QAE@XZ); 0x00C1C820 ??1PKCCriticalSection@@QAE@XZ (was ??1CBase_00c1c820@@QAE@XZ)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C386F0, oracle length 76 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  (EH prologue) ; push ecx ; push esi ; mov esi,ecx ; mov [esp+4],esi ; lea ecx,[esi+0x1c] ; mov dword ptr [esp+10h],0 ; call ~member ; mov ecx,esi ; mov dword ptr [esp+10h],-1 ; call ~base ; mov ecx,[esp+8] ; pop esi ; mov fs:[0],ecx ; add esp,10h ; ret
// Built with C++ exceptions enabled (/EHsc). Non-polymorphic aggregate (no vtable stores): the
// member at +0x1c is destroyed out of line @ 0x00C29FB0 under EH state 0, then the base
// destructor @ 0x00C1C820 runs under state -1.
// The manifest label (`_Dest_val<...pair<CCharString,CScriptInfo>>`) names a map pair; classes are
// placeholders named by their destructor addresses. One of 9 bodies.
#include "libpk/PKCProtectionInstance.h"

#include "libpk/PKCSemaphore.h"

struct CPair_00c386f0 : PKCCriticalSection {
    unsigned char _pad[0x4];   // after the 0x18-byte PKCCriticalSection base (libpk/PKCCriticalSection.h)
    PKCSemaphore m_member;          // +0x1c
    ~CPair_00c386f0();
};

CPair_00c386f0::~CPair_00c386f0()
{
}