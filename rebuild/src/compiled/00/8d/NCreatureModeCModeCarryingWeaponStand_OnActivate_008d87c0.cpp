// NCreatureMode::CModeCarryingWeaponStand::OnActivate() -- reset the holding filter's group to "", take the animation group of the weapon in the creature's primary carry slot, then the stand-mode activation
// Leaf: ?OnActivate@CModeCarryingWeaponStand
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
class CTCBase;

class CCharStringData { public:
    const char*   m_pData;                     // +0
    unsigned long m_length;                    // +4
};
class CCharString { public:
    CCharStringData* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
    CCharString& operator=(const CCharString& other);  // 0x0099EFB0
    unsigned long GetLength() const { return m_pData == 0 ? 0 : m_pData->m_length; }
    const char* GetPString() const { return m_pData == 0 ? "" : m_pData->m_pData; }
};

unsigned long __fastcall Calc_004014a0(unsigned long seed, const void* data, unsigned long length);   // 0x004014A0 CCRC::Calc

class CAnimFilterOfGroup { public:                    // vtable 0x01267568
    virtual void v00();
    CCharString   Group;                       // +4
    unsigned long GroupCRC;                    // +8
    void SetGroup(const CCharString& group);   // 0x0073DFF0
    __forceinline void SetGroupInline(const CCharString& group)   // the same body, expanded at this site
    {
        Group = group;
        GroupCRC = Calc_004014a0(0, group.GetPString(), group.GetLength());
    }
};

struct CInterfaceMap {                         // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    const SEntry* LowerBound(const long* type) const;  // 0x004365B0 (the const instantiation)
};

// Same-TU stand-in body for the out-of-line lower_bound (the retail one is a separate COMDAT): with it VC7.1 knows the key's
// address does not escape, so both lookup keys share the string temporary's frame word (GOTCHAS I-3).
#pragma auto_inline(off)
const CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type) const
{
    const SEntry* it = first;
    int len = int(last - first);
    while (len > 0) {
        int half = len / 2;
        const SEntry* middle = it + half;
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
    unsigned char  _pad0[0x20];
    unsigned long  m_presentBits[9];           // +0x20 std::bitset of interface types
    CInterfaceMap  m_interfaces;               // +0x44
    __forceinline unsigned long HasTC(long interfaceType) const
    {
        return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31));
    }
    template<class T> __forceinline T* GetTC(long interfaceType) const
    {
        long type = interfaceType;
        const CInterfaceMap* map = &m_interfaces;
        const CInterfaceMap::SEntry* it = map->LowerBound(&type);
        const CInterfaceMap::SEntry* sel = (it == map->last || interfaceType < it->type) ? map->last : it;
        return (T*)sel->component;
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