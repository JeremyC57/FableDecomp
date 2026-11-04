// CTarget_00c08a80::Update() -- EH-framed: under a scoped lock over the global @ 0x0143BB14, the object at +0x8 is updated out of line (@ 0x00C2D2D0) when present
// Vendor naming V2 (plan phase 1, header-first): PKCProtectionInstance / PKCCriticalSection come from the shared libpk/PKCProtectionInstance.h (was a local copy).
// Vendor naming wave V1 (one address, one name): 0x00C15AA0 ?Update@CTarget_00c08a80@@QAEXXZ (was ?FrameUpdate@CCreatureActionHeroJump_00c15aa0@@QAEXXZ)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C15AA0, oracle length 93 bytes. Built under /EHsc.
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; sub esp,8 ; push esi ; mov esi,ecx ; push offset g_lock ; lea ecx,[esp+8] ; call PKCProtectionInstance::PKCProtectionInstance ; mov ecx,[esi+0x8] ; test ecx,ecx ; mov dword ptr [esp+14h],0 ; pop esi ; je skip ; call CTarget::Update ; skip: lea ecx,[esp] ; mov dword ptr [esp+10h],-1 ; call ~PKCProtectionInstance ; mov ecx,[esp+8] ; mov fs:[0],ecx ; add esp,14h ; ret
// The scoped-lock shape of `2026-10-14_linux_i1117` with a null-checked call on the member
// pointer (esi is popped before the branch since `this` is no longer needed). The manifest label
// is propagated; the class is named by address.
#include "libpk/PKCProtectionInstance.h"
extern PKCCriticalSection g_lock_0143bb14;        // 0x0143BB14


struct CTarget_00c15aa0 {
    void Update();                         // 0x00C2D2D0
};

struct CTarget_00c08a80 {
    unsigned char _pad[0x8];
    CTarget_00c15aa0* m_pTarget;                // +0x8
    void Update();
};

void CTarget_00c08a80::Update()
{
    PKCProtectionInstance lock(&g_lock_0143bb14);
    if (m_pTarget)
        m_pTarget->Update();
}