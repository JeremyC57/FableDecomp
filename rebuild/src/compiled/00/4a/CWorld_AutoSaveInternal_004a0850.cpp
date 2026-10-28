// CWorld::AutoSaveInternal(const CWideString&, bool) -- autosave the game unless there is no live hero or an autosave is already running
// Retail: Fable.exe @ 0x004A0850, 113 bytes. PDB ?AutoSaveInternal@CWorld@@IAE_NABVCWideString@@_N@Z (protected)
// Callees: CPlayerManager::GetMainPlayer 0x00449970, CPlayer::GetPControlledCreature 0x00487DC0, the quest manager singleton
// 0x013B89FC IsBetweenQuestStartAndFinishScreens() 0x004AF290, CWorld::SaveGameState(const CWideString&, bool, bool)
// 0x004A05C0 (PDB ?SaveGameState@CWorld@@QAEXABVCWideString@@_N1@Z; manifest label "CWorld_SaveGameState").
// Retail offsets: CWorld PlayerManager +0x0C, AutoSaveLocked +0x100 (PDB +0x10C); CThing Dead +0x91 bit 0 (PDB - 0x10).
// Shape: the two SaveGameState calls (quest-screen flag true/false) are complete statements that VC7.1 merges into
// one call after the argument pushes.
// Layout (supersedes any local-model note above): engine/CWorld.h now has the retail STLport 4.6 offsets, so the data
//   members come from the shared header; the class below keeps only the declarations this body needs.
#define CWorld CWorldDonor
#include "engine/CWorld.h"
#undef CWorld
class CWideStringData;
class CWideString { public: CWideStringData* PStringData; };

class CThing { public:
    unsigned char _pad[0x91];
    unsigned char Dead : 1;                      // +0x91 bit 0
};
class CThingPlayerCreature : public CThing { };
class CPlayer { public: CThingPlayerCreature* GetPControlledCreature() const; };                   // 0x00487DC0
class CPlayerManager { public: public:
    CPlayer& GetMainPlayer();   // 0x00449970
};          // 0x00449970
class CQuestManager { public: bool IsBetweenQuestStartAndFinishScreens() const; };   // 0x004AF290
extern CQuestManager* g_pQuestManager;           // 0x013B89FC

class CWorld : public CWorldDonor {
 public:
    void SaveGameState(const CWideString& name, bool questScreens, bool b);   // 0x004A05C0
    bool AutoSaveInternal(const CWideString& name, bool b);
};

bool CWorld::AutoSaveInternal(const CWideString& name, bool b)
{
    CThing* hero = PlayerManager->GetMainPlayer().GetPControlledCreature();
    if (hero && !hero->Dead && !AutoSaveLocked)
    {
        AutoSaveLocked = true;
        if (g_pQuestManager->IsBetweenQuestStartAndFinishScreens())
            SaveGameState(name, true, b);
        else
            SaveGameState(name, false, b);
        AutoSaveLocked = false;
        return true;
    }
    return false;
}