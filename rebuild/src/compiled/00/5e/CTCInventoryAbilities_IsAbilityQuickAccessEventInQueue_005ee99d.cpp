// CTCInventoryAbilities::IsAbilityQuickAccessEventInQueue(EHeroAbility ability) -- true when one of the inventory's quick-access slots holding the ability's quick-access item has a press of type 2 queued for the main player
// Leaf: IsAbilityQuickAccessEventInQueue
// Retail 0x005EE99D, 254 bytes, `ret 4`. Frame-pointer size TU: optimize("y", off) + ("s", on).
// Callees: GetAbilityPDef(EHeroAbility) const 0x005EDFD9 (counted CDefPointer by value, built in the dead `ability`
// argument slot), std::bitset::test 0x00410DE0 + the const CVectorMap::LowerBound 0x004365B0 (inline GetInterface for the
// inventory component 0x11), CTCBase::PeekPlayerManager() const 0x00686D50, CPlayerManager::GetMainPlayer() const
// 0x00449960, CPlayer::GetPlayerNumber 0x00487510, the main game component's (0x013B86A0) game player interface
// getter 0x00416359, CGamePlayerInterface::IsAssignableSpecialMoveButtonInQueue(long, EInputButtonEventType, long)
// 0x004460C0, the def's slot 1 on release.
// Shape: the def's quick-access item id (+0x98) must be positive; the inventory's quick-access slot vector (+0x148,
// size re-read every pass) is scanned for it and each hit asks the input queue; both exits release the def through
// its inline destructor (the null test folds into the `if (def)`); the call object comes after the argument chain.
// `this` stays in edx across GetAbilityPDef because its real body (byte-exact to 0x005EDFD9, frame omitted) is compiled
// above the caller in the same TU; g_pMainGameComponent is read first through an inline accessor (ebx before the
// argument chain); the vector's start is loaded into a local each pass so the size test and the element read share it.
// Layout: retail CHeroAbilityDef quick-access item +0x98; CTCInventory quick-access slots vector<long> +0x148.
#pragma optimize("y", off)
#pragma optimize("s", on)

struct CPresenceBits {
    bool test(long bit);                             // 0x00410DE0 (std::bitset::test)
};
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    const SEntry* LowerBound(const ETCInterfaceType& type) const;      // 0x004365B0
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing { public:
    unsigned char _pad_0x00[0x20];
    CPresenceBits m_present;                         // +0x20
    unsigned char _pad_0x21[0x23];
    CInterfaceMap m_interfaces;                      // +0x44
    template<class T> __forceinline bool GetInterface(T** out, long wanted)
    {
        if (m_present.test(wanted)) {
            ETCInterfaceType type = (ETCInterfaceType)(wanted);
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::SEntry* it = map->LowerBound(type);
            *out = (T*)((void*)((it == map->last || wanted < it->first) ? map->last : it)->second);
            return true;
        }
        return false;
    }
};
struct CLongVector {
    long* first;
    long* last;
    long* end_of_storage;
    unsigned long size() const { return (unsigned long)(last - first); }
    long& operator[](unsigned long n) { return *(first + n); }
};
class CTCInventory { public:                                // interface type 0x11
    unsigned char _pad_0x000[0x148];
    CLongVector QuickAccessItems;                    // +0x148
};
class CHeroAbilityDef { public:
    virtual void v00();
    virtual void Destroy();                          // slot 1
    long RefCount;                                   // +0x04
    unsigned char _pad_0x08[0x98 - 0x08];
    long QuickAccessItem;                            // +0x98
    void Release() { if (--RefCount == 0) Destroy(); }
};
template <class T> class CDefPointer { public:
    T* Object;
    __forceinline CDefPointer(const CDefPointer& other) : Object(other.Object) { if (Object) ++Object->RefCount; }
    ~CDefPointer() { if (Object) Object->Release(); }
    T* operator->() const { return Object; }
};
class CPlayer { public:
    long GetPlayerNumber();                          // 0x00487510
};
class CPlayerManager { public:
    CPlayer* GetMainPlayer() const;                  // 0x00449960
};
class CGamePlayerInterface { public:
    bool IsAssignableSpecialMoveButtonInQueue(long button, long buttonEventType, long playerNumber);   // 0x004460C0
};
class CMainGameComponent { public:
    CGamePlayerInterface* GetGamePlayerInterface(); // 0x00416359
};
extern CMainGameComponent* g_pMainGameComponent;    // 0x013B86A0
__forceinline CMainGameComponent* GetMainGameComponent() { return g_pMainGameComponent; }
enum EHeroAbility { EHeroAbility_Force32 = 0x7fffffff };

struct SAbilityEntry {                               // std::pair<long, CDefPointer<const CHeroAbilityDef> >
    long                         Level;
    CDefPointer<CHeroAbilityDef> PDef;
};
struct CAbilityVector {                              // STLport vector<SAbilityEntry>
    SAbilityEntry* _M_start;
    SAbilityEntry* _M_finish;
    SAbilityEntry* _M_end_of_storage;
    const SAbilityEntry& operator[](unsigned long n) const { return *(_M_start + n); }
};
class CTCBase
{
protected:
    const CPlayerManager& PeekPlayerManager() const;   // 0x00686D50
};
namespace NInventory {
class CTCInventoryBase : public CTCBase
{
public:
};
}

class CTCInventoryAbilities : public NInventory::CTCInventoryBase { public:
    void* __vftable;
    CThing* ParentThing;                             // +0x04
    unsigned char _pad_0x08[0x160 - 0x08];
    CAbilityVector Abilities;                        // +0x160 (PDB +0x180)
    CDefPointer<CHeroAbilityDef> GetAbilityPDef(EHeroAbility ability) const;   // 0x005EDFD9
    bool IsAbilityQuickAccessEventInQueue(EHeroAbility ability);
};

// Same-TU body of GetAbilityPDef @ 0x005EDFD9 (retail's TU, above the caller): it leaves edx alone, so VC7.1 keeps
// `this` there across the call, as retail does.
#pragma optimize("y", on)
__declspec(noinline) CDefPointer<CHeroAbilityDef> CTCInventoryAbilities::GetAbilityPDef(EHeroAbility ability) const
{
    return Abilities[ability].PDef;
}
#pragma optimize("y", off)

bool CTCInventoryAbilities::IsAbilityQuickAccessEventInQueue(EHeroAbility ability)
{
    CDefPointer<CHeroAbilityDef> def = GetAbilityPDef(ability);
    if (def.Object) {
        long item = def->QuickAccessItem;
        if (item > 0) {
            CTCInventory* inventory;
            if (ParentThing->GetInterface(&inventory, 0x11) && inventory) {
                for (unsigned long i = 0; ; ++i) {
                    const long* data = inventory->QuickAccessItems.first;
                    if (i >= (unsigned long)(inventory->QuickAccessItems.last - data))
                        break;
                    if (data[i] == item &&
                        GetMainGameComponent()->GetGamePlayerInterface()->IsAssignableSpecialMoveButtonInQueue(
                            i, 2, PeekPlayerManager().GetMainPlayer()->GetPlayerNumber()))
                        return true;
                }
            }
        }
    }
    return false;
}