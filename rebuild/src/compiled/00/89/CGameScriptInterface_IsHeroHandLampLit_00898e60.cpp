// CGameScriptInterface::IsHeroHandLampLit() const -- script call: whether the main player's hero's hand lamp (CTCLight, interface 0x27) is active; false without a live hero
// Leaf: ?IsHeroHandLampLit@CGameScriptInterface@@UBE_NXZ
// Retail: Fable.exe @ 0x00898E60, 95 bytes, thiscall `ret`.
// Identity (proven): retail CGameScriptInterface vtable 0x01260F0C slot 185 holds this body; slot 185 is
//   ?IsHeroHandLampLit@CGameScriptInterface@@UBE_NXZ in ghidra_out/cgsi_slots_aligned.tsv; the landed setter
//   SetHeroHandLampAsLit 0x00898EC0 works on the same component. Interface 0x27 = CTCLight (landed
//   CTCLight::GetInterfaceType 0x006423C0); getter FableWin.pdb ?IsActive@CTCLight@@QBE_NXZ, inline (+0x1c).
// Body: PlayerManager (+0x14)->GetMainPlayer() (0x00449970)->PeekPControlledCreature() (0x00487DD0); live hero: PeekTC
//   (const LowerBound 0x004365B0) of interface 0x27 and return IsActive() of the result. Retail reads the light through
//   the uninitialised out-slot when the component is absent (`mov eax,[esp] ; mov al,[eax+1Ch]`): the source ignores
//   PeekTC's result (GOTCHAS chunk 141: an unset local read on the absent path).
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
class CThingPlayerCreature : public CThing { };
class CPlayer {
public:
    CThingPlayerCreature* GetPControlledCreature() const;    // 0x00487DC0
    const CThingPlayerCreature* PeekPControlledCreature() const;   // 0x00487DD0
};
class CPlayerManager {
public:
    CPlayer& GetMainPlayer();                            // 0x00449970
};
class CTCLight {                                        // interface 0x27
public:
    enum { INTERFACE_TYPE = 0x27 };
    unsigned char _pad00[0x1c];
    bool Active;                                         // +0x1c
    bool IsActive() const { return Active; }
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
bool CGameScriptInterface::IsHeroHandLampLit() const
{
    const CThingPlayerCreature* hero = PlayerManager->GetMainPlayer().PeekPControlledCreature();
    if (hero && !hero->IsDead())
    {
        const CTCLight* light;
        hero->PeekTC(&light);
        return light->IsActive();
    }
    return false;
}