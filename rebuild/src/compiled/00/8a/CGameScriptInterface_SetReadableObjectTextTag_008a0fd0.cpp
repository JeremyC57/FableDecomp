// CGameScriptInterface::SetReadableObjectTextTag(const CScriptThing& thing, const CCharString& tag) const -- give a live readable object (interface 0xd) a new text tag
// Leaf: ?SetReadableObjectTextTag@CGameScriptInterface@@UBEXABVCScriptThing@@ABVCCharString@@@Z
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +8.
// Retail: Fable.exe @ 0x008A0FD0, 107 bytes, `ret 8`; Ego_r.pdb same symbol.
// Body: t = thing.GetPThing() (slot 11); if t, not dead (bit 0 of +0x91) and holding interface 0xd (bit 13 of
// UsingComponents word 0, +0x20): r = GetTC (non-const LowerBound 0x0040F020, key in the dead `thing` slot; no null
// test on r); r->SetTextTag(tag.ToWideString()) -- ToWideString 0x0099E480 (labelled GetDefaultVal<CAnimationSet>, a
// folded body), CWideString::operator= 0x0099B7D0 on TextTag +0x18, temp dtor 0x0099B510 (labelled ~CCharString, folded).
// Layout: CThing as the matched 0x0089A330 / 0x0089B0B0 rows. CTCActionUseReadable: Ego_r.pdb TextTag (CWideString) +0x18,
// inline ?SetTextTag@CTCActionUseReadable@@QAEXABVCWideString@@@Z. Interface 0xd = CTCActionUseReadable is
// inferred from that +0x18 store, not confirmed.
// Shape: /O2 (16-aligned entry); the GetTC out-pointer form.
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing {
public:
    unsigned char _pad00[0x10];
    unsigned char Type;                                  // +0x10 (1 = a creature)
    unsigned char _pad11[0x20 - 0x11];
    unsigned long UsingComponents[9];                    // +0x20 bitset<274>
    CInterfaceMap Components;                            // +0x44
    unsigned char _pad54[0x3d];
    unsigned char Dead : 1;                              // +0x91 bit 0
    bool IsDead() const { return Dead; }
    bool HasTC(long type) const
    {
        if (UsingComponents[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    template <class T> __forceinline bool GetTC(T** out)               // ??$GetTC@V...@@@CThing@@QAE_NPAPAV...@@@Z
    {
        if (!HasTC(T::INTERFACE_TYPE))
            return false;
        ETCInterfaceType type = (ETCInterfaceType)(T::INTERFACE_TYPE);
        CInterfaceMap* map = &Components;
        CInterfaceMap::value_type* it = map->LowerBound(type);
        if (it == map->end() || T::INTERFACE_TYPE < it->first)
            it = map->end();
        *out = (T*)it->second;
        return true;
    }
    template <class T> __forceinline bool PeekTC(const T** out) const  // ??$PeekTC@$$CBV...@@@CThing@@QBE_NPAPBV...@@@Z
    {
        if (!HasTC(T::INTERFACE_TYPE))
            return false;
        ETCInterfaceType type = (ETCInterfaceType)(T::INTERFACE_TYPE);
        const CInterfaceMap* map = &Components;
        const CInterfaceMap::value_type* it = map->LowerBound(type);
        if (it == map->end() || T::INTERFACE_TYPE < it->first)
            it = map->end();
        *out = (const T*)it->second;
        return true;
    }
};
class CWideString {
public:
    void* m_pData;
    const CWideString& operator=(const CWideString& other);    // 0x0099B7D0 (returns const& in the PDB)
    ~CWideString();                                      // 0x0099B510
};
class CCharString {
public:
    void* m_pData;
    CWideString ToWideString() const;                    // 0x0099E480
};
class CTCActionUseReadable {                             // interface 0xd
public:
    enum { INTERFACE_TYPE = 0xd };
    unsigned char _pad00[0x18];
    CWideString TextTag;                                 // +0x18
    void SetTextTag(const CWideString& tag) { TextTag = tag; }
};
#include "engine/CCountedPointer.h"
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
void CGameScriptInterface::SetReadableObjectTextTag(const CScriptThing& thing, const CCharString& tag) const
{
    CThing* t = thing.GetPThing();
    if (t && !t->IsDead())
    {
        CTCActionUseReadable* readable;
        if (t->GetTC(&readable))
            readable->SetTextTag(tag.ToWideString());
    }
}