#ifndef FABLE_ENGINE_CV_BANDITCAMPPATHSCRIPT_H
#define FABLE_ENGINE_CV_BANDITCAMPPATHSCRIPT_H
// Retail opt-in (chunk 797): NScript::CV_BanditCampPathScript, new header (no includer before chunk 797).
// Layout: Ego_r.pdb CV_BanditCampPathScript (struct_layouts_egor.tsv, 136 bytes) at retail offsets: PDB - 0x14 up to Bandits (retail
// CLevelScriptBase is 0x48), PDB - 0x18 after it (the retail STLport vector is 12 bytes, Ego_r's Dinkumware one 16):
// OverheardBanditConversation +0x48, Bandits +0x4c, Bandit1 +0x58, Bandit2 +0x64; 0x70 bytes. Checked against retail: Main 0x00E46D30
// fills +0x4c through GetAllThingsWithScriptName, assigns +0x58 / +0x64 (CScriptThing::operator= 0x008AB980), reads and sets +0x48.
// Base: CLevelScriptBase<CQ_SunnyvaleMasterData> (the Sunnyvale village scripts' base, as engine/CV_BanditTollScript.h).
// Methods (Ego_r.pdb): RegisterMain / Main / Init (UAEXXZ; Main 0x00E46D30, RegisterMain 0x00E46C40, Init 0x00E46C30 landed),
// OnPersist(CPersistContext&). Nested entity script: CBCP_Bandit (Alloc 0x00E47500, landed; binding literal "BCP_Bandit").
#ifndef FABLE_RETAIL_NSCRIPT
#error engine/CV_BanditCampPathScript.h is the retail NScript model: define FABLE_RETAIL_NSCRIPT before including it
#else
#include "rebuild_abi.h"
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CLevelScriptBase.h"
#include "engine/CEntityScriptBase.h"
#include "stlport/fable_containers.h"
class CPersistContext;
namespace NScript {
class CQ_SunnyvaleMasterData;
class CV_BanditCampPathScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    virtual void RegisterMain();                                         // 0x00E46C40
    virtual void Main();                                                 // 0x00E46D30
    virtual void Init();                                                 // 0x00E46C30
    virtual void OnPersist(CPersistContext&);
    bool OverheardBanditConversation;                                    // +0x48
    std::vector<CScriptThing> Bandits;                                   // +0x4c
    CScriptThing Bandit1;                                                // +0x58
    CScriptThing Bandit2;                                                // +0x64
    class CBCP_Bandit;                                                   // Alloc 0x00E47500
};
}
FABLE_STATIC_ASSERT(sizeof(NScript::CV_BanditCampPathScript) == 0x70);

#endif // FABLE_RETAIL_NSCRIPT
#endif // FABLE_ENGINE_CV_BANDITCAMPPATHSCRIPT_H
