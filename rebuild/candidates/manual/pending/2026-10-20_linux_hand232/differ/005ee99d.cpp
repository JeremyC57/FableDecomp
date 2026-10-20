// Grade: DIFFER(269v254) -- retail keeps `this` in edx across the GetAbilityPDef call (whole-program register knowledge of the LTCG callee; lane gap) and spills it to [ebp-8]; then the frame (0xC v 8: item spill), the size() load order (first then last) and the global/player-interface scheduling differ. A same-TU stand-in GetAbilityPDef (probe/005ee99d_v2) enregisters the def instead (251v254).
// CTCInventoryAbilities::IsAbilityQuickAccessEventInQueue(EHeroAbility ability) -- true when one of the inventory's quick-access slots holding the ability's quick-access item has a press of type 2 queued for the main player
// Retail 0x005EE99D, 254 bytes, `ret 4`. Frame-pointer size TU: optimize("y", off) + ("s", on).
// Callees: GetAbilityPDef(EHeroAbility) const 0x005EDFD9 (counted CDefPointer by value, built in the dead `ability`
// argument slot), std::bitset::test 0x00410DE0 + the const CVectorMap::LowerBound 0x004365B0 (inline GetInterface for the
// inventory component 0x11), CTCBase::GetPlayerManager() const 0x00686D50, CPlayerManager::GetMainPlayer() const
// 0x00449960, CPlayer::GetPlayerNumber 0x00487510, the main game component's (0x013B86A0) game player interface
// getter 0x00416359, CGamePlayerInterface::IsAssignableSpecialMoveButtonInQueue(long, EInputButtonEventType, long)
// 0x004460C0, the def's slot 1 on release.
// Shape: the def's quick-access item id (+0x98) must be positive; the inventory's quick-access slot vector (+0x148,
// size re-read every pass) is scanned for it and each hit asks the input queue; both exits release the def through
// its inline destructor (the null test folds into the `if (def)`); the call object comes after the argument chain.
// Layout: retail CHeroAbilityDef quick-access item +0x98; CTCInventory quick-access slots vector<long> +0x148.
#pragma optimize("y", off)
#pragma optimize("s", on)

struct CPresenceBits {
    bool test(long bit);                             // 0x00410DE0 (std::bitset::test)
};
struct CInterfaceMap {
    struct SEntry { long type; void* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) const;      // 0x004365B0
};
struct CThing {
    unsigned char _pad_0x00[0x20];
    CPresenceBits m_present;                         // +0x20
    unsigned char _pad_0x21[0x23];
    CInterfaceMap m_interfaces;                      // +0x44
    template<class T> __forceinline bool GetInterface(T** out, long wanted)
    {
        if (m_present.test(wanted)) {
            long type = wanted;
            const CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *out = (T*)((it == map->last || wanted < it->type) ? map->last : it)->component;
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
struct CTCInventory {                                // interface type 0x11
    unsigned char _pad_0x000[0x148];
    CLongVector QuickAccessItems;                    // +0x148
};
struct CHeroAbilityDef {
    virtual void v00();
    virtual void Destroy();                          // slot 1
    long RefCount;                                   // +0x04
    unsigned char _pad_0x08[0x98 - 0x08];
    long QuickAccessItem;                            // +0x98
    void Release() { if (--RefCount == 0) Destroy(); }
};
template <class T> struct CDefPointer {
    T* Object;
    CDefPointer(const CDefPointer& other);
    ~CDefPointer() { if (Object) Object->Release(); }
    T* operator->() const { return Object; }
};
struct CPlayer {
    long GetPlayerNumber();                          // 0x00487510
};
struct CPlayerManager {
    CPlayer* GetMainPlayer() const;                  // 0x00449960
};
struct CGamePlayerInterface {
    bool IsAssignableSpecialMoveButtonInQueue(long button, long buttonEventType, long playerNumber);   // 0x004460C0
};
struct CMainGameComponent {
    CGamePlayerInterface* GetGamePlayerInterface(); // 0x00416359
};
extern CMainGameComponent* g_pMainGameComponent;    // 0x013B86A0
enum EHeroAbility { EHeroAbility_Force32 = 0x7fffffff };

struct CTCInventoryAbilities {
    void* __vftable;
    CThing* ParentThing;                             // +0x04
    CDefPointer<CHeroAbilityDef> GetAbilityPDef(EHeroAbility ability) const;   // 0x005EDFD9
    const CPlayerManager* GetPlayerManager() const;  // 0x00686D50 (CTCBase)
    bool IsAbilityQuickAccessEventInQueue(EHeroAbility ability);
};

bool CTCInventoryAbilities::IsAbilityQuickAccessEventInQueue(EHeroAbility ability)
{
    CDefPointer<CHeroAbilityDef> def = GetAbilityPDef(ability);
    if (def.Object) {
        long item = def->QuickAccessItem;
        if (item > 0) {
            CTCInventory* inventory;
            if (ParentThing->GetInterface(&inventory, 0x11) && inventory) {
                for (unsigned long i = 0; i < inventory->QuickAccessItems.size(); ++i) {
                    if (inventory->QuickAccessItems[i] == item &&
                        g_pMainGameComponent->GetGamePlayerInterface()->IsAssignableSpecialMoveButtonInQueue(
                            i, 2, GetPlayerManager()->GetMainPlayer()->GetPlayerNumber()))
                        return true;
                }
            }
        }
    }
    return false;
}
