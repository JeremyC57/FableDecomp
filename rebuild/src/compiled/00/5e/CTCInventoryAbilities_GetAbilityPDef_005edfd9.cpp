// CTCInventoryAbilities::GetAbilityPDef(EHeroAbility ability) const -- the definition of one hero ability: a counted copy of Abilities[ability].second
// Leaf: ?GetAbilityPDef@CTCInventoryAbilities@@QBE?AV?$CDefPointer@$$CBVCHeroAbilityDef@@@@W4EHeroAbility@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x005EDFD9, 30 bytes, thiscall `ret 8` (hidden CDefPointer result + the ability).
// Signature: Ego_r.pdb / FableWin.pdb ?GetAbilityPDef@CTCInventoryAbilities@@QBE?AV?$CDefPointer@$$CBVCHeroAbilityDef@@@@W4EHeroAbility@@@Z
//   (FINDINGS chunk 380: the earlier `PDef** GetAbilityPDef(PDef** out, int index)` model had the wrong signature).
// Layout: struct_layouts_egor CTCInventoryAbilities::Abilities is vector<pair<long, CDefPointer<const CHeroAbilityDef> > >
//   at +0x180 (Dinkumware) -> retail +0x160 (the start pointer read here); 8-byte elements, the CDefPointer at +4. The def
//   keeps its reference count at +4 (behind the vtable). Local model: engine/CTCInventoryAbilities.h has the PDB offsets.
// Shape: the CDefPointer copy constructor into the hidden result (`*result = p ; if (p) ++p->refcount`).
enum EHeroAbility { HERO_ABILITY_NULL };

class CHeroAbilityDef {
public:
    virtual void v00();
    long RefCount;                              // +0x04
};

template <class T> class CDefPointer {
public:
    T* Object;
    CDefPointer(const CDefPointer& other) : Object(other.Object) { if (Object) ++((CHeroAbilityDef*)Object)->RefCount; }
};

template <class T1, class T2> struct SPair {    // std::pair<long, CDefPointer<const CHeroAbilityDef> >
    T1 first;
    T2 second;
};

class CTCInventoryAbilities {
public:
    unsigned char _pad000[0x160];
    SPair<long, CDefPointer<const CHeroAbilityDef> >* AbilitiesStart;   // +0x160 Abilities._M_start
    CDefPointer<const CHeroAbilityDef> GetAbilityPDef(EHeroAbility ability) const;
};

CDefPointer<const CHeroAbilityDef> CTCInventoryAbilities::GetAbilityPDef(EHeroAbility ability) const
{
    return AbilitiesStart[ability].second;
}
