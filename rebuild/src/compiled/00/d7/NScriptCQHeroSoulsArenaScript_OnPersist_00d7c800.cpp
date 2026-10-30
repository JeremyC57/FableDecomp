// NScript::CQ_HeroSoulsArenaScript::OnPersist(CPersistContext& context) -- saves/loads CreatureCount, PlayedArenaCS, PlayedHoHCS, JackTauntNearHOH, ArenaLevelReached by name (default false / 0)
// Leaf: ?OnPersist@CQ_HeroSoulsArenaScript@NScript@@UAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00d7c800, 144 bytes, thiscall, `ret 4`.
// Identity (proven): slot 4 (OnPersist) of the vtable 0x012D3994 that the landed constructor 0x00D7C700 (catalogued under the stand-in CVtable_012D3994) stores; the five literals name Ego_r.pdb CQ_HeroSoulsArenaScript members (CreatureCount, PlayedArenaCS, PlayedHoHCS, JackTauntNearHOH, ArenaLevelReached); the PDB name sets hold its OnPersist.
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
class CQ_HeroSoulsArenaScript : public CScriptBase {
public:
    virtual void OnPersist(CPersistContext& context);
    void* m_pInterface;                                 // +0x40
    void* m_pOwner;                                     // +0x44
    long           CreatureCount;                           // +0x48
    bool           PlayedArenaCS;                           // +0x4c
    unsigned char _pad4d[0x50 - 0x4d];
    bool           JackTauntNearHOH;                        // +0x50
    unsigned char _pad51[0x52 - 0x51];
    bool           PlayedHoHCS;                             // +0x52
    unsigned char _pad53[0x54 - 0x53];
    long           ArenaLevelReached;                       // +0x54
};
void CQ_HeroSoulsArenaScript::OnPersist(CPersistContext& context)
{
    context.Transfer("CreatureCount", CreatureCount, 0L);
    context.Transfer("PlayedArenaCS", PlayedArenaCS, false);
    context.Transfer("PlayedHoHCS", PlayedHoHCS, false);
    context.Transfer("JackTauntNearHOH", JackTauntNearHOH, false);
    context.Transfer("ArenaLevelReached", ArenaLevelReached, 0L);
}
}