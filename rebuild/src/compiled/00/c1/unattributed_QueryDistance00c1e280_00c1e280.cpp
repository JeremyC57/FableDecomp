// CPathFinding_00c1e280::QueryDistance() -- EH-framed: under a scoped lock over the global @ 0x0143BB14, the sum of three out-of-line queries on the object at +4: its member at +8 (@ 0x00C2D920), then two of its own (@ 0x00C1E9F0, @ 0x00C1E9C0); accumulated in esi
// Vendor naming V2 (plan phase 1, header-first): PKCProtectionInstance / PKCCriticalSection come from the shared libpk/PKCProtectionInstance.h (was a local copy).
// Vendor naming wave V1 (one address, one name): 0x00C1E9F0 ?GetAlignedDuration@CSequenceClient00c1e9f0@@QAEJXZ (was ?Second@CTarget_00c1e280@@QAEJXZ); 0x00C1E9C0 ?GetSequencePhase@CSequenceClient00c1e9f0@@QAEJXZ (was ?First@CTarget_00c1e280@@QAEJXZ)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C1E280, oracle length 119 bytes. Built under /EHsc.
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; sub esp,8 ; push esi ; push edi ; mov edi,ecx ; push offset g_lock ; lea ecx,[esp+0Ch] ; call PKCProtectionInstance::PKCProtectionInstance ; mov ecx,[edi+4] ; add ecx,8 ; mov dword ptr [esp+18h],0 ; call CSub::Count ; mov ecx,[edi+4] ; mov esi,eax ; call CTarget::First ; mov ecx,[edi+4] ; add esi,eax ; call CTarget::Second ; lea ecx,[esp+8] ; add esi,eax ; mov dword ptr [esp+18h],-1 ; call ~PKCProtectionInstance ; mov ecx,[esp+10h] ; pop edi ; mov eax,esi ; pop esi ; mov fs:[0],ecx ; add esp,14h ; ret
// Left-to-right evaluation of the three-term sum, the object pointer reloaded for each call.
// The manifest label is propagated; the class is named by address.
#include "libpk/PKCProtectionInstance.h"
extern PKCCriticalSection g_lock_0143bb14;        // 0x0143BB14


struct CSub_00c1e280 { long Count(); };    // 0x00C2D920

struct CSequenceClient00c1e9f0 {
    unsigned char _pad[8];
    CSub_00c1e280 m_sub;                   // +8
    long GetSequencePhase();                          // 0x00C1E9F0
    long GetAlignedDuration();                         // 0x00C1E9C0
};

struct CPathFinding_00c1e280 {
    unsigned char _pad[4];
    CSequenceClient00c1e9f0* m_pTarget;           // +4
    long QueryDistance();
};

long CPathFinding_00c1e280::QueryDistance()
{
    PKCProtectionInstance lock(&g_lock_0143bb14);
    return m_pTarget->m_sub.Count() + m_pTarget->GetSequencePhase() + m_pTarget->GetAlignedDuration();
}