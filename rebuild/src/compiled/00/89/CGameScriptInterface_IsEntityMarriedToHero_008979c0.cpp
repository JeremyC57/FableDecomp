// CGameScriptInterface::IsEntityMarriedToHero(const CScriptThing& thing) const -- a live creature whose wife component (interface 0xd7) answers IsMarriedToThePlayer()
// Leaf: ?IsEntityMarriedToHero@CGameScriptInterface@@UBE_NABVCScriptThing@@@Z
// Retail: Fable.exe @ 0x008979C0, 101 bytes, `ret 4`; Ego_r.pdb ?IsEntityMarriedToHero@CGameScriptInterface@@UBE_NABVCScriptThing@@@Z.
// Body: t = thing.GetPThing() (slot 11, +0x2c); if t, not dead (bit 0 of +0x91), a creature (type byte +0x10 == 1) and
// holding interface 0xd7 (bit 23 of UsingComponents word 6, +0x38), the component from the const Components map
// (LowerBound 0x004365B0, key in the dead `thing` slot) answers IsMarriedToThePlayer() (0x006CEC80, compares a +0x20 field against a 0x0049D870 value); else false.
// The byte twin of 0x00897A30 IsEntityMarriable (only the callee differs).
// Layout: CThing as the matched 0x0089A330 / 0x0089B0B0 rows. Interface 0xd7 is modelled as CTCWife (Ego_r.pdb
// ?IsMarriedToThePlayer@CTCWife@@QBE_NXZ / ?IsWillingToMarry@CTCWife@@QBE_NXZ); the interface number is not confirmed.
// Shape: /O2 (16-aligned entry); the PeekTC out-pointer form.
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
class CTCWife {                                         // interface 0xd7
public:
    enum { INTERFACE_TYPE = 0xd7 };
    bool IsMarriedToThePlayer() const;                   // 0x006CEC80
    bool IsWillingToMarry() const;                       // 0x006CFEC0
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
bool CGameScriptInterface::IsEntityMarriedToHero(const CScriptThing& thing) const
{
    const CThing* t = thing.GetPThing();
    if (t && !t->IsDead() && t->Type == 1)
    {
        const CTCWife* wife;
        if (t->PeekTC(&wife) && wife->IsMarriedToThePlayer())
            return true;
    }
    return false;
}