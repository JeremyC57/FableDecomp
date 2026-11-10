// CTCInventoryClothing::ResetMannequin() -- when there is a mannequin, dress it like the hero: copy the hero's worn clothing appearance modifiers (type 2) onto the mannequin's, replacing whatever it wore
// Leaf: ?ResetMannequin@CTCInventoryClothing@@QAEXXZ
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: the empty local std::bitset stand-in is the STLport bitset<274> (36 bytes) of stlport/fable_containers.h, test() declared only under FABLE_RETAIL_STLP_BITSET_TEST_DECL (retail calls 0x00410DE0); bitset pad _pad21[0x23] removed.
// Naming wave 18: the interface-presence stand-in CPresenceBits is std::bitset<274> (STLport 4.6); its test is ?test@?$bitset@$0BBC@@std@@QBE_NI@Z (name-only).
// Retail: Fable.exe @ 0x005B425B, 209 bytes, __fastcall `ret`. Ego_r.pdb `?ResetMannequin@CTCInventoryClothing@@QAEXXZ`
// (name inferred: the only argument-less CTCInventoryClothing member that fits -- it reads PMannequin and rebuilds its
// clothing from the hero's; the landed InitialiseMannequin 0x005B37F7 creates PMannequin at the same +0x158).
// Callees: std::bitset::test @ 0x00410DE0 and CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound @ 0x0040F020 (the inlined
// CThing::GetInterface for interface 0x5E = CTCHeroAttachableAppearanceModifiers, on the owner and on the mannequin),
// list<long>::list(const allocator&) @ 0x0042AB58, CTCHeroAttachableAppearanceModifiers::GetAppearanceModifierList
// (list<long>&, long) const @ 0x00706800, ::RemoveAllAppearanceModifiers(long) @ 0x00706640,
// ::AddAppearanceModifierWithReplace(long) @ 0x00706950, ~list<long> @ 0x0042AB73 (models from the landed
// AddClothingToThing 0x005B43A2).
// Layout: retail CTCInventoryClothing (12-byte containers): ParentThing +0x04, PMannequin +0x158 (PDB +0x17C).
// Shape: frame-pointer size TU; early returns keep the list in the function block, so its frame word ([ebp-8]) is not
// packed with the inlined GetInterface key ([ebp-0Ch]) -- the nested-if form packs them into one word (frame 0Ch v 10h,
// a temporary key packs the same way); `push 5Eh ; pop edi` serves both lookups' test and key; the hero's component is
// spilled to [ebp-10h], the mannequin's lives in edi; the list's allocator temporary is the frame byte [ebp-1] (the
// list constructor's same-TU body).
// Name fix (agentNF 2026-11-10): `worn` is the shared STLport 4.6 list<long> (was a local CLongList); _List_base ctor 0x0042AB58 / dtor
//   0x0042AB73 and GetAppearanceModifierList 0x00706800 bound under their real spellings; the ctor keeps its same-TU body (STLport
//   _List_base body as an explicit specialisation).
// Label: the catalog's Appearance_UpdateModifiers is this method.
#pragma optimize("y", off)
#pragma optimize("s", on)
#define FABLE_RETAIL_STLP_LIST_BASE_OUTOFLINE
#define FABLE_RETAIL_STLP_BITSET_TEST_DECL
#include "stlport/fable_containers.h"   // list<long>: _List_base ctor 0x0042AB58 / dtor 0x0042AB73 out of line
enum EHeroAttachableAppearanceModifierType { };      // Ego_r enum (value 2 used here)
class CTCHeroAttachableAppearanceModifiers { public:           // interface 0x5E
    void GetAppearanceModifierList(std::list<long>& out, EHeroAttachableAppearanceModifierType type) const;   // 0x00706800
    void RemoveAllAppearanceModifiers(long type);       // 0x00706640
    void AddAppearanceModifierWithReplace(long def);    // 0x00706950
};
#include "engine/CVectorMap.h"
class CTCBase;
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CTCInventoryClothing { public:
    void*   __vftable;
    CThing* ParentThing;                                // +0x04
    unsigned char _pad08[0x158 - 0x08];
    CThing* PMannequin;                                 // +0x158
    void ResetMannequin();
};
// The list constructor's same-TU out-of-line body (it builds only the sentinel), as in the landed AddClothingToThing.
#pragma auto_inline(off)
template<> std::_List_base<long, std::allocator<long> >::_List_base(const std::allocator<long>&)
    : _M_node(std::allocator<std::_List_node<long> >(), 0)
{
    std::_List_node<long>* n = _M_node.allocate(1);
    n->_M_next = n;
    n->_M_prev = n;
    _M_node._M_data = n;
}
#pragma auto_inline(on)
void CTCInventoryClothing::ResetMannequin()
{
    if (!PMannequin)
        return;
    CTCHeroAttachableAppearanceModifiers* heroModifiers;
    if (!ParentThing->GetTC(TCI_HERO_ATTACHABLE_APPEARANCE_MODIFIERS, &heroModifiers))
        return;
    CTCHeroAttachableAppearanceModifiers* mannequinModifiers;
    if (!PMannequin->GetTC(TCI_HERO_ATTACHABLE_APPEARANCE_MODIFIERS, &mannequinModifiers))
        return;
    std::list<long> worn;
    heroModifiers->GetAppearanceModifierList(worn, (EHeroAttachableAppearanceModifierType)2);
    mannequinModifiers->RemoveAllAppearanceModifiers(2);
    for (std::list<long>::iterator node = worn.begin(); node != worn.end(); ++node)
        mannequinModifiers->AddAppearanceModifierWithReplace(*node);
}