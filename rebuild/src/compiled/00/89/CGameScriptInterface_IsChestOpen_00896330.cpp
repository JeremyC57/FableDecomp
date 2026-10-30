// CGameScriptInterface::IsChestOpen(const CScriptThing& chest) const -- script call: whether a valid chest thing's chest component (interface 0x3c) is open
// Leaf: ?IsChestOpen@CGameScriptInterface@@UBE_NABVCScriptThing@@@Z
// Retail: Fable.exe @ 0x00896330, 88 bytes, thiscall `ret 4`.
// Identity (proven): retail CGameScriptInterface vtable 0x01260F0C slot 434 holds this body; slot 434 is
//   ?IsChestOpen@CGameScriptInterface@@UBE_NABVCScriptThing@@@Z. Interface 0x3c: the CTCChest vtable 0x0126957C slot 21 is
//   0x004E48C3 (returns 0x3c; the same body is CTCSearchableContainer's slot 21). Getter: FableWin.pdb
//   ?IsOpen@CTCChest@@QBE_NXZ, inline (byte +0x1c; member name descriptive).
// Body: if chest.IsAlive() (CScriptThing slot 75, +0x12c): t = chest.GetPThing() (slot 11, no null or dead test); if t
//   holds interface 0x3c (UsingComponents word 1 bit 28): PeekTC (const LowerBound 0x004365B0) and return IsOpen().
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
    unsigned char _pad00[0x20];
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
    template <class T> __forceinline bool GetTC(T** out)
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
    template <class T> __forceinline bool PeekTC(const T** out) const
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
class CCPPointerInfo;
template <class T> class CCountedPointer { public: T* Data; CCPPointerInfo* Info; };   // Ego_r.pdb members (Data, Info)
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
class CTCChest {                                        // interface 0x3c
public:
    enum { INTERFACE_TYPE = 0x3c };
    unsigned char _pad00[0x1c];
    bool Opened;                                         // +0x1c (descriptive)
    bool IsOpen() const { return Opened; }
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
bool CGameScriptInterface::IsChestOpen(const CScriptThing& chest) const
{
    if (chest.IsAlive())
    {
        const CThing* t = chest.GetPThing();
        const CTCChest* tc;
        if (t->PeekTC(&tc))
            return tc->IsOpen();
    }
    return false;
}