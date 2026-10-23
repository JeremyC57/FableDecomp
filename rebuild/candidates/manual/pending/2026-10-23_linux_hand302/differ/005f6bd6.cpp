// CTCInventoryAbilities::SetAbilityAsAvailable(EHeroAbility, bool, bool) -- mark an ability (un)available, keep the received-order list and the special-move buttons in step, optionally re-page the assigned row
// Grade: DIFFER(588v618) -- logic modelled; retail keeps the ability in ebx and a zero in edi for the whole body, re-reads the level twice, and runs the duplicate scan on a byte offset with i+1 homed in the dead `available` slot; the lane allocates differently (3 strategies)
#pragma optimize("y", off)
#pragma optimize("s", on)
enum EHeroAbility { EHeroAbility_Force32 = 0x7fffffff };
template<class T> inline const T& StlMax(const T& a, const T& b) { return a < b ? b : a; }

struct CDefPointeeBase;
struct SAbilityEntry { long first; CDefPointeeBase* second; };          // pair<long, CDefPointer<const CHeroAbilityDef> >
struct CAbilityEntryVector {
    SAbilityEntry* _M_start; SAbilityEntry* _M_finish; SAbilityEntry* _M_end_of_storage;
    SAbilityEntry& operator[](unsigned long n) { return _M_start[n]; }
};
struct CAbilityVector {                                         // vector<EHeroAbility> (12 bytes)
    long* _M_start; long* _M_finish; long* _M_end_of_storage;
    unsigned long size() const { return _M_finish - _M_start; }
    long& operator[](unsigned long n) { return _M_start[n]; }
    long* erase(long* position);                                // 0x005F8750
};
long* __fastcall FindAbility(long* first, long* last, const EHeroAbility& value);   // 0x005F8150 (std::find)
struct CAbilityButtonMap {                                      // CVectorMap<long, EHeroAbility>
    struct SEntry { long first; long second; };
    SEntry* first; SEntry* last; SEntry* end_of_storage;
    SEntry* LowerBound(const long* key);                        // 0x005F7F4C
    void erase(SEntry* position);                               // 0x005F87C0
    SEntry* find(const long& key)
    {
        SEntry* it = LowerBound(&key);
        return (it == last || key < it->first) ? last : it;
    }
    SEntry* FindValue(long value)
    {
        SEntry* it = first;
        SEntry* end = last;
        while (it != end && !(it->second == value))
            ++it;
        return it;
    }
};

struct CPadController { long GetNumButtons() const; };          // 0x00445940
struct CPlayer { unsigned char _pad00[0xc]; CPadController* PController; };
struct CPlayerManager { const CPlayer& GetMainPlayer() const; };    // 0x00449960

// Layout: retail CTCInventoryAbilities (Ego_r.pdb - 0x18, 12-byte containers): WaitingToGiveTutorial +0x140,
// Initialised +0x141, CurrentAssignedAbilityLevel +0x144, ReceivedAbilityOrder +0x154, Abilities +0x160,
// AbilityButtons +0x16C.
struct CTCInventoryAbilities {
    void* __vftable;
    unsigned char _pad04[0x140 - 0x04];
    bool  WaitingToGiveTutorial;                                // +0x140
    bool  Initialised;                                          // +0x141
    unsigned char _pad142[2];
    long  CurrentAssignedAbilityLevel;                          // +0x144
    unsigned char _pad148[0x154 - 0x148];
    CAbilityVector      ReceivedAbilityOrder;                   // +0x154
    CAbilityEntryVector Abilities;                              // +0x160
    CAbilityButtonMap   AbilityButtons;                         // +0x16C
    CPlayerManager* GetPlayerManager();                         // 0x00686D50 (CTCBase)
    long GetNumAbilitiesPerRow() const;                         // 0x005EDCF0
    void AssignAbilityToButton(EHeroAbility ability, long button, bool quiet);   // 0x005F6423
    void CycleAssignedAbilities(bool forward);                  // 0x005F64DD
    __forceinline void AddReceivedAbility(EHeroAbility ability)
    {
        for (unsigned long i = 0; i < ReceivedAbilityOrder.size(); ++i) {
            if (ReceivedAbilityOrder[i] == 0) {
                ReceivedAbilityOrder[i] = ability;
                return;
            }
        }
        for (unsigned long i = 0; i < ReceivedAbilityOrder.size(); ++i) {
            for (unsigned long j = i + 1; j < ReceivedAbilityOrder.size(); ++j) {
                if (ReceivedAbilityOrder[j] == ReceivedAbilityOrder[i]) {
                    ReceivedAbilityOrder[j] = ability;
                    return;
                }
            }
        }
    }
    __forceinline long GetNumAssignedReceivedAbilities()
    {
        long count = 0;
        for (unsigned long i = 0; i < ReceivedAbilityOrder.size(); ++i)
            if (ReceivedAbilityOrder[i] != 0)
                ++count;
        return count;
    }
    void SetAbilityAsAvailable(EHeroAbility ability, bool available, bool updateRow);
};
void CTCInventoryAbilities::SetAbilityAsAvailable(EHeroAbility ability, bool available, bool updateRow)
{
    SAbilityEntry& entry = Abilities[ability];
    bool wasAvailable = entry.first >= 0;
    if (available)
        entry.first = StlMax(0L, entry.first);
    else
        entry.first = -1;
    if (Initialised) {
        if (available && !wasAvailable) {
            AddReceivedAbility(ability);
            long buttons = GetPlayerManager()->GetMainPlayer().PController->GetNumButtons();
            for (long button = 0; button < buttons; ++button) {
                if (AbilityButtons.find(button) == AbilityButtons.last) {
                    AssignAbilityToButton(ability, button, true);
                    break;
                }
            }
            if (GetNumAssignedReceivedAbilities() > GetNumAbilitiesPerRow())
                WaitingToGiveTutorial = true;
        } else if (wasAvailable && !available) {
            CAbilityButtonMap::SEntry* button = AbilityButtons.FindValue(ability);
            if (button != AbilityButtons.last)
                AbilityButtons.erase(button);
            long* received = FindAbility(ReceivedAbilityOrder._M_start, ReceivedAbilityOrder._M_finish, ability);
            if (received != ReceivedAbilityOrder._M_finish)
                ReceivedAbilityOrder.erase(received);
        }
    }
    if (updateRow) {
        CurrentAssignedAbilityLevel = StlMax(CurrentAssignedAbilityLevel - GetNumAbilitiesPerRow(), -GetNumAbilitiesPerRow());
        CycleAssignedAbilities(true);
    }
}
