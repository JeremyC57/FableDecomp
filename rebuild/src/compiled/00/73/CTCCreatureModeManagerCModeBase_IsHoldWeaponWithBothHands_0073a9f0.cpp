// CTCCreatureModeManager::CModeBase::IsHoldWeaponWithBothHands() const -- true when the creature carries, in the "CARRY_SLOT_BOTH_HANDS" carry slot, a live weapon thing it is able to use
// Leaf: IsHoldWeaponWithBothHands
// Retail: Fable.exe (TLC, VC7.1) @ 0x0073A9F0, 208 bytes (`ret`), CModeBase vtable 0x012673CC slot 10. FableWin.pdb names the
//   slot-10 virtual ?IsHoldWeaponWithBothHands@CModeBase@CTCCreatureModeManager@@UBE_NXZ (the only `bool () const` virtual
//   left once IsIdleMode / IsRemoveOnDeactivate / IsAllowingPlayerSpecialMovementMoves are placed); the manifest has vfunc_10.
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound 0x004365B0 (the const instantiation, inlined GetTC for CTCCarrying
//   0x46 and CTCWeapon 0x48), CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0, CThing::PeekDefinitionManager
//   0x004C7990, CDefinitionManager::GetDefGlobalIndexFromName 0x009AD410, the carry-slot lookup CTCCarrying::GetThingInCarrySlot
//   (long slot) 0x006A11F0 (manifest label CThing_Array_FindByID is propagated), CTCWeapon::IsUsableBy(CThing*) 0x005DAB10.
// Layout: CModeBase ParentThing +0x04 (as the landed CModeStrafe::UpdateWeaponState 0x008E9950, whose thing model this reuses):
//   thing interface bitset +0x20 (0x46 = bit 6 of +0x28, 0x48 = bit 8 of +0x28), interface map +0x44, dead bit 0 of +0x91.
// Shape: HasTC then a separate GetTC (one bit test per component); the def-index string is the literal "CARRY_SLOT_BOTH_HANDS"
//   (0x01268748) as a temporary CCharString argument; the result is an explicit `if (...) return true;`.
class CTCBase;
class CThing;

class CCharString { public:
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};

class CDefinitionManager { public:
    long GetDefGlobalIndexFromName(const CCharString& name) const;   // 0x009AD410
};
class CGameDefinitionManager : public CDefinitionManager
{
};

enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:                     // CVectorMap<ETCInterfaceType, CTCBase*>
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    const SEntry* LowerBound(const ETCInterfaceType& type) const;  // 0x004365B0 (the const instantiation)
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThing { public:
    unsigned char  _pad0[0x20];
    unsigned long  m_presentBits[9];       // +0x20 std::bitset of interface types
    CInterfaceMap  m_interfaces;           // +0x44
    unsigned char  _pad4c[0x91 - 0x4c];
    unsigned char  m_lifeFlags;            // +0x91 (bit 0: dead)
    bool IsDead() const { return (m_lifeFlags & 1) != 0; }
    __forceinline bool HasTC(long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)))
            return true;
        return false;
    }
    template<class T> __forceinline const T* GetTC(long interfaceType) const
    {
        ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
        const CInterfaceMap* map = &m_interfaces;
        const CInterfaceMap::SEntry* it = map->LowerBound(type);
        const CInterfaceMap::SEntry* sel = (it == map->last || interfaceType < it->first) ? map->last : it;
        return (const T*)sel->second;
    }
public:
    const CGameDefinitionManager& PeekDefinitionManager() const;   // 0x004C7990
};

class CTCCarrying { public:                       // interface type 0x46
    CThing* GetThingInCarrySlot(long slot) const;   // 0x006A11F0
};

class CTCWeapon { public:                         // interface type 0x48
    bool IsUsableBy(CThing* user) const;   // 0x005DAB10
};

namespace CTCCreatureModeManager {
class CModeBase { public:                         // retail 0x1c
    virtual void v00();
    CThing*       ParentThing;             // +0x04
    unsigned char _body[0x14];
    virtual bool IsHoldWeaponWithBothHands() const;
};

bool CModeBase::IsHoldWeaponWithBothHands() const
{
    if (ParentThing->HasTC(0x46))
    {
        const CTCCarrying* carrying = ParentThing->GetTC<CTCCarrying>(0x46);
        long slot = ParentThing->PeekDefinitionManager().GetDefGlobalIndexFromName("CARRY_SLOT_BOTH_HANDS");
        CThing* weapon = carrying->GetThingInCarrySlot(slot);
        if (weapon && !weapon->IsDead() && weapon->HasTC(0x48))
        {
            if (weapon->GetTC<CTCWeapon>(0x48)->IsUsableBy(ParentThing))
                return true;
        }
    }
    return false;
}
}