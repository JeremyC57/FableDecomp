// NCreatureMode::CModeCarryingWeaponStand::OnActivate() -- reset the holding filter's group to "", take the animation group of the weapon in the creature's primary carry slot, then the stand-mode activation
// Leaf: ?OnActivate@CModeCarryingWeaponStand
// Plan phase 2 (2026-11-04), CVectorMap wave (stand-in map): the local stand-in map class (and its same-TU LowerBound body) replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<..> > derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D; the same-TU noinline LowerBound is the header's); entries read as pair first/second, the map end through end(). Spellings unified: 004365b0: ?LowerBound@CInterfaceMap@@QBEPBUSEntry@1@PBJ@Z -> ?LowerBound@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@V?$CKeyPairCompareLess@W4ETCInterfaceType@@PAVCTCBase@@@@@@QBEPBU?$pair@W4ETCInterfaceType@@PAVCTCBase@@@std@@ABW4ETCInterfaceType@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x008D87C0, 211 bytes (+ int3 padding), `ret`; vtable 0x012856E4 slot 5.
// Callees: CCharString(const char*, long) @ 0x0099EBF0 ("" @ 0x0122D70E), CCharString::operator= @ 0x0099EFB0, CCRC::Calc
//   @ 0x004014A0 (`__fastcall`; "" @ 0x0129AAF4 for a null string), ~CCharString @ 0x0099EAE0, CVectorMap::LowerBound (const)
//   @ 0x004365B0 (inlined HasTC + GetTC for CTCCarrying 0x46 and CTCWeapon 0x48), CTCCarrying::GetThingInPrimarySlot
//   @ 0x006A1440, CTCWeapon::GetAnimationGroupName @ 0x005DABE0, CAnimFilterOfGroup::SetGroup @ 0x0073DFF0,
//   CModeStandBase::OnActivate @ 0x008621A0 (qualified; CModeCarryingStandBase does not override slot 5).
// Name: FableWin.pdb `?OnActivate@CModeCarryingWeaponStand@NCreatureMode@@UAEXXZ` (slot 5).
// Layout: Ego_r.pdb CModeCarryingWeaponStand (0x74): CModeStandBase 0x68, Filter +0x68 (CAnimFilterOfGroup: Group +4,
//   GroupCRC +8).
// Shape: as the landed CModeStrafe::SetChangeOfWeapon @ 0x008E9850, SetGroup is expanded inline at one site (here the
//   first, with the literal's temporary) and called out of line at the other; the filter address rides in edi; the
//   carrying test is the byte presence test, the weapon test the dword `unsigned long` HasTC (`mov ecx,[eax+28h] ;
//   test ch,1`); both GetTC lookups are the one-ternary returned form (component straight into ecx) and share the
//   string temporary's frame slot for their key.
#include "fable_compiler.h"
class CTCBase;

class CCharStringData { public:
    const char*   m_pData;                     // +0
    unsigned long m_length;                    // +4
};
class CCharString { public:
    CCharStringData* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
    const CCharString& operator=(const CCharString& other);  // 0x0099EFB0
    unsigned long GetLength() const { return m_pData == 0 ? 0 : m_pData->m_length; }
    const char* GetPString() const { return m_pData == 0 ? "" : m_pData->m_pData; }
};

class CCRC {
public:
    static unsigned long __fastcall Calc(unsigned long, const void*, unsigned long);   // 0x004014A0
};   // 0x004014A0 CCRC::Calc

class CAnimFilterOfGroup { public:                    // vtable 0x01267568
    virtual void v00();
    CCharString   Group;                       // +4
    unsigned long GroupCRC;                    // +8
    void SetGroup(const CCharString& group);   // 0x0073DFF0
    FABLE_FORCEINLINE void SetGroupInline(const CCharString& group)   // the same body, expanded at this site
    {
        Group = group;
        GroupCRC = CCRC::Calc(0, group.GetPString(), group.GetLength());
    }
};

enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;


class CThing { public:
    unsigned char  _pad0[0x20];
    unsigned long  m_presentBits[9];           // +0x20 std::bitset of interface types
    CInterfaceMap  m_interfaces;               // +0x44
    FABLE_FORCEINLINE unsigned long HasTC(long interfaceType) const
    {
        return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31));
    }
    template<class T> FABLE_FORCEINLINE T* GetTC(long interfaceType) const
    {
        long type = interfaceType;
        const CInterfaceMap* map = &m_interfaces;
        const CInterfaceMap::value_type* it = map->LowerBound((const ETCInterfaceType&)type);
        const CInterfaceMap::value_type* sel = (it == map->end() || interfaceType < it->first) ? map->end() : it;
        return (T*)sel->second;
    }
};

class CTCCarrying { public:                           // interface type 0x46
    CThing* GetThingInPrimarySlot() const;     // 0x006A1440
};
class CTCWeapon { public:                             // interface type 0x48
    const CCharString& GetAnimationGroupName() const;   // 0x005DABE0
};

struct CModeBase {                             // CTCCreatureModeManager::CModeBase, retail 0x1c
    virtual void v00();
    CThing*       ParentThing;                 // +0x04
    unsigned char _body[0x14];
};
struct CModeStandBase : CModeBase {            // retail sizeof 0x68
    unsigned char _stand[0x4c];
    void OnActivate();                         // 0x008621A0
};

namespace NCreatureMode {
struct CModeCarryingStandBase : CModeStandBase {
};
struct CModeCarryingWeaponStand : CModeCarryingStandBase {   // vtable 0x012856E4
    CAnimFilterOfGroup Filter;                 // +0x68
    void OnActivate();
};
}

void NCreatureMode::CModeCarryingWeaponStand::OnActivate()
{
    Filter.SetGroupInline("");
    if (ParentThing->HasTC(0x46)) {
        CThing* weapon = ParentThing->GetTC<CTCCarrying>(0x46)->GetThingInPrimarySlot();
        if (weapon != 0 && weapon->HasTC(0x48))
            Filter.SetGroup(weapon->GetTC<CTCWeapon>(0x48)->GetAnimationGroupName());
    }
    CModeCarryingStandBase::OnActivate();
}