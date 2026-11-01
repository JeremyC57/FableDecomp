// CArmourDef::Transfer(CPersistContext& context) -- persist the armour definition: the augmentation / damage-type response and armour-threshold maps, the three all-hits flags and the damage / block materials
// Leaf: ?Transfer@CArmourDef@@UAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004615FB, 101 bytes, thiscall, `ret 4` (unaligned entry, def module).
// Identity (proven): slot 18 of the CArmourDef vtable 0x01234AEC (rebuild/manifest/rtti-vtables.tsv; slot 19 is the Copy
//   thunk 0x0046133F of the landed CArmourDef::operator= 0x00461344, slot 0 the landed ??_G 0x004585A1); Ego_r.pdb and
//   FableWin.pdb ?Transfer@CArmourDef@@UAEXAAVCPersistContext@@@Z. The transferred members are Ego_r.pdb's CArmourDef fields at
//   their retail offsets (CParentDefClassBase 0x39 packed, maps 0xC in STLport): AugmentationResponse +0x3c,
//   DamageTypeResponse +0x48, ArmourThresholds +0x54, AllHitsKnockdown/AllHitsNegated/AllHitsCauseRecoil +0x60..+0x62,
//   DamageMaterial +0x64, BlockMaterial +0x68 (the landed operator= 0x00461344 copies the same offsets).
// Callees: CPersistContext::TransferMap 0x00461660 (map<EObjectAugmentationType,float>: CheckCRC, then
//   GFSerialiseMapBinaryOut / landed GFSerialiseMapBinaryIn 0x004616D6) and 0x00461894 (map<EDamageAttribute,float>, x2;
//   its In half is the landed 0x0046190A); CPersistContext::Transfer<bool>(bool&) 0x0043314A (x3), Transfer<long>(long&)
//   0x00431102 (x2). The one-argument TransferMap spelling is descriptive: the PDBs only hold the named
//   TransferMap<V,K>(const char*, map&[, const V&]) forms (as for the landed one-argument Transfer<T>(T&) rows).
// Shape: the def-module family (CMultiStaticMeshEntryDef::Transfer 0x004EB8C3): one transfer per member in declaration order,
//   no base Transfer call (pure in CDefClassBase), context in edi, size TU.
#pragma optimize("s", on)
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CParentDefClassBase.h"
namespace std {
template <class _Tp> struct less { };
template <class _Tp> class allocator { public: };
template <class _T1, class _T2> struct pair { _T1 first; _T2 second; };
template <class _Tp, class _Alloc> class _STLP_alloc_proxy : public _Alloc { public:
    _Tp _M_data;
};
struct _Rb_tree_node_base {
    bool                _M_color;
    _Rb_tree_node_base* _M_parent;
    _Rb_tree_node_base* _M_left;
    _Rb_tree_node_base* _M_right;
};
template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base { _Value _M_value_field; };
template <class _Tp, class _Alloc> class _Rb_tree_base { public:
    _STLP_alloc_proxy<_Rb_tree_node<_Tp>*, allocator<_Rb_tree_node<_Tp> > > _M_header;   // +0x0
};
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
class _Rb_tree : public _Rb_tree_base<_Value, _Alloc> { public:
    unsigned int _M_node_count;                             // +0x4
    _Compare     _M_key_compare;                            // +0x8
};
template <class _Pair> struct _Select1st { };
template <class _Key, class _Tp, class _Compare = less<_Key>, class _Alloc = allocator<pair<const _Key, _Tp> > >
class map { public:
    _Rb_tree<_Key, pair<const _Key, _Tp>, _Select1st<pair<const _Key, _Tp> >, _Compare, _Alloc> _M_t;   // 0xC
};
} // namespace std
enum EObjectAugmentationType { };                        // global enums (Ego_r W4...@@); values do not reach the bytes
enum EDamageAttribute { };
class CPersistContext {
public:
    template <class T> void Transfer(T& value);                                   // bool 0x0043314A, long 0x00431102
    template <class V, class K> void TransferMap(std::map<K, V>& value);          // 0x00461660 / 0x00461894
};
class CArmourDef : public CParentDefClassBase {                                   // vtable 0x01234AEC
public:
    virtual void Transfer(CPersistContext& context);                              // slot 18
    virtual void Copy(const CDefClassBase*);                                      // slot 19 0x0046133F
    virtual unsigned long GetSizeofClass();                                       // slot 20 0x0045859D
    std::map<EObjectAugmentationType, float> AugmentationResponse;                // +0x3c
    std::map<EDamageAttribute, float>        DamageTypeResponse;                  // +0x48
    std::map<EDamageAttribute, float>        ArmourThresholds;                    // +0x54
    bool AllHitsKnockdown;                                                        // +0x60
    bool AllHitsNegated;                                                          // +0x61
    bool AllHitsCauseRecoil;                                                      // +0x62
    long DamageMaterial;                                                          // +0x64
    long BlockMaterial;                                                           // +0x68
};
void CArmourDef::Transfer(CPersistContext& context)
{
    context.TransferMap(AugmentationResponse);
    context.TransferMap(DamageTypeResponse);
    context.TransferMap(ArmourThresholds);
    context.Transfer(AllHitsKnockdown);
    context.Transfer(AllHitsNegated);
    context.Transfer(AllHitsCauseRecoil);
    context.Transfer(DamageMaterial);
    context.Transfer(BlockMaterial);
}