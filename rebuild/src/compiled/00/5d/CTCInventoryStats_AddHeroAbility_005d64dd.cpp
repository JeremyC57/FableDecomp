// CTCInventoryStats::AddHeroAbility(NUISystem::CComponent* parent, EHeroAbility ability, float& yPos) -- add one hero ability row to the stats screen: a tick slider named after the ability, showing its current level, its possible level-up and its maximum, coloured by the ability; a slider for an ability the hero lacks is dimmed; the next row goes 30 pixels lower
// Leaf: ?AddHeroAbility@CTCInventoryStats@@QAEXPAVCComponent@NUISystem@@W4EHeroAbility@@AAM@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x005D64DD, 477 bytes, `ret 0Ch`. Frame-pointer size TU: optimize("y",off) + ("s",on)
//   (packed odd address, `push imm ; pop edi`, `leave`), as the landed CTCInventoryStats::InitialiseMannequin 0x005CD767.
// Identity (proven): Ego_r.pdb / FableWin.pdb ?AddHeroAbility@CTCInventoryStats@@QAEXPAVCComponent@NUISystem@@W4EHeroAbility@@AAM@Z;
//   Ego_r donor order ~CTCInventoryStats / AddHeroAbility / SetCreatureAsKilled matches retail 0x005D64DD / 0x005D66BA; three
//   stack arguments (`ret 0Ch`): the parent component (virtual AddChild), the ability (pushed to every ability getter) and the
//   float row position by reference (read for the slider's Y, then += 30).
// Callees: std::bitset<274>::test 0x00410DE0 (out of line in this TU), CVectorMap<ETCInterfaceType,CTCBase*,..>::LowerBound
//   0x0040F020 (non-const) / 0x004365B0 (const), CTCInventoryExperience::GetExperienceAbilityName 0x0056F2BD and
//   GetExperienceAbilityColour 0x0056EDD2, NUISystem::CManager::GetInstance 0x0041E5F2, CTCInventoryAbilities::GetAbilityLevel
//   0x005EDE93 (called twice, the first result unused) and GetAbilityMaximumLevel 0x005EDEA3, CTCHeroExperience::
//   GetAbilityPossibleLevelUpAmount 0x006D7B80, __ftol2 0x00BFEA70, NUISystem::CManager::CreateTickSlider 0x00427216,
//   CCountedPointer<NUISystem::CComponent>::Reset 0x00429C6F and ~CCountedPointer 0x004291DE, the component's virtual
//   GetStates (slot 82, +0x148) and STLport map<unsigned long, NUISystem::CUIState>::operator[] 0x0042D5B1, the parent's
//   virtual AddChild (slot 59, +0xEC), ~CWideString 0x0099B510.
// Interface types (vtable slot 21 GetInterfaceType): CTCInventoryExperience 0x16, CTCHeroExperience 0x68,
//   CTCInventoryAbilities 0x6F. Float pool: 303.0f 0x01255310, 73.0f 0x01255314, 30.0f 0x01248018 (read from .rdata).
// Layout: CTCBase::ParentThing +0x04 (engine/CTCBase.h); CThing UsingComponents +0x20, Components +0x44 (retail);
//   NUISystem::CUIState::Colour +0x18 (alpha byte +0x1B).
// Shape: the three TC lookups are the inline CThing::GetTC / PeekTC templates (the key temporary shares the out pointer's
//   frame word; the abilities lookup is const, hence the const LowerBound); the CCountedPointer local is zeroed at its
//   declaration and filled by Reset; for the missing-ability dimming the slider component is read into a block local, which
//   schedules the state key's zero store after the vtable load as retail does (through `slider->` it lands before the push).
#pragma optimize("y", off)
#pragma optimize("s", on)
#include "engine/CTCBase.h"

namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
template <class _Tp> class allocator { };
template <class _Tp> struct less { };
template <class _Key, class _Tp, class _Compare = less<_Key>, class _Alloc = allocator<pair<const _Key, _Tp> > > class map {
public:
    void* _M_header;
    unsigned long _M_node_count;
    _Compare _M_key_compare;
    _Tp& operator[](const _Key& __k);                         // 0x0042D5B1 for <unsigned long, NUISystem::CUIState>
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
    _Tp* end() { return _M_finish; }
    const _Tp* end() const { return _M_finish; }
};
template <unsigned int _Nb> class bitset {
public:
    unsigned long _M_w[(_Nb + 31) / 32];
    bool test(unsigned int __pos) const;                      // 0x00410DE0 for 274
};
}   // namespace std

class CWideString {
public:
    void* PStringData;
    ~CWideString();                                           // 0x0099B510
};
class C2DCoordI {
public:
    long X;
    long Y;
    C2DCoordI(long x, long y) : X(x), Y(y) { }
};
class CRGBColour {
public:
    unsigned char R, G, B, A;
};
class CRGBFloatColour {
public:
    float R, G, B, A;
    CRGBFloatColour(float r, float g, float b, float a) : R(r), G(g), B(b), A(a) { }
};
template <class T> class CCountedPointer {
public:
    T*    Data;
    void* Info;
    CCountedPointer() : Data(0), Info(0) { }
    ~CCountedPointer();                                       // 0x004291DE
    void Reset(T* data);                                      // 0x00429C6F
    T* operator->() const { return Data; }
};

#define CManager CManagerLayout
#include "engine/CManager.h"
#undef CManager
namespace NUISystem {
class CUIState {
public:
    unsigned long StateChangeFlag;
    long          StateChangeType;
    float         Position[2];
    float         Zoom[2];
    CRGBColour    Colour;                                     // +0x18
    float         UpdateTime;
    bool          LinearChange;
};
class CComponent {
public:
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005();
    virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011();
    virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017();
    virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029();
    virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035();
    virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039(); virtual void v040(); virtual void v041();
    virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047();
    virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053();
    virtual void v054(); virtual void v055(); virtual void v056(); virtual void v057(); virtual void v058();
    virtual void AddChild(CCountedPointer<CComponent>& child);                        // slot 59 (+0xEC)
    virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063(); virtual void v064(); virtual void v065();
    virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069(); virtual void v070(); virtual void v071();
    virtual void v072(); virtual void v073(); virtual void v074(); virtual void v075(); virtual void v076(); virtual void v077();
    virtual void v078(); virtual void v079(); virtual void v080(); virtual void v081();
    virtual std::map<unsigned long, CUIState>& GetStates();                           // slot 82 (+0x148)
};
class CManager {
public:
    static CManager* __fastcall GetInstance();                                        // 0x0041E5F2
    CComponent* CreateTickSlider(const CWideString& name, const C2DCoordI& position, long value, long maximum,
                                 const CRGBFloatColour& colour, float possibleValue, float width, bool a, bool b,
                                 const CRGBFloatColour& backColour, float height);    // 0x00427216
};
}   // namespace NUISystem

enum EHeroAbility { };
enum ETCInterfaceType { };
template <class K, class V> class CKeyPairCompareLess { };
template <class T> class CArray : public std::vector<T> { };
template <class K, class V, class TCompare> class CVectorMap {
public:
    typedef std::pair<K, V> value_type;
    CArray<value_type> Data;                                  // +0x00
    TCompare           Compare;                               // +0x0C
    bool               Dirty;                                 // +0x0D
    value_type* LowerBound(const K& key);                     // 0x0040F020 for the TC map
    const value_type* LowerBound(const K& key) const;         // 0x004365B0
};

