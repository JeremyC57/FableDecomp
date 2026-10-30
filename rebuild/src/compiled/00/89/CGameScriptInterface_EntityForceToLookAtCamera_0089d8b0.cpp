// CGameScriptInterface::EntityForceToLookAtCamera(const CScriptThing& thing, bool look) const -- set the valid thing's targeting component (interface 8) camera-look flag
// Leaf: ?EntityForceToLookAtCamera@CGameScriptInterface@@UBEXABVCScriptThing@@_N@Z
// Retail: Fable.exe @ 0x0089D8B0, 85 bytes, `ret 8`; virtual (vtable reference 0x012616D8); Ego_r.pdb
// ?EntityForceToLookAtCamera@CGameScriptInterface@@UBEXABVCScriptThing@@_N@Z.
// Body: if thing.IsAlive() (slot 75, +0x12c): t = thing.GetPThing() (slot 11, +0x2c); if t holds interface 8 (bit 8 of
// UsingComponents +0x20), the component from the Components map (LowerBound 0x0040F020, key in the dead `thing` slot)
// gets its +0x34 byte = look.
// Layout: as the landed 0x0089D830 EntityForceToLookAtThing sibling (same slots, interface 8 = the targeting component)
// and the 0x0089D7C0 row's CThing model (UsingComponents +0x20, Components +0x44). The +0x34 member name is not confirmed.
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
class CTCTargetingBase {
public:
    enum { INTERFACE_TYPE = 8 };
    unsigned char _pad00[0x34];
    bool          LookAtCamera;                          // +0x34 (name not confirmed)
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
void CGameScriptInterface::EntityForceToLookAtCamera(const CScriptThing& looker, bool look) const
{
    if (looker.IsAlive())
    {
        CTCTargetingBase* tc;
        if (looker.GetPThing()->GetTC(&tc))
            tc->LookAtCamera = look;
    }
}