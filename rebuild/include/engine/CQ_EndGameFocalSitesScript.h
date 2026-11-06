#ifndef FABLE_ENGINE_CQ_ENDGAMEFOCALSITESSCRIPT_H
#define FABLE_ENGINE_CQ_ENDGAMEFOCALSITESSCRIPT_H
// Retail opt-in (chunk 800): NScript::CQ_EndGameFocalSitesScript, new header (no includer before chunk 800; the 45 landed rows of
// this quest carry local models). Not engine/CQ_EndGameScript.h, which is a different quest.
// Layout: Ego_r.pdb CQ_EndGameFocalSitesScript (struct_layouts_egor.tsv, 168 bytes) at retail offsets = PDB - 0x14 (retail
// CLevelScriptBase is 0x48; no STLport container in the class): CurrentFocalSiteRegion +0x48, TeleportHSPs +0x4c,
// RegionDialogueNames +0x60, TeleportPosition +0x74, MissionFailed +0x80, JackLeaves +0x81, EndCutScenePlayed +0x82,
// QuestStartScreenShown +0x83, counter +0x84, JackTimer +0x88, WorldTimer +0x8c, JackReached +0x90 -- the offsets the landed
// Init 0x00D2A8A0 writes (+0x48, +0x4c.., +0x60..) and CEndGameScythe::Main 0x00D2ED20 reads (+0x81, +0x83).
// Retail level-script vtable 0x012CBEB4 (0 dtor, 1 RegisterMain 0x00D2A950, 2 Main, 3 Init 0x00D2A8A0, 4 OnPersist 0x00D2AA10).
// Nested entity script: CEndGameScythe (Ego_r.pdb CEndGameScythe, 28 bytes: CEntityScriptBase 0x14, ParentClass +0x14,
// PMasterData +0x18; vtable 0x012CC33C: 0 scalar deleting dtor 0x00D2C710, 1 Main 0x00D2ED20, 2 Init 0x00D2C700,
// 3 GetParentScript 0x00D2C6F0; Alloc 0x00D2C650 -- the landed rows' leaves).
// Base: CLevelScriptBase<CQ_SunnyvaleMasterData>, as the other quest headers; ParentClass is a reference (engine/CQ_BanditCampBossBattleScript.h).
#ifndef FABLE_RETAIL_NSCRIPT
#error engine/CQ_EndGameFocalSitesScript.h is the retail NScript model: define FABLE_RETAIL_NSCRIPT before including it
#else
#include "rebuild_abi.h"
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CLevelScriptBase.h"
#include "engine/CEntityScriptBase.h"
#include "engine/C3DVector.h"
#include "engine/CTimer.h"
namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_EndGameFocalSitesScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    virtual void RegisterMain();                                         // 0x00D2A950
    virtual void Main();
    virtual void Init();                                                 // 0x00D2A8A0
    virtual void OnPersist(CPersistContext&);                            // 0x00D2AA10
    class CEndGameFocalSite;                                             // Alloc 0x00D2C140
    class CBattleMinion;                                                 // Alloc 0x00D2C340
    class CBattleGuard;                                                  // Alloc 0x00D2C430
    class CEndGameJackOfBlades;                                          // Alloc 0x00D2C260
    class CEndGameScythe;                                                // Alloc 0x00D2C650
    class CEndGameThunder;                                               // Alloc 0x00D2C730
    class CEndGameScarletRobe;                                           // Alloc 0x00D2CF80
    class CPermanentBarrier;
    long CurrentFocalSiteRegion;                                         // +0x48 (Ego_r.pdb +0x5c)
    CCharString TeleportHSPs[5];                                         // +0x4c
    CCharString RegionDialogueNames[5];                                  // +0x60
    C3DVector TeleportPosition;                                          // +0x74
    bool MissionFailed;                                                  // +0x80
    bool JackLeaves;                                                     // +0x81
    bool EndCutScenePlayed;                                              // +0x82
    bool QuestStartScreenShown;                                          // +0x83
    long counter;                                                        // +0x84
    CTimer JackTimer;                                                    // +0x88
    CTimer WorldTimer;                                                   // +0x8c
    bool JackReached;                                                    // +0x90
    class CEndGameScythe : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_EndGameFocalSitesScript&, CScriptDataBase*, const CScriptThing&);   // 0x00D2C650
        virtual CScriptBase* GetParentScript() const;                    // slot 3 0x00D2C6F0
    private:
        virtual void Main();                                             // slot 1 0x00D2ED20
        virtual void Init();                                             // slot 2 0x00D2C700
    protected:
        NScript::CQ_EndGameFocalSitesScript & ParentClass;               // +0x14
    protected:
        NScript::CQ_SunnyvaleMasterData * PMasterData;                   // +0x18
    };
};
}
FABLE_STATIC_ASSERT(sizeof(NScript::CQ_EndGameFocalSitesScript) == 0x94);
FABLE_STATIC_ASSERT(sizeof(NScript::CQ_EndGameFocalSitesScript::CEndGameScythe) == 0x1c);

#endif // FABLE_RETAIL_NSCRIPT
#endif // FABLE_ENGINE_CQ_ENDGAMEFOCALSITESSCRIPT_H
