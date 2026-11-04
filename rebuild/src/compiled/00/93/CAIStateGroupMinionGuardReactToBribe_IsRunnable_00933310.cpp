// CAIStateGroup_MinionGuardReactToBribe::IsRunnable() -- with the guard's bribe component: react at once to a pending bribe (latch the live hero, no bribe received), else react while a bribe offered within the last 4 seconds is newer than 7 seconds after the last reaction (bribe received)
// Plan phase 2 (2026-11-04), CVectorMap wave (stand-in map): the local CInterfaceMap stand-in (and its same-TU LowerBound body) replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<..> > derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D; the same-TU noinline LowerBound is the header's); entries read as pair first/second, the map end through end(). Spellings unified: 0040f020: ?LowerBound@CInterfaceMap@@QAEPAUSEntry@1@PBJ@Z -> ?LowerBound@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@V?$CKeyPairCompareLess@W4ETCInterfaceType@@PAVCTCBase@@@@@@QAEPAU?$pair@W4ETCInterfaceType@@PAVCTCBase@@@std@@ABW4ETCInterfaceType@@@Z
// Naming wave 18: the global stand-in FramesPerSecond (0x01375550) is the protected static long CMainGameComponent::ConstantFPS, read through the
//   inline static accessor GetConstantFPS() as retail code outside the class does (name-only: the bytes are unchanged).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00933310, length 352 bytes (+ int3 padding).
// Callees: CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface 0xb4, bit 20 of +0x34), CAIStateGroupBase::
//          GetPPlayerManager @ 0x008FCF90, CPlayerManager::GetMainPlayer @ 0x00449970, CPlayer::GetPControlledCreature @ 0x00487DC0,
//          CBaseIntelligentPointer ctor/SetPItem/GetPItem/dtor (temporary vtable 0x01231C4C), CWorld::GetFrame
//          @ 0x0049D870, IS_THING_ALIVE<CIntelligentPointer<...>> @ 0x0048C740 (__fastcall, out of line at the second test).
// Globals: CMainGameComponent::GetConstantFPS() @ 0x01375550.
// Shape: `HasTC` (dword test against memory) then the out-param pick; a same-TU LowerBound stand-in lets the PHero
//          temporaries overlay the dead key slot (8-byte frame); PHero is assigned through a frame temporary with
//          the named-item operator= (the member address is formed after the temporary's GetPItem); the first
//          IS_THING_ALIVE is inline, the second the out-of-line template.
// Layout: engine/CAIStateGroup_MinionGuardReactToBribe.h (PHero +0x14, FrameLastReactedToReceiveing +0x1c,
//          HasRecievedBribe +0x20); PBrain is the first dword at +0x04; the 0xb4 component's +0x10 / +0x40 are descriptive.
#include "engine/CAIStateGroup_MinionGuardReactToBribe.h"

class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad9[0x3d];
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
            CInterfaceMap::value_type* it = map->LowerBound((const ETCInterfaceType&)type);
            if (it == map->end() || interfaceType < it->first)
                it = map->end();
            *ppOut = (T*)it->second;
        }
    }
};
class CThingPlayerCreature : public CThing { };
struct CTCGuardBribe {                          // interface 0xb4 (descriptive)
    unsigned char _pad[0x10];
    long PendingBribe;                          // +0x10 (descriptive: negative while a bribe awaits a reaction)
    unsigned char _pad14[0x40 - 0x14];
    long FrameBribeOffered;                     // +0x40 (descriptive)
};
class CPlayer { public:
    CThingPlayerCreature* GetPControlledCreature() const;                    // 0x00487DC0
};
// CPlayerManager: the shared model engine/CPlayerManager.h (Ego_r.pdb CPlayerManager; the five accessors declared
//   there with their retail addresses: GetPlayer 0x004498C0 / 0x00449910, GetMainPlayer 0x00449970 / 0x00449960,
//   GetMainPlayerNumber 0x00449700).
#include "engine/CPlayerManager.h"
class CWorld { public: static long __fastcall GetFrame(); };            // 0x0049D870
class CMainGameComponent {
protected:
    static long ConstantFPS;                             // 0x01375550 ?ConstantFPS@CMainGameComponent@@1JA (protected static)
public:
    static long __fastcall GetConstantFPS() { return ConstantFPS; }   // Ego_r / FableWin ?GetConstantFPS@CMainGameComponent@@SIJXZ (inline)
};

class CBaseObject;
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CBaseObject* GetPItem() const;                   // 0x00A01B50
    CBaseIntelligentPointer();                  // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                 // 0x00A01C10
    void SetPItem(CBaseObject*a0);                // 0x00A01B90
};
struct CIntelligentPointerHero : CBaseIntelligentPointer {   // CIntelligentPointer<CThingPlayerCreature>, vtable 0x01231C4C
    virtual void v00();
    __forceinline CIntelligentPointerHero(CThing* item) : CBaseIntelligentPointer() { SetPItem((CBaseObject*)item); }
    __forceinline CIntelligentPointerHero& operator=(const CIntelligentPointerHero& other) { CThing* item = ((CThing*)other.GetPItem()); SetPItem((CBaseObject*)item); return *this; }
};
__forceinline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (((CThing*)p.GetPItem()) != 0) {
        if (!(((CThing*)p.GetPItem())->m_flags91 & 1))
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
            Hero() = CIntelligentPointerHero(GetPPlayerManager()->GetMainPlayer().GetPControlledCreature());
            if (IS_THING_ALIVE(Hero())) {
                HasRecievedBribe = false;
                return true;
            }
        }
        long deadline = bribe->FrameBribeOffered + CMainGameComponent::GetConstantFPS() * 4;
        long earliest = CMainGameComponent::GetConstantFPS() * 7 + FrameLastReactedToReceiveing;
        if (CWorld::GetFrame() < deadline && deadline > earliest) {
            Hero() = CIntelligentPointerHero(GetPPlayerManager()->GetMainPlayer().GetPControlledCreature());
            if (IS_THING_ALIVE_Pointer(Hero())) {
                HasRecievedBribe = true;
                return true;
            }
        }
    }
    return false;
}