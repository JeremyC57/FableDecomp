// CGameScriptInterface::IsHeroHandLampLit() const -- script call: whether the main player's hero's hand lamp (CTCLight, interface 0x27) is active; false without a live hero
// Leaf: ?IsHeroHandLampLit@CGameScriptInterface@@UBE_NXZ
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +8.
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
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThingPlayerCreature : public CThing { };
class CPlayer {
public:
    CThingPlayerCreature* GetPControlledCreature() const;    // 0x00487DC0
    const CThingPlayerCreature* PeekPControlledCreature() const;   // 0x00487DD0
};
// CPlayerManager: the shared model engine/CPlayerManager.h (Ego_r.pdb CPlayerManager; the five accessors declared
//   there with their retail addresses: GetPlayer 0x004498C0 / 0x00449910, GetMainPlayer 0x00449970 / 0x00449960,
//   GetMainPlayerNumber 0x00449700).
#include "engine/CPlayerManager.h"
class CTCLight {                                        // interface 0x27
public:
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
        const const CTCLight* light;
        hero->PeekTC(TCI_LIGHT, &light);
        return light->IsActive();
    }
    return false;
}