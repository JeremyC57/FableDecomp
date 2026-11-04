// PKCMailbox::GetCount() -- EH-framed: under a scoped lock over `this`, virtual slot 2 of the object at +0x18, its result returned
// Vendor naming V2 (plan phase 1, header-first): PKCProtectionInstance / PKCCriticalSection come from the shared libpk/PKCProtectionInstance.h (was a local copy).
// Vendor naming wave V1 (one address, one name): 0x00C4BE10 ?GetCount@PKCMailbox@@QAEJXZ (was ?Query@CV_StatueMasterScript_00c4be10@@QAEJXZ)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C4BE10, oracle length 90 bytes. Built under /EHsc.
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; sub esp,8 ; push esi ; mov esi,ecx ; push esi ; lea ecx,[esp+8] ; call PKCProtectionInstance::PKCProtectionInstance ; mov ecx,[esi+18h] ; mov eax,[ecx] ; mov dword ptr [esp+14h],0 ; call [eax+8] ; lea ecx,[esp+4] ; mov esi,eax ; mov dword ptr [esp+14h],-1 ; call ~PKCProtectionInstance ; mov ecx,[esp+0Ch] ; mov eax,esi ; pop esi ; mov fs:[0],ecx ; add esp,14h ; ret
// The scoped-lock shape with `this` as the lock target and a virtual call on the member pointer
// at +0x18 whose result survives the lock's destructor in esi. The manifest label is propagated
// garbage; the class is named by address.
#include "libpk/PKCProtectionInstance.h"

struct CTarget_00c4be10 {
    virtual void v00(); virtual void v01();
    virtual long GetCount();                  // slot 2
};

struct PKCMailbox {
    unsigned char _pad[0x18];
    CTarget_00c4be10* m_pTarget;                // +0x18
    long GetCount();
};

long PKCMailbox::GetCount()
{
    PKCProtectionInstance lock((PKCCriticalSection*)this);
    return m_pTarget->GetCount();
}