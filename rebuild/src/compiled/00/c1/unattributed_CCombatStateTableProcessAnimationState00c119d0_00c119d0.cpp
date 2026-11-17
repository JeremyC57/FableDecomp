// CCombatStateTable_00c119d0::ProcessAnimationState(tag, entry) -- EH-framed under the module's profiling lock (0x0143BB14; ctor @ 0x00C0FFC0, dtor @ 0x00C0FFE0, state 0): -1 without an entry; otherwise a 0x10-byte lookup key is built on the frame (ctor @ 0x00C25C50, state 1) from the entry's +4 word, the entry and the entry's +8 table resolved by the tag (@ 0x00C1BAC0; set into the key's value part at +4 @ 0x00C25C70), the state map embedded at +0x1f4 is searched (slot 0x44), the key is cleared (@ 0x00C25CB0) and destroyed (@ 0x00C25CA0), and the hit's +0xc state (read before the destructors) or -1 is returned
// Vendor naming V2 (plan phase 1, header-first): PKCProtectionInstance / PKCCriticalSection come from the shared libpk/PKCProtectionInstance.h (was a local copy).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C119D0, oracle length 238 bytes. Built under /EHsc.
// Retail bytes:  ... mov edi,ecx ; push offset g_section ; lea ecx,[esp+0Ch] ; call CProfileLock::CProfileLock ; mov esi,[esp+34h] ; test esi,esi ; mov dword ptr [esp+28h],0 ; je none ; lea ecx,[esp+10h] ; call CKey::CKey ; mov eax,[esp+30h] ; push eax ; lea ecx,[esi+8] ; mov byte ptr [esp+2Ch],1 ; call CTable::Lookup ; mov ecx,[esi+4] ; push eax ; push esi ; push ecx ; lea ecx,[esp+20h] ; call CKey::Set ; mov edx,[edi+1F4h] ; lea ecx,[edi+1F4h] ; lea eax,[esp+10h] ; push eax ; call [edx+44h] ; lea ecx,[esp+10h] ; mov esi,eax ; call CKey::Clear ; test esi,esi ; lea ecx,[esp+10h] ; mov byte ptr [esp+28h],0 ; je miss ; mov esi,[esi+0Ch] ; call CKey::~CKey ; lea ecx,[esp+8] ; mov dword ptr [esp+28h],-1 ; call ~CProfileLock ; pop edi ; mov eax,esi ; ... ret 8 ; miss: call CKey::~CKey ; none: lea ecx,[esp+8] ; mov dword ptr [esp+28h],-1 ; call ~CProfileLock ; ... or eax,-1 ; ... ret 8
// The manifest label is propagated; the classes are named by address.
#include "libpk/PKCProtectionInstance.h"
extern PKCCriticalSection g_lock_0143bb14;  // 0x0143BB14: the module's critical section


// NF53: the key is the shared libpk CSampleRef (libpk/CSampleRef.h: ctor 0x00C25C50 zeroes the four words, Bind 0x00C25C70 stores
//   sample/+4/+8/+0xc, Release 0x00C25CB0, dtor 0x00C25CA0); Bind takes four arguments with `this` = the key itself: the tag is
//   pushed first, then CSoundPlayParameters::GetCategory 0x00C1BAC0 (no stack argument, plain `ret`; the 0x00C12940 spelling)
//   runs and its result, the entry and the entry's sample follow. Was the CKey/CKeyFields/CTable stand-ins (same bytes).
#include "libpk/CSampleRef.h"
struct CSoundPlayParameters { long GetCategory() const; };   // 0x00C1BAC0 (descriptive libpk, as the landed 0x00C12940)

struct CEntry_00c119d0 {
    long m_0;
    CSoundSample* m_pSample;               // +4
    CSoundPlayParameters m_params;         // +8
};

struct CStateHit_00c119d0 {
    unsigned char _pad[0xc];
    long m_state;                          // +0xc
};

struct CStateMap_00c119d0 {                // embedded at +0x1f4
    virtual void Slot0();
    virtual void Slot4();
    virtual void Slot8();
    virtual void SlotC();
    virtual void Slot10();
    virtual void Slot14();
    virtual void Slot18();
    virtual void Slot1C();
    virtual void Slot20();
    virtual void Slot24();
    virtual void Slot28();
    virtual void Slot2C();
    virtual void Slot30();
    virtual void Slot34();
    virtual void Slot38();
    virtual void Slot3C();
    virtual void Slot40();
    virtual CStateHit_00c119d0* Find(CSampleRef* key);   // slot 0x44
};

struct CCombatStateTable_00c119d0 {
    unsigned char _pad[0x1f4];
    CStateMap_00c119d0 m_states;           // +0x1f4
    long ProcessAnimationState(void* tag, CEntry_00c119d0* entry);
};

long CCombatStateTable_00c119d0::ProcessAnimationState(void* tag, CEntry_00c119d0* entry)
{
    PKCProtectionInstance lock(&g_lock_0143bb14);
    if (entry) {
        CSampleRef key;
        key.Bind(entry->m_pSample, (long)entry, entry->m_params.GetCategory(), (long)tag);
        CStateHit_00c119d0* hit = m_states.Find(&key);
        key.Release();
        if (hit)
            return hit->m_state;
    }
    return -1;
}