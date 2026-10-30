// CGameScriptInterface::EntityResetAsPureAINarrator(const CScriptThing& thing) const -- for a valid creature thing, set the +0x34 flag of its interface-0xf component
// Leaf: ?EntityResetAsPureAINarrator@CGameScriptInterface@@UBEXABVCScriptThing@@@Z
// Retail: Fable.exe @ 0x0089B0B0, 87 bytes, `ret 4`; virtual (vtable reference 0x012614B8); Ego_r.pdb
// ?EntityResetAsPureAINarrator@CGameScriptInterface@@UBEXABVCScriptThing@@@Z.
// Body: if thing.IsAlive() (slot 75, +0x12c): t = thing.GetPThing() (slot 11, +0x2c); if t is a creature (type byte +0x10
// == 1) and holds interface 0xf (bit 15 of UsingComponents +0x20), the component from the Components map (LowerBound
// 0x0040F020, key in the dead `thing` slot) gets its +0x34 byte = true.
// Layout: CThing as the matched 0x0089D8B0 / 0x0089D7C0 rows plus the type byte +0x10 (the landed 0x0089D830 model:
// 1 = a creature). The component class and the +0x34 member name are not confirmed.
// Shape: /O2 (16-aligned entry); the GetTC out-pointer form.
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:                                    // CThing::Components, CVectorMap<ETCInterfaceType, CTCBase*> (retail 8 bytes)
public:
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry*       LowerBound(const ETCInterfaceType& type);          // 0x0040F020
    const SEntry* LowerBound(const ETCInterfaceType& type) const;    // 0x004365B0
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing {
public:
    unsigned char _pad00[0x10];
    unsigned char Type;                                  // +0x10 (1 = a creature)
    unsigned char _pad11[0x20 - 0x11];
    unsigned long UsingComponents[9];                    // +0x20 bitset<274>
    CInterfaceMap Components;                            // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
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
        CInterfaceMap::SEntry* it = map->LowerBound(type);
        if (it == map->last || T::INTERFACE_TYPE < it->first)
            it = map->last;
        *out = (T*)it->second;
        return true;
    }
    template <class T> __forceinline bool PeekTC(const T** out) const  // ??$PeekTC@$$CBV...@@@CThing@@QBE_NPAPBV...@@@Z
    {
        if (!HasTC(T::INTERFACE_TYPE))
            return false;
        ETCInterfaceType type = (ETCInterfaceType)(T::INTERFACE_TYPE);
        const CInterfaceMap* map = &Components;
        const CInterfaceMap::SEntry* it = map->LowerBound(type);
        if (it == map->last || T::INTERFACE_TYPE < it->first)
            it = map->last;
        *out = (const T*)it->second;
        return true;
    }
};
class CTCInterface0F {                                  // interface 0xf (class not confirmed)
public:
    enum { INTERFACE_TYPE = 0xf };
    unsigned char _pad00[0x34];
    bool          PureAINarrator;                        // +0x34 (name not confirmed)
};
class CCPPointerInfo;
template <class T> class CCountedPointer { public: T* Data; CCPPointerInfo* Info; };   // Ego_r.pdb members (Data, Info)
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
void CGameScriptInterface::EntityResetAsPureAINarrator(const CScriptThing& thing) const
{
    if (thing.IsAlive())
    {
        CThing* t = thing.GetPThing();
        CTCInterface0F* tc;
        if (t->Type == 1 && t->GetTC(&tc))
            tc->PureAINarrator = true;
    }
}