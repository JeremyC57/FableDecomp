// NScript::CV_RockTrollFirstEncounterScript::OnPersist(CPersistContext& context) -- saves/loads Activated, RockTrollTriggered, PlayedExhumeCutScene, AddedItemsToRockTroll, RockTrollHealthBarID by name (default false / 0)
// Leaf: ?OnPersist@CV_RockTrollFirstEncounterScript@NScript@@UAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00ec4970, 144 bytes, thiscall, `ret 4`.
// Identity (proven): slot 4 (OnPersist) of the vtable 0x012F15C4 that the landed constructor 0x00EC3C20 stores; the five literals name Ego_r.pdb CV_RockTrollFirstEncounterScript members (Activated, RockTrollTriggered, PlayedExhumeCutScene, AddedItemsToRockTroll, RockTrollHealthBarID); the PDB name sets hold its OnPersist.
// Callees: CPersistContext::Transfer<bool>(const char*, bool&, const bool&) 0x004045C0, CPersistContext::Transfer<long>(const char*, long&, const long&) 0x00410BE0 (FableWin.pdb template names, bound).
// Shape (family L144 = the landed entity OnPersist 0x00E641B0's model): each default is a temporary in a dead stack slot;
//   the member offsets are retail's.
class CPersistContext {
public:
    template <class T> void Transfer(const char* name, T& value, const T& defaultValue);
};
// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptBase.h"
namespace NScript {
class CV_RockTrollFirstEncounterScript : public CScriptBase {
public:
    virtual void OnPersist(CPersistContext& context);
    void* m_pInterface;                                 // +0x40
    void* m_pOwner;                                     // +0x44
    unsigned char _pad48[0x4d - 0x48];
    bool           Activated;                               // +0x4d
    bool           RockTrollTriggered;                      // +0x4e
    unsigned char _pad4f[0x50 - 0x4f];
    long           RockTrollHealthBarID;                    // +0x50
    bool           PlayedExhumeCutScene;                    // +0x54
    bool           AddedItemsToRockTroll;                   // +0x55
};
void CV_RockTrollFirstEncounterScript::OnPersist(CPersistContext& context)
{
    context.Transfer("Activated", Activated, false);
    context.Transfer("RockTrollTriggered", RockTrollTriggered, false);
    context.Transfer("PlayedExhumeCutScene", PlayedExhumeCutScene, false);
    context.Transfer("AddedItemsToRockTroll", AddedItemsToRockTroll, false);
    context.Transfer("RockTrollHealthBarID", RockTrollHealthBarID, 0L);
}
}