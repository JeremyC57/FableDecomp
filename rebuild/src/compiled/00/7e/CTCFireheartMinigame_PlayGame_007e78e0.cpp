// CTCFireheartMinigame::PlayGame() -- start a session: find the sixteen grid markers by script name once, then reset the hero to grid cell (0, 3), put the player into mode 0x2A, hide the bottom half of the GUI and clear QuitWithDelay
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007E78E0, length 221 bytes (+ int3 padding). __fastcall, `ret`.
//         Ego_r.pdb: `?PlayGame@CTCFireheartMinigame@@QAEXXZ`.
// Callees: CCharString(const CCharString&) @ 0x0099EC30 / ~CCharString @ 0x0099EAE0 (the filter's name),
//          CTCBase::PeekThingSearchTools() const @ 0x00686D90,
//          CThingSearchTools::GetPFirstThing<CThingFilter_HasScriptName, CThing>(const F&, CThing**) const @ 0x0056D050,
//          CThing::GetPos @ 0x004C73D0, the physics component's SetPosition(const C3DVector&) (slot 30, +0x78),
//          CTCBase::GetPlayerManager @ 0x00686D40, CPlayerManager::GetMainPlayer @ 0x00449970,
//          CPlayer::GetCurrentMode @ 0x00633BE0, CPlayer::AddMode(EPlayerMode, bool) @ 0x006345C0.
//          Globals: the marker script-name table CCharString[4][4] @ 0x013BAEB0, g_pPlayerGui @ 0x013B8790.
// Shape: the marker grid is two nested counted loops strength-reduced to a name pointer (esi, compared with the table
//        end) and a marker pointer (edi); each filter is a temporary holding a copy of the name, built before the
//        pushes; the physics component is a named local (esi) whose vtable is read into edi before the position call; both mode arms end in the
//        duplicated GUI/QuitWithDelay tail.
// Layout: Ego_r.pdb CTCFireheartMinigame matches retail (Markers +0x0C, InSession +0x4C, ForceQuit +0x4D, Initialised
//         +0x54, XPos +0x68, YPos +0x6C, QuitWithDelay +0x88); retail CThing PhysicsTC +0x60; CPlayerGui::DisplayBottomHalfGUI +0x330.
// Layout (supersedes any local-model note above): engine/CPlayerGui.h now has the retail STLport 4.6 offsets, so the data
//   members come from the shared header; the class below keeps only the declarations this body needs.
#define CPlayerGui CPlayerGuiDonor
#include "engine/CPlayerGui.h"
#undef CPlayerGui
class CThing;
class CTCBase;

class CCharString {
public:
    void* PStringData;
    CCharString(const CCharString& other);          // 0x0099EC30
    ~CCharString();                                 // 0x0099EAE0
};
extern CCharString g_FireheartMarkerNames[4][4];    // 0x013BAEB0

class CThingFilter_HasScriptName {
public:
    CCharString ScriptName;
    CThingFilter_HasScriptName(const CCharString& name) : ScriptName(name) { }
};

class CThingSearchTools {
public:
    template<class F, class T> bool GetPFirstThing(const F& filter, T** ppThing) const;   // 0x0056D050 (HasScriptName, CThing)
};

class C3DVector;
class CTCPhysicsBase {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24();
    virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void SetPosition(const C3DVector& position);   // slot 30 (+0x78)
};

class CThing {
public:
    unsigned char _pad00[0x60];
    CTCPhysicsBase* PhysicsTC;                      // +0x60
    const C3DVector& GetPos() const;           // 0x004C73D0
};

class CPlayer {
public:
    long GetCurrentMode();                          // 0x00633BE0 (EPlayerMode)
    void AddMode(long mode, bool flag);             // 0x006345C0
};
class CPlayerManager {
public:
    CPlayer& GetMainPlayer();                       // 0x00449970
};

class CPlayerGui : public CPlayerGuiDonor {
public:
};
extern CPlayerGui* g_pPlayerGui;                    // 0x013B8790

class CTCBase {
public:
    virtual void v00();
    CThing* ParentThing;                            // +0x04
    unsigned char _pad08[0x4];
protected:
    CPlayerManager& GetPlayerManager();   // 0x00686D40
    const CThingSearchTools& PeekThingSearchTools() const;   // 0x00686D90
};

class CTCFireheartMinigame : public CTCBase {
public:
    CThing* Markers[4][4];                          // +0x0C
    bool InSession;                                 // +0x4C
    bool ForceQuit;                                 // +0x4D
    unsigned char _pad4e[2];
    CThing* CurrentMarker;                          // +0x50
    bool Initialised;                               // +0x54
    bool BlocksActive[4][4];                        // +0x55
    long XPos;                                      // +0x68
    long YPos;                                      // +0x6C
    bool Moving;                                    // +0x70
    unsigned char _pad71[0x88 - 0x71];
    bool QuitWithDelay;                             // +0x88
    void PlayGame();
};

void CTCFireheartMinigame::PlayGame()
{
    if (!Initialised) {
        for (long row = 0; row < 4; ++row)
            for (long column = 0; column < 4; ++column)
                PeekThingSearchTools().GetPFirstThing(CThingFilter_HasScriptName(g_FireheartMarkerNames[row][column]),
                                                      &Markers[row][column]);
    }
    InSession = true;
    ForceQuit = false;
    XPos = 0;
    YPos = 3;
    CTCPhysicsBase* physics = ParentThing->PhysicsTC;
    physics->SetPosition(Markers[0][3]->GetPos());
    CPlayer* player = &GetPlayerManager().GetMainPlayer();
    if (player->GetCurrentMode() != 0x2a)
        player->AddMode(0x2a, false);
    g_pPlayerGui->DisplayBottomHalfGUI = false;
    QuitWithDelay = false;
}