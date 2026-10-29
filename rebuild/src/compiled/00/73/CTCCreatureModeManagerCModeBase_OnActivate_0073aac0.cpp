// CTCCreatureModeManager::CModeBase::OnActivate() -- mark the mode active and point the weapon-based filter at the animation group of the weapon in the creature's primary carrying slot, or at "STAND" when it carries no live weapon
// Leaf: OnActivate
// Retail: Fable.exe (TLC, VC7.1) @ 0x0073AAC0, 219 bytes (`ret`).
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound 0x0040F020 (non-const) and the const instantiation 0x004365B0;
// CTCCarrying::GetThingInPrimarySlot 0x006A1440; CTCWeapon::GetAnimationGroupName 0x005DABE0;
// CAnimFilterOfGroup::SetGroup(const CCharString&) 0x0073DFF0 (manifest label "CRCHashedString_SetAndHash");
// CCharString(const char*, long) 0x0099EBF0, operator= 0x0099EFB0, ~CCharString 0x0099EAE0; CRC 0x004014A0.
// Layout (retail CModeBase 0x1c): ParentThing +0x04, WeaponBasedFilter +0x08 (CAnimFilterOfGroup: Group +0x0c,
// GroupCRC +0x10), FramesBeforeRemovingSelf +0x14, Active +0x18. CThing: interface bitset +0x20 (CTCCarrying 0x46 = bit 6
// of +0x28, CTCWeapon 0x48 = bit 8), interface map +0x44, dead flag bit 0 of +0x91.
// Shape: the carrying component comes from the non-const out-parameter GetInterface; the carried thing is used through a
// const pointer, so its weapon lookup binds the const LowerBound copy. The weapon arm calls SetGroup out of line, while the
// "STAND" fallback is an inline copy of the same assignment + CRC (the literal's CCharString temporary, one shared null test).
class CCharStringData { public:
    const char*   m_pData;                 // +0
    unsigned long m_length;                // +4
};

class CCharString { public:
    CCharStringData* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                        // 0x0099EAE0
    const CCharString& operator=(const CCharString& other);   // 0x0099EFB0
    unsigned long GetLength() const { return m_pData == 0 ? 0 : m_pData->m_length; }
    const char* GetPString() const { return m_pData == 0 ? "" : m_pData->m_pData; }
};

class CCRC {
public:
    static unsigned long __fastcall Calc(unsigned long, const void*, unsigned long);   // 0x004014A0
};   // 0x004014A0: CRC

class CAnimFilterOfGroup { public:                // vtable 0x01267568
    virtual void v00();
    CCharString   Group;                   // +4
    unsigned long GroupCRC;                // +8
    void SetGroup(const CCharString& group);   // 0x0073DFF0 (the out-of-line copy)
    __forceinline void SetGroupInline(const CCharString& group)
    {
        Group = group;
        GroupCRC = CCRC::Calc(0, group.GetPString(), group.GetLength());
    }
};

class CTCBase;

enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:                     // CVectorMap<ETCInterfaceType, CTCBase*>
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);              // 0x0040F020
    const SEntry* LowerBound(const ETCInterfaceType& type) const;  // 0x004365B0 (the const instantiation)
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];        // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;            // +0x44
    unsigned char _pad1[0x91 - 0x4c];
    unsigned char m_lifeFlags;             // +0x91 (bit 0: dead)
    bool IsDead() const { return (m_lifeFlags & 1) != 0; }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(type);
            if (it == map->last || interfaceType < it->first)
                it = map->last;
            *ppOut = (T*)it->second;
            return true;
        }
        return false;
    }
    template<class T> __forceinline bool GetInterface(const T** ppOut, long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::SEntry* it = map->LowerBound(type);
            if (it == map->last || interfaceType < it->first)
                it = map->last;
            *ppOut = (const T*)it->second;
            return true;
        }
        return false;
    }
};

class CTCCarrying { public:                       // interface type 0x46
public:
    CThing* GetThingInPrimarySlot() const;   // 0x006A1440
};

class CTCWeapon { public:                         // interface type 0x48
    const CCharString& GetAnimationGroupName() const;   // 0x005DABE0
};

class CTCCreatureModeManager { public:
    class CModeBase { public:                     // vtable 0x012673CC
        virtual void v00();
        CThing*            ParentThing;               // +0x04
        CAnimFilterOfGroup WeaponBasedFilter;         // +0x08
        long               FramesBeforeRemovingSelf;  // +0x14
        bool               Active;                    // +0x18
        void OnActivate();
    };
};

void CTCCreatureModeManager::CModeBase::OnActivate()
{
    Active = true;
    CTCCarrying* carrying;
    if (ParentThing->GetInterface(&carrying, 0x46)) {
        const CThing* item = carrying->GetThingInPrimarySlot();
        if (item && !item->IsDead()) {
            const CTCWeapon* weapon;
            if (item->GetInterface(&weapon, 0x48)) {
                WeaponBasedFilter.SetGroup(weapon->GetAnimationGroupName());
                return;
            }
        }
    }
    WeaponBasedFilter.SetGroupInline("STAND");
}