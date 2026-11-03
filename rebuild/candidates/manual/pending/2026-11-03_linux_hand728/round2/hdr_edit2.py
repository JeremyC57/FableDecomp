H="/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/chunk728b/hdr/"
def ed(f, pairs):
    p=H+f; s=open(p).read()
    for a,b in pairs:
        assert s.count(a)==1, (f, a[:70], s.count(a))
        s=s.replace(a,b,1)
    open(p,"w").write(s)
ed("engine/CPlayerGui.h",[
("""    CSubtitleRenderer& GetSubtitleRenderer() { return SubtitleRenderer; }
};""","""    CSubtitleRenderer& GetSubtitleRenderer() { return SubtitleRenderer; }
    // Chunk 728 round 2 (row 0x006D0830): Ego_r.pdb / FableWin.pdb ?GetPHeroInfoScreensManager@CPlayerGui@@QBEPAVCManager@
    // NHeroInformationScreens@@XZ, out of line in retail (0x00437CD0 `mov eax,[ecx+32Ch] ; ret`; the landed OnKill rows bind it).
    NHeroInformationScreens::CManager* GetPHeroInfoScreensManager() const;   // 0x00437CD0
};"""),
("""class CThing;class C3DVector;
""","""class CThing;class C3DVector;
namespace NHeroInformationScreens { class CManager; }
"""),
])
ed("engine/CTCSoundPlayer.h",[
("""class CTCSoundPlayer : public CTCBase {
public:
    unsigned long PlayAttachedCriteria(const CCharString&,bool,bool,bool,long);""",
"""class CTCSoundPlayer : public CTCBase {
public:
    unsigned long PlayAttachedCriteria(const CCharString&,bool,bool,bool,long);
#ifdef FABLE_RETAIL_CTCSOUNDPLAYER
    // Chunk 728 round 2 (row 0x006D0830 CTCWife::GlobalUpdate): TCI_SOUND_PLAYER (16; GlobalUpdate tests bit 16 of UsingComponents
    // +0x20, `test ecx,10000h`) and the Ego_r.pdb / FableWin.pdb members it calls out of line:
    // ?PlayAttachedDialogue@CTCSoundPlayer@@QAEKABVCCharString@@_N111@Z 0x0066BED0, ?IsAttachedSoundPlaying@CTCSoundPlayer@@QBE_NK@Z
    // 0x0066AE10, ?StopAttachedSound@CTCSoundPlayer@@QAEXK@Z 0x0066ADD0. Declarations only: no layout change.
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return TCI_SOUND_PLAYER; }
    unsigned long PlayAttachedDialogue(const CCharString& group, bool, bool, bool, bool);   // 0x0066BED0
    bool IsAttachedSoundPlaying(unsigned long sampleID) const;           // 0x0066AE10
    void StopAttachedSound(unsigned long sampleID);                      // 0x0066ADD0
#endif"""),
("""#include "engine/CIntelligentPointer.h"
""","""#include "engine/CIntelligentPointer.h"
#ifdef FABLE_RETAIL_CTCSOUNDPLAYER
#include "engine/ETCInterfaceType.h"
#endif
"""),
])
ed("engine/CTCVillage.h",[
("""    void AddThingToVillage(CThing& thing);                   // 0x0065B854 ?AddThingToVillage@CTCVillage@@QAEXAAVCThing@@@Z
""","""    void AddThingToVillage(CThing& thing);                   // 0x0065B854 ?AddThingToVillage@CTCVillage@@QAEXAAVCThing@@@Z
    // Chunk 728 round 2 (row 0x006D0830 CTCWife::GlobalUpdate): Ego_r.pdb / FableWin.pdb ?OnHeroSlept@CTCVillage@@QAEXXZ, out of line
    // (0x0065935E). A declaration only.
    void OnHeroSlept();                                      // 0x0065935E
"""),
])
ed("engine/CThingCreatureBase.h",[
("""    bool AutoPlaceNearBuilding(const CThingBuilding*,float);
""","""    bool AutoPlaceNearBuilding(const CThingBuilding*,float);
    // Chunk 728 round 2 (row 0x006D0830 CTCWife::GlobalUpdate): Ego_r.pdb / FableWin.pdb ?AutoPlaceNear@CThingCreatureBase@@QAE_N
    // ABVC3DVector@@M@Z (0x00665DF0) and ?GetSex@CThingCreatureBase@@QBE?AW4ESex@@XZ (0x00661FF0), both out of line. ESex is a
    // forward enum (no enumerator names on disk; GlobalUpdate compares the result with 2).
    bool AutoPlaceNear(const C3DVector& pos, float radius);             // 0x00665DF0
    ESex GetSex() const;                                                 // 0x00661FF0
"""),
("""class CHeroCombatDef;
class CThingCreatureBase : public CThingGameObject {""","""class CHeroCombatDef;
enum ESex;
class CThingCreatureBase : public CThingGameObject {"""),
])
ed("engine/CThingAICreature.h",[
("""    unsigned char _pad_0x160[0x68];                                      // +0x160 RelationshipsInit .. MaxFlyingSpeed (Ego_r.pdb members, opaque)""",
"""    unsigned char _pad_0x160[0x8];                                       // +0x160 RelationshipsInit .. (Ego_r.pdb members, opaque)
    // Chunk 728 round 2 (row 0x006D0830 CTCWife::GlobalUpdate): Ego_r.pdb CIntelligentPointer<CThingBuilding> HomeBuilding (the
    // default branch's +0x168; GlobalUpdate calls GetPItem 0x00A01B50 on [creature+168h]). Same size and offsets as before.
    CIntelligentPointer<CThingBuilding> HomeBuilding;                    // +0x168
    unsigned char _pad_0x170[0x58];                                      // +0x170 WorkBuilding .. MaxFlyingSpeed (opaque)"""),
("""    void SetAsAllowedToFollowHero(bool allowed) { AllowedToFollowHero = allowed; }
""","""    void SetAsAllowedToFollowHero(bool allowed) { AllowedToFollowHero = allowed; }
    // Chunk 728 round 2: Ego_r.pdb / FableWin.pdb ?GetHomeBuilding@CThingAICreature@@QBEPAVCThingBuilding@@XZ, inline in retail
    // (GlobalUpdate: `lea ecx,[ebp+168h] ; call GetPItem`).
    CThingBuilding* GetHomeBuilding() const { return HomeBuilding.Get(); }
"""),
])
ed("engine/CPlayer.h",[
("""    void AddMode(EPlayerMode mode, bool);                        // 0x006345C0
""","""    void AddMode(EPlayerMode mode, bool);                        // 0x006345C0
    // Chunk 728 round 2 (row 0x006D0830 CTCWife::GlobalUpdate): Ego_r.pdb / FableWin.pdb ?RemoveMode@CPlayer@@QAEXW4EPlayerMode@@@Z
    // (retail 0x00634190, `ret 4`; the bound ...QAEXJ@Z spelling is a long-typed stand-in).
    void RemoveMode(EPlayerMode mode);                           // 0x00634190
"""),
])
ed("engine/CGameCameraManager.h",[
("""    void SetScriptedCameraMode(CCountedPointer<NCameraModes::CScriptModuleBase> mode);   // 0x00699E60
""","""    void SetScriptedCameraMode(CCountedPointer<NCameraModes::CScriptModuleBase> mode);   // 0x00699E60
    // Chunk 728 round 2 (row 0x006D0830 CTCWife::GlobalUpdate): Ego_r.pdb / FableWin.pdb ?ResetCamera@CGameCameraManager@@QAEXXZ,
    // out of line (0x0069A990). A declaration only.
    void ResetCamera();                                                  // 0x0069A990
"""),
])
