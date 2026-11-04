// CTCInventoryMagic_00c15c70::Update() -- EH-framed: under a scoped lock over the global @ 0x0143BB14, the out-of-line step @ 0x00C2D490 on the object at +8
// Vendor naming V2 (plan phase 1, header-first): PKCProtectionInstance / PKCCriticalSection come from the shared libpk/PKCProtectionInstance.h (was a local copy).
// Vendor naming wave V1 (one address, one name): 0x00C2D490 ?Update@CInstanceManager@@QAEXXZ (was ?Step@CTarget_00c15c70@@QAEXXZ)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C15C70, oracle length 90 bytes. Built under /EHsc.
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; sub esp,8 ; push esi ; mov esi,ecx ; push offset g_lock ; lea ecx,[esp+8] ; call PKCProtectionInstance::PKCProtectionInstance ; mov ecx,[esi+8] ; mov dword ptr [esp+14h],0 ; call CTarget::Step ; lea ecx,[esp+4] ; mov dword ptr [esp+14h],-1 ; call ~PKCProtectionInstance ; mov ecx,[esp+0Ch] ; pop esi ; mov fs:[0],ecx ; add esp,14h ; ret
// The scoped-lock shape of `2026-10-14_linux_i1117` with the body a call on the member pointer at
// +8. The manifest label is propagated; the class is named by address.
#include "libpk/PKCProtectionInstance.h"
extern PKCCriticalSection g_lock_0143bb14;        // 0x0143BB14


struct CInstanceManager {
    void Update();                           // 0x00C2D490
};

struct CTCInventoryMagic_00c15c70 {
    unsigned char _pad[8];
    CInstanceManager* m_pTarget;           // +8
    void Update();
};

void CTCInventoryMagic_00c15c70::Update()
{
    PKCProtectionInstance lock(&g_lock_0143bb14);
    m_pTarget->Update();
}