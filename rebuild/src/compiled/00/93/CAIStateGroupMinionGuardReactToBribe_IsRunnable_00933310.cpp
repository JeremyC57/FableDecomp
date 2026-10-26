// CAIStateGroup_MinionGuardReactToBribe::IsRunnable() -- with the guard's bribe component: react at once to a pending bribe (latch the live hero, no bribe received), else react while a bribe offered within the last 4 seconds is newer than 7 seconds after the last reaction (bribe received)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00933310, length 352 bytes (+ int3 padding).
// Callees: CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface 0xb4, bit 20 of +0x34), CAIStateGroupBase::
//          GetPPlayerManager @ 0x008FCF90, CPlayerManager::GetMainPlayer @ 0x00449970, CPlayer::GetPCharacter @ 0x00487DC0,
//          CBaseIntelligentPointer ctor/SetPItem/GetPItem/dtor (temporary vtable 0x01231C4C), GFGetCurrentFrame
//          @ 0x0049D870, IS_THING_ALIVE<CIntelligentPointer<...>> @ 0x0048C740 (__fastcall, out of line at the second test).
// Globals: FramesPerSecond @ 0x01375550.
// Shape: `HasTC` (dword test against memory) then the out-param pick; a same-TU LowerBound stand-in lets the PHero
//          temporaries overlay the dead key slot (8-byte frame); PHero is assigned through a frame temporary with
//          the named-item operator= (the member address is formed after the temporary's GetPItem); the first
//          IS_THING_ALIVE is inline, the second the out-of-line template.
// Layout: engine/CAIStateGroup_MinionGuardReactToBribe.h (PHero +0x14, FrameLastReactedToReceiveing +0x1c,
//          HasRecievedBribe +0x20); PBrain is the first dword at +0x04; the 0xb4 component's +0x10 / +0x40 are descriptive.
#include "engine/CAIStateGroup_MinionGuardReactToBribe.h"

class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020 (stand-in body below)
};
#pragma auto_inline(off)
// Same-TU stand-in for CVectorMap::LowerBound (0x0040F020, STLport lower_bound; retail's LTCG saw its body): the key's
// address does not escape, so its frame slot is free again for the PHero temporaries.
__declspec(noinline) CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    SEntry* it = first;
    int len = int(last - first);
    while (len > 0) {
        int half = len / 2;
        SEntry* middle = it + half;
        if (middle->type < *type) {
            it = middle + 1;
            len = len - half - 1;
        } else
            len = half;
    }
    return it;
}
#pragma auto_inline(on)
class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad1[0x91 - 0x4c];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    __forceinline unsigned long HasTC(long interfaceType) const
    {
        return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31));
    }
    template<class T> __forceinline void GetInterfaceV(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
        }
    }
};
struct CTCGuardBribe {                          // interface 0xb4 (descriptive)
    unsigned char _pad[0x10];
    long PendingBribe;                          // +0x10 (descriptive: negative while a bribe awaits a reaction)
    unsigned char _pad14[0x40 - 0x14];
    long FrameBribeOffered;                     // +0x40 (descriptive)
};
class CPlayer { public:
    CThing* GetPCharacter();                    // 0x00487DC0
};
class CPlayerManager { public:
    CPlayer* GetMainPlayer();                   // 0x00449970
};
long __fastcall GFGetCurrentFrame();            // 0x0049D870
extern long FramesPerSecond;                    // 0x01375550

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CThing* GetPItem() const;                   // 0x00A01B50
    CBaseIntelligentPointer();                  // 0x00A01B10
    ~CBaseIntelligentPointer();                 // 0x00A01C10
    void SetPItem(CThing* item);                // 0x00A01B90
};
struct CIntelligentPointerHero : CBaseIntelligentPointer {   // CIntelligentPointer<CThingPlayerCreature>, vtable 0x01231C4C
    virtual void v00();
    __forceinline CIntelligentPointerHero(CThing* item) : CBaseIntelligentPointer() { SetPItem(item); }
    __forceinline CIntelligentPointerHero& operator=(const CIntelligentPointerHero& other) { CThing* item = other.GetPItem(); SetPItem(item); return *this; }
};
__forceinline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}
bool __fastcall IS_THING_ALIVE_Pointer(const CBaseIntelligentPointer& p);   // 0x0048C740 (IS_THING_ALIVE<CIntelligentPointer<T>>)
class CAIBrain { public:
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};

struct CAIStateGroupMinionGuardReactToBribeMethods : CAIStateGroup_MinionGuardReactToBribe {
    CAIBrain* Brain() const { return *(CAIBrain* const*)_pad_0x04; }
    CThing* GetCreature() const { return Brain()->PCreature; }
    CIntelligentPointerHero& Hero() { return *(CIntelligentPointerHero*)PHero; }
    CPlayerManager* GetPPlayerManager();        // 0x008FCF90
    bool IsRunnable();
};

bool CAIStateGroupMinionGuardReactToBribeMethods::IsRunnable()
{
    CThing* creature = GetCreature();
    if (creature->HasTC(0xb4)) {
        CTCGuardBribe* bribe;
        creature->GetInterfaceV(&bribe, 0xb4);
        if (bribe->PendingBribe < 0) {
            Hero() = CIntelligentPointerHero(GetPPlayerManager()->GetMainPlayer()->GetPCharacter());
            if (IS_THING_ALIVE(Hero())) {
                HasRecievedBribe = false;
                return true;
            }
        }
        long deadline = bribe->FrameBribeOffered + FramesPerSecond * 4;
        long earliest = FramesPerSecond * 7 + FrameLastReactedToReceiveing;
        if (GFGetCurrentFrame() < deadline && deadline > earliest) {
            Hero() = CIntelligentPointerHero(GetPPlayerManager()->GetMainPlayer()->GetPCharacter());
            if (IS_THING_ALIVE_Pointer(Hero())) {
                HasRecievedBribe = true;
                return true;
            }
        }
    }
    return false;
}