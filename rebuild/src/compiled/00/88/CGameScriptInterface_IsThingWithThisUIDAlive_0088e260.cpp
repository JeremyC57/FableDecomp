// CGameScriptInterface::IsThingWithThisUIDAlive(unsigned __int64 uid) const -- script call: whether the thing with this unique id exists and is not dead
// Leaf: ?IsThingWithThisUIDAlive@CGameScriptInterface@@UBE_N_K@Z
// Retail: Fable.exe @ 0x0088E260, 52 bytes, thiscall `ret 8`.
// Identity (proven): retail CGameScriptInterface vtable 0x01260F0C slot 86 holds this body; slot 86 is
//   ?IsThingWithThisUIDAlive@CGameScriptInterface@@UBE_N_K@Z (ghidra_out/cgsi_slots_aligned.tsv).
// Body: World (+0x04) virtual slot 3 (+0x0c, CWorld vtable 0x012390F0 slot 3 = 0x0049E1B0, manifest
//   CWorld::GetThingManager; Ego_r.pdb ?GetThingManager@CWorld@@UAEAAVCThingManager@@XZ precedes PeekThingManager, as
//   slot 3 precedes slot 4) -> GetPThingWithUID(uid) (0x0051ED80); true when the thing exists and CThing::IsAlive() (inline !Dead, +0x91 bit 0;
//   FableWin.pdb ?IsAlive@CThing@@QBE_NXZ) holds. A plain `!IsDead()` gives `test byte [eax+91h],1`; retail's
//   `mov cl,[eax+91h] ; not cl ; test cl,1` is the inline IsAlive value.
// Callee 0x0051ED80: byte-identical twins 0x0051ED60 / 0x0051ED80; Ego_r.pdb and FableWin.pdb both list
//   PeekPThingWithUID before GetPThingWithUID, as retail orders the twins, so 0x0051ED80 is
//   ?GetPThingWithUID@CThingManager@@QAEPAVCThing@@_K@Z (41 call sites against 5 for 0x0051ED60; the manifest's
//   "CStopWatch::GetTicks" label is wrong).
class CThing {
public:
    unsigned char _pad00[0x91];
    unsigned char Dead : 1;                              // +0x91 bit 0
    bool IsAlive() const { return !Dead; }                // inline: `not cl ; test cl,1` (a !Dead value, not a bit test)
};
class CThingManager {
public:
    CThing* GetPThingWithUID(unsigned __int64 uid);      // 0x0051ED80
};
class CWorld {
public:
    virtual void v00(); virtual void v01(); virtual void v02();
    virtual CThingManager& GetThingManager();            // slot 3 (+0x0c)
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
bool CGameScriptInterface::IsThingWithThisUIDAlive(unsigned __int64 uid) const
{
    CThing* t = World->GetThingManager().GetPThingWithUID(uid);
    if (t && t->IsAlive())
        return true;
    return false;
}