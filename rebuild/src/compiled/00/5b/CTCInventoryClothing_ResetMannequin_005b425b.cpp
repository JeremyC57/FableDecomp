// CTCInventoryClothing::ResetMannequin() -- when there is a mannequin, dress it like the hero: copy the hero's worn clothing appearance modifiers (type 2) onto the mannequin's, replacing whatever it wore
// Leaf: ?ResetMannequin@CTCInventoryClothing@@
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
// Label: the catalog's Appearance_UpdateModifiers is this method.
#pragma optimize("y", off)
#pragma optimize("s", on)
struct SAllocator { SAllocator() { } };
struct SLongListNode { SLongListNode* _M_next; SLongListNode* _M_prev; long _M_data; };
struct CLongList {                                      // STLport list<long>: the sentinel pointer
    SLongListNode* _M_node;
    explicit CLongList(const SAllocator& allocator = SAllocator());   // 0x0042AB58
    ~CLongList();                                       // 0x0042AB73
};
class CTCHeroAttachableAppearanceModifiers { public:           // interface 0x5E
    void GetAppearanceModifierList(CLongList& out, long type) const;   // 0x00706800
    void RemoveAllAppearanceModifiers(long type);       // 0x00706640
    void AddAppearanceModifierWithReplace(long def);    // 0x00706950
};
struct CPresenceBits { bool test(long bit); };          // 0x00410DE0
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);               // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing { public:
    unsigned char  _pad00[0x20];
    CPresenceBits  m_present;                           // +0x20
    unsigned char  _pad21[0x23];
    CInterfaceMap  m_interfaces;                        // +0x44
    template<class T> __forceinline bool GetInterface(T** out, long wanted)
    {
        if (m_present.test(wanted)) {
            ETCInterfaceType type = (ETCInterfaceType)(wanted);
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(type);
            *out = (T*)((void*)((it == map->last || wanted < it->first) ? map->last : it)->second);
            return true;
        }
        return false;
    }
};
class CTCInventoryClothing { public:
    void*   __vftable;
    CThing* ParentThing;                                // +0x04
    unsigned char _pad08[0x158 - 0x08];
    CThing* PMannequin;                                 // +0x158
    void ResetMannequin();
};
// The list constructor's same-TU out-of-line body (it builds only the sentinel), as in the landed AddClothingToThing.
#pragma auto_inline(off)
CLongList::CLongList(const SAllocator&)
{
    _M_node = (SLongListNode*)operator new(sizeof(SLongListNode));
    _M_node->_M_next = _M_node;
    _M_node->_M_prev = _M_node;
}
#pragma auto_inline(on)
void CTCInventoryClothing::ResetMannequin()
{
    if (!PMannequin)
        return;
    CTCHeroAttachableAppearanceModifiers* heroModifiers;
    if (!ParentThing->GetInterface(&heroModifiers, 0x5e))
        return;
    CTCHeroAttachableAppearanceModifiers* mannequinModifiers;
    if (!PMannequin->GetInterface(&mannequinModifiers, 0x5e))
        return;
    CLongList worn;
    heroModifiers->GetAppearanceModifierList(worn, 2);
    mannequinModifiers->RemoveAllAppearanceModifiers(2);
    for (SLongListNode* node = worn._M_node->_M_next; node != worn._M_node; node = node->_M_next)
        mannequinModifiers->AddAppearanceModifierWithReplace(node->_M_data);
}