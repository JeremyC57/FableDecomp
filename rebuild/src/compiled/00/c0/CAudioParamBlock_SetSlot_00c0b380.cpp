// CAudioParamBlock::SetSlot(unsigned long, void*, void*) -- EH-framed: under a scoped lock over the global @ 0x0143BB14, when the index is below 16 the 0x34-byte ramped parameter at +4 + 0x34*index is retargeted from the two arguments and 0 (CRampedParam::SetTarget @ 0x00C0B570), the display at +0x38 of the owner at +0x3c4 refreshed (@ 0x00C11180) and the optional sprite at +0x60 of the owner updated (@ 0x00C15B90)
// Leaf: ?SetSlot@CAudioParamBlock@@QAEXKPAX0@Z
// Vendor naming V2 (plan phase 1, header-first): PKCProtectionInstance / PKCCriticalSection come from the shared libpk/PKCProtectionInstance.h (was a local copy).
// Vendor naming wave V1 (one address, one name): 0x00C11180 ?SetFlagBit1@CLockedFlagOwner_00c11180@@QAEXXZ (was ?Refresh@CDisplay@@QAEXXZ)
// Naming (naming wave 21, DECISIONS_W20_RESOLVED): CAudioOwner -> CSoundSystem, the one class name for vtable 0x012B1E24 (the libpk sound system CreateSoundSystem_00c096e0 0x00C096E0 returns and the landed CSoundManagerLH ctor 0x00A3EF00 drives; descriptive, libpk has no PDB).
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C0B380, oracle length 150 bytes. Built under /EHsc.
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; sub esp,8 ; push esi ; mov esi,ecx ; push offset g_lock ; lea ecx,[esp+8] ; call PKCProtectionInstance::PKCProtectionInstance ; mov eax,[index] ; cmp eax,10h ; mov dword ptr [esp+14h],0 ; jae done ; mov ecx,[b] ; imul eax,eax,34h ; mov edx,[a] ; push ecx ; push 0 ; push edx ; lea ecx,[eax+esi+4] ; call CRampedParam::SetTarget ; mov eax,[esi+3C4h] ; mov ecx,[eax+38h] ; call CLockedFlagOwner_00c11180::Refresh ; mov ecx,[esi+3C4h] ; mov ecx,[ecx+60h] ; test ecx,ecx ; je done ; call CSprite::Update ; done: lea ecx,[esp+4] ; mov dword ptr [esp+14h],-1 ; call ~PKCProtectionInstance ; ...
// Naming (sweep1, 2026-10-25): was landed as _unattributed::CCombatAbilityDef_00c0b380::SetSlot (manifest
//   "Container_UpdateElement_WithMatrixTransform", propagated); descriptive (no PDB covers this library). `this` is the
//   libpk audio parameter block (chunk 431's CAudioParamBlock): this function is slot 0 of its vtable 0x012B1E84 (the
//   vtable the block constructor 0x00C0B200 and destructor 0x00C0A9A0 store), the sixteen 0x34-byte ramped parameters
//   sit at +4 and the owner at +0x3c4, exactly as in the landed CAudioParamBlock::Update 0x00C0AF20, whose CSoundSystem /
//   CLockedFlagOwner_00c11180 / CSprite / CRampedParam model (display +0x38, sprite +0x60, Refresh 0x00C11180, Update 0x00C15B90) this
//   source now shares. The landed CRampedParam::SetImmediate 0x00C0B510 names 0x00C0B570 SetTarget.
#include "libpk/PKCProtectionInstance.h"
extern PKCCriticalSection g_lock_0143bb14;        // 0x0143BB14


struct CRampedParam {                      // 0x34 bytes
    unsigned char _body[0x34];
    void SetTarget(void* a, long mode, void* b);   // 0x00C0B570
};

struct CLockedFlagOwner_00c11180 { void SetFlagBit1(); };       // 0x00C11180
struct CSprite { void Update(); };         // 0x00C15B90

struct CSoundSystem {
    unsigned char _pad[0x38];
    CLockedFlagOwner_00c11180* m_pDisplay;                  // +0x38
    unsigned char _pad2[0x24];
    CSprite* m_pSprite;                    // +0x60
};

struct CAudioParamBlock {
    unsigned char _pad[4];                 // +0 vptr (vtable 0x012B1E84; this function is slot 0)
    CRampedParam m_gains[16];              // +4
    unsigned char _pad2[0x80];             // +0x344 the eight pitch factors and the 16 + 8 parameter names
    CSoundSystem* m_pOwner;                 // +0x3c4
    void SetSlot(unsigned long index, void* a, void* b);
};

void CAudioParamBlock::SetSlot(unsigned long index, void* a, void* b)
{
    PKCProtectionInstance lock(&g_lock_0143bb14);
    if (index < 16) {
        m_gains[index].SetTarget(a, 0, b);
        m_pOwner->m_pDisplay->SetFlagBit1();
        if (m_pOwner->m_pSprite)
            m_pOwner->m_pSprite->Update();
    }
}