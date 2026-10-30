// NScript::CQ_EndGameFocalSitesScript::Init() -- disable every creature generator group, start at focal site 1 and fill the teleport hero-start points and the guildmaster dialogue names of sites 1..4
// Leaf: ?Init@CQ_EndGameFocalSitesScript@NScript@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D2A8A0, 168 bytes, thiscall, plain `ret`.
// Identity (proven): slot 3 (Init) of the level-script vtable 0x012CBEB4 (0 dtor, 1 RegisterMain, 2 Main, 3 Init, 4 OnPersist), stored by the quest ctor 0x00D2A820; ?Init@CQ_EndGameFocalSitesScript@NScript@@ is in Ego_r.pdb and FableWin.pdb.
// Label: the manifest's "sub_00d2a8a0" is a gap-scan placeholder.
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0, CCharString::operator=(const char*) 0x0099EFE0; interface slot 558 SetCreatureGeneratorsEnabled.
// Data: the pooled empty string 0x0122D70E; "EndGameSecondHSP" @ 0x012CBFA8 .. "TEXT_QST_001_GUILDMASTER_GIBBET_HILL_SITE" @ 0x012CBEC8.
// Layout: retail CScriptBase is 0x40 bytes (Ego_r.pdb 0x54), then the interface +0x40 and the master data +0x44; the quest's own members sit 0x14 below their Ego_r.pdb offsets, 4 lower again after each Dinkumware container. CurrentFocalSiteRegion +0x48, TeleportHSPs (CCharString[5]) +0x4c, RegionDialogueNames (CCharString[5]) +0x60, TeleportPosition (C3DVector) +0x74, QuestStartScreenShown +0x83.
class CCharString {                                      // 4 bytes: the counted string-data pointer
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);      // 0x0099EBF0
    ~CCharString();                                       // 0x0099EAE0
    const CCharString& operator=(const char* text);       // 0x0099EFE0
};
class C3DVector;                                         // 1-aligned 12-byte vector (only referenced here)
class CCPPointerInfo {                                   // CCountedPointer's shared block
public:
    long  RefCount;
    void (__fastcall* DeleteFunc)(void* data);
    void* Data;
};
template <class T>
class CCountedPointer {
public:
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info) { if (Info) ++Info->RefCount; }
    CCountedPointer& operator=(const CCountedPointer& other);   // 0x00704580 (CScriptThing instance)
};
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0 (slot 0), implicit copy
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// Retail expands these at this row's sites (the local model had them inline or implicit): defined inline after the header.
inline CScriptThing::CScriptThing(const CScriptThing& other) : CBaseClass(other), PImp(other.PImp) { }
inline CScriptThing& CScriptThing::operator=(const CScriptThing& other) { PImp = other.PImp; return *this; }
// CGameScriptInterfaceBase is the retail abstract script interface from the shared headers (FABLE_RETAIL_NSCRIPT
//   opt-in: CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x0126009C, 917 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterfaceBase.h"
class CPersistContext;
// CScriptBase / CLevelScriptBase<T>: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40; CLevelScriptBase<T> Game +0x40, MasterScriptData +0x44).
#include "engine/CLevelScriptBase.h"
namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_EndGameFocalSitesScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    long CurrentFocalSiteRegion;                        // +0x48 (Ego_r.pdb +0x5c)
    CCharString TeleportHSPs[5];                        // +0x4c
    CCharString RegionDialogueNames[5];                 // +0x60
    unsigned char TeleportPosition[0xc];                // +0x74 (C3DVector)
    bool MissionFailed;                                 // +0x80
    bool JackLeaves;                                    // +0x81
    bool EndCutScenePlayed;                             // +0x82
    bool QuestStartScreenShown;                         // +0x83
    virtual void Init();                                // public in Ego_r.pdb (?Init@...@UAEXXZ)
};
}

void NScript::CQ_EndGameFocalSitesScript::Init()
{
    Game->SetCreatureGeneratorsEnabled("", false);
    CurrentFocalSiteRegion = 1;
    TeleportHSPs[1] = "EndGameSecondHSP";
    TeleportHSPs[2] = "HobbeCave2HSP";
    TeleportHSPs[3] = "BowerstoneJailHSP";
    TeleportHSPs[4] = "GuildTrainingHSP";
    RegionDialogueNames[1] = "TEXT_QST_001_GUILDMASTER_WITCHWOOD_SITE";
    RegionDialogueNames[2] = "TEXT_QST_001_GUILDMASTER_GREATWOOD_SITE";
    RegionDialogueNames[3] = "TEXT_QST_001_GUILDMASTER_HOBBE_CAVE_SITE";
    RegionDialogueNames[4] = "TEXT_QST_001_GUILDMASTER_GIBBET_HILL_SITE";
    QuestStartScreenShown = false;
}