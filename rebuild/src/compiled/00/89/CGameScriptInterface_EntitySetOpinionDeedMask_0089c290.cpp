// CGameScriptInterface::EntitySetOpinionDeedMask(const CScriptThing& thing, long mask) const -- when the script thing's thing exists and has a CTCCreatureOpinionOfHero (interface 0x4F), pass the mask to its SetOpinionDeedMask
// Leaf: ?EntitySetOpinionDeedMask@CGameScriptInterface@@UBEXABVCScriptThing@@J@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0089c290, 77 bytes, thiscall, `ret 8`.
// Identity (proven): vtable 0x01260F0C (RTTI .?AVCGameScriptInterface@@) slot 614; ghidra_out/cgsi_slots_aligned.tsv (Ego_r.pdb vtable order) names it ?EntitySetOpinionDeedMask@CGameScriptInterface@@UBEXABVCScriptThing@@J@Z; its callee 0x006821E0 is bound ?SetOpinionDeedMask@CTCCreatureOpinionOfHero@@QAEXJ@Z (Ego_r.pdb); CTCCreatureOpinionOfHero::GetInterfaceType() is 79 (0x4F, landed).
// Callees: CScriptThing virtual slot 11 (+0x2C, GetThing), CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<...> >::
//   LowerBound(const ETCInterfaceType&) 0x0040F020, CTCCreatureOpinionOfHero::SetOpinionDeedMask(long) 0x006821E0.
// Body: `CThing* t = thing.GetThing(); if (t && t->GetInterface(&opinion, 0x4F)) opinion->SetOpinionDeedMask(mask);` (the inlined GetInterface: the component lands in eax, then ecx) -- the
//   presence test (bit 15 of bitset word 2) (`test ch,ch ; jns`), the lookup key homed in the dead thing-argument slot.
// Header debt: engine/CScriptThing.h, CThing and engine/CTCCreatureOpinionOfHero.h are flat layouts; modelled locally.
enum ETCInterfaceType { ETCInterfaceType_FIRST };
class CTCBase;
namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
}
template <class K, class V> class CKeyPairCompareLess { };
template <class K, class V, class TCompare> class CVectorMap {   // retail 0x10 bytes
public:
    typedef std::pair<K, V> value_type;
    value_type* First;                                     // +0x00 (CArray: STLport vector)
    value_type* Last;                                      // +0x04
    value_type* EndOfStorage;                              // +0x08
    TCompare    Compare;                                   // +0x0C
    bool        Dirty;                                     // +0x0D
    value_type* end() { return Last; }
    value_type* LowerBound(const K& key);                  // 0x0040F020 for the interface map
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing {
public:
    virtual ~CThing();
    unsigned char _pad04[0x1C];
    unsigned long PresentInterfaces[9];                    // +0x20 (the bitset of present interface types)
    CInterfaceMap Interfaces;                              // +0x44
    template <class T> __forceinline bool GetInterface(T** out, ETCInterfaceType type)
    {
        if (PresentInterfaces[type >> 5] & (1UL << (type & 31))) {
            ETCInterfaceType key = type;
            CInterfaceMap* map = &Interfaces;
            CInterfaceMap::value_type* it = map->LowerBound(key);
            if (it == map->end() || type < it->first)
                it = map->end();
            *out = (T*)it->second;
            return true;
        }
        return false;
    }
};
class CScriptThing {
public:
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005();
    virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009(); virtual void v010();
    virtual CThing* GetThing() const;                      // slot 11 (+0x2C)
    virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017();
    virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029();
    virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035();
    virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039(); virtual void v040(); virtual void v041();
    virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047();
    virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053();
    virtual void v054(); virtual void v055(); virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059();
    virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063(); virtual void v064(); virtual void v065();
    virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069(); virtual void v070(); virtual void v071();
    virtual void v072(); virtual void v073(); virtual void v074();
    virtual bool IsValid() const;                          // slot 75 (+0x12C)
};
class CCharString;
class CTCCreatureOpinionOfHero {
public:
    void SetOpinionDeedMask(long mask);                             // 0x006821E0
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
void CGameScriptInterface::EntitySetOpinionDeedMask(const CScriptThing& thing, long mask) const
{
    CThing* t = thing.GetThing();
    CTCCreatureOpinionOfHero* opinion;
    if (t && t->GetInterface(&opinion, (ETCInterfaceType)0x4F))
        opinion->SetOpinionDeedMask(mask);
}