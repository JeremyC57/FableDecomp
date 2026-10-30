// NScript::CV_FisticuffsClubScript::OnPersist(CPersistContext& context) -- saves/loads the script members CompletedKnotholePunchClub, CompletedBowerstonePunchClub, CompletedBanditPunchClub, CompletedOakvalePunchClub by name (default false / 0)
// Leaf: ?OnPersist@CV_FisticuffsClubScript@NScript@@UAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00e82190, 131 bytes, thiscall, `ret 4`.
// Identity (proven): slot 4 (OnPersist) of the script vtable 0x012EAF14 (the landed constructor 0x00E81F60); the four literals name Ego_r.pdb CV_FisticuffsClubScript members (CompletedKnotholePunchClub, CompletedBowerstonePunchClub, CompletedBanditPunchClub, CompletedOakvalePunchClub); the PDB name sets hold ?OnPersist@CV_FisticuffsClubScript@NScript@@UAEXAAVCPersistContext@@@Z.
// Callees: CPersistContext::Transfer<bool>(const char*, bool&, const bool&) 0x004045C0 (FableWin.pdb template names, bound).
// Shape (family L141 = the landed entity OnPersist 0x00E641B0 with four members): each default is a temporary in a dead
//   stack slot; the member offsets are retail's (the script base is retail 0x40 bytes).
class CPersistContext {
public:
    template <class T> void Transfer(const char* name, T& value, const T& defaultValue);
};
// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptBase.h"
namespace NScript {
class CV_FisticuffsClubScript : public CScriptBase {
public:
    virtual void OnPersist(CPersistContext& context);
    void* m_pInterface;                                 // +0x40
    unsigned char _pad44[0x95 - 0x44];
    bool           CompletedKnotholePunchClub;              // +0x95
    bool           CompletedBowerstonePunchClub;            // +0x96
    bool           CompletedBanditPunchClub;                // +0x97
    bool           CompletedOakvalePunchClub;               // +0x98
};
void CV_FisticuffsClubScript::OnPersist(CPersistContext& context)
{
    context.Transfer("CompletedKnotholePunchClub", CompletedKnotholePunchClub, false);
    context.Transfer("CompletedBowerstonePunchClub", CompletedBowerstonePunchClub, false);
    context.Transfer("CompletedBanditPunchClub", CompletedBanditPunchClub, false);
    context.Transfer("CompletedOakvalePunchClub", CompletedOakvalePunchClub, false);
}
}