#define CTCInventoryExperience CTCInventoryExperienceLayout
#include "engine/CTCInventoryExperience.h"
#undef CTCInventoryExperience
class CTCInventoryExperience : public CTCInventoryExperienceLayout {
public:
    enum { INTERFACE_TYPE = 0x16 };
    CWideString GetExperienceAbilityName(long ability);                               // 0x0056F2BD
    CRGBFloatColour GetExperienceAbilityColour(EHeroAbility ability);                 // 0x0056EDD2
};
#define CTCHeroExperience CTCHeroExperienceLayout
#include "engine/CTCHeroExperience.h"
#undef CTCHeroExperience
class CTCHeroExperience : public CTCHeroExperienceLayout {
public:
    enum { INTERFACE_TYPE = 0x68 };
    long GetAbilityPossibleLevelUpAmount(EHeroAbility ability, float& fraction);     // 0x006D7B80
};
#define CTCInventoryAbilities CTCInventoryAbilitiesLayout
#include "engine/CTCInventoryAbilities.h"
#undef CTCInventoryAbilities
class CTCInventoryAbilities : public CTCInventoryAbilitiesLayout {
public:
    enum { INTERFACE_TYPE = 0x6F };
    long GetAbilityLevel(EHeroAbility ability) const;                                 // 0x005EDE93
    long GetAbilityMaximumLevel(EHeroAbility ability) const;                          // 0x005EDEA3
};

class CThing {
public:
    typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > TCMap;
    unsigned char    _pad00[0x20];
    std::bitset<274> UsingComponents;                         // +0x20
    TCMap            Components;                              // +0x44
    template <class T> __forceinline bool GetTC(T** out)
    {
        if (UsingComponents.test(T::INTERFACE_TYPE)) {
            TCMap* map = &Components;
            TCMap::value_type* it = map->LowerBound((ETCInterfaceType)T::INTERFACE_TYPE);
            if (it == map->Data.end() || T::INTERFACE_TYPE < it->first)
                it = map->Data.end();
            *out = (T*)it->second;
            return true;
        }
        return false;
    }
    template <class T> __forceinline bool PeekTC(const T** out) const
    {
        if (UsingComponents.test(T::INTERFACE_TYPE)) {
            const TCMap* map = &Components;
            const TCMap::value_type* it = map->LowerBound((ETCInterfaceType)T::INTERFACE_TYPE);
            if (it == map->Data.end() || T::INTERFACE_TYPE < it->first)
                it = map->Data.end();
            *out = (const T*)it->second;
            return true;
        }
        return false;
    }
};

class CTCInventoryStats : public CTCBase {
public:
    void AddHeroAbility(NUISystem::CComponent* parent, EHeroAbility ability, float& yPos);
};

void CTCInventoryStats::AddHeroAbility(NUISystem::CComponent* parent, EHeroAbility ability, float& yPos)
{
    CTCInventoryExperience* experience;
    ParentThing->GetTC(&experience);
    CWideString name = experience->GetExperienceAbilityName(ability);
    CTCHeroExperience* heroExperience;
    ParentThing->GetTC(&heroExperience);
    NUISystem::CManager* manager = NUISystem::CManager::GetInstance();
    const CTCInventoryAbilities* abilities;
    ParentThing->PeekTC(&abilities);
    CCountedPointer<NUISystem::CComponent> slider;
    abilities->GetAbilityLevel(ability);
    long level = abilities->GetAbilityLevel(ability);
    float possible = 0.0f;
    long amount = heroExperience->GetAbilityPossibleLevelUpAmount(ability, possible);
    possible += (float)(amount + level + 1);
    slider.Reset(manager->CreateTickSlider(name, C2DCoordI(0, (long)yPos), level + 1,
                                           abilities->GetAbilityMaximumLevel(ability) + 1,
                                           experience->GetExperienceAbilityColour(ability), possible, 303.0f,
                                           false, false, CRGBFloatColour(0.0f, 0.0f, 0.0f, 1.0f), 73.0f));
    if (level == -1) {
        NUISystem::CComponent* component = slider.Data;
        component->GetStates()[0].Colour.A = 100;
    }
    parent->AddChild(slider);
    yPos += 30.0f;
}