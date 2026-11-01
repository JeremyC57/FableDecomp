// CScriptGameResourceObjectMovie::~CScriptGameResourceObjectMovie() -- movie script resource dtor: end the script interface's movie sequence and, when it set the conversation input process, take player mode 0x1B off the current player; then the base members (the PImp counted pointer) and ~CScriptGameResourceObjectBase
// Leaf: ??1CScriptGameResourceObjectMovie@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x006E7550, 111 bytes, tail jump to ~CBaseObject.
// Identity (proven): first store = vtable 0x01260F00, RTTI `.?AVCScriptGameResourceObjectMovie@@` (: CScriptGameResourceObjectMovieBase
//   : CScriptGameResourceObjectBase : CBaseObject : CBaseClass); the sole caller 0x006E7C93 is the scalar deleting dtor 0x006E7C90,
//   slot 0 of that vtable. Ego_r.pdb ??1CScriptGameResourceObjectMovie@@UAE@XZ. The last store 0x0126008C is
//   CScriptGameResourceObjectBase's vtable (its ??_G 0x006E70A0 inlines the same store + ~CBaseObject); MovieBase adds no store
//   (implicit dtor, its own ??_G 0x006E7BD0 expands the same PImp release).
// Callees: CGameScriptInterface::InternalEndMovieSequence 0x0089B210, CPlayerManager::GetPlayer(long) 0x004498C0 (Ego_r.pdb
//   ?GetPlayer@CPlayerManager@@QAEAAVCPlayer@@J@Z), CPlayer::RemoveMode(EPlayerMode) 0x00634190 (mode 0x1B; enumerator name not in
//   either PDB), the PImp info block's DeleteFunc, operator delete 0x00BFE9BC, CBaseObject::~CBaseObject 0x0099A430 (tail jump).
// Layout: Ego_r.pdb: CScriptGameResourceObjectMovieBase PImp (CCountedPointer<CScriptGameResourceObjectMovieBase>) +0x08,
//   CScriptGameResourceObjectMovie ScriptInterface +0x10, SetConversationInputProcess +0x14; CGameScriptInterface PlayerManager
//   +0x14, CurrentPlayer +0x1C. CBaseObject / CCountedPointer from the shared headers; no headers for the script classes.
// Shape: `CPlayer& player = ...GetPlayer(ScriptInterface->GetCurrentPlayer()); player.RemoveMode(0x1B);` -- the named reference
//   puts `push 1Bh` after the GetPlayer call and the inline PDB accessor GetCurrentPlayer (?GetCurrentPlayer@CGameScriptInterface@@
//   QBEJXZ) gives the CCountedPointer release its eax reload; one chained expression pushes 1Bh first, a direct CurrentPlayer read
//   reloads Info into edx (probes 1-5).
#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"
#include "engine/CCountedPointer.h"

enum EPlayerMode { };
class CPlayer {
public:
    void RemoveMode(EPlayerMode mode);                      // 0x00634190
};
#include "engine/CPlayerManager.h"                   // CPlayerManager::GetPlayer(long) 0x004498C0
class CWorld;
class CMainGameComponent;
class CDisplayEngine;
class CGameDefinitionManager;
class CThingSearchTools;
class CGameScriptInterface {
public:
    virtual ~CGameScriptInterface();
    void InternalEndMovieSequence();                        // 0x0089B210
    CWorld*                  World;                         // +0x04
    CMainGameComponent*      Component;                     // +0x08
    CDisplayEngine*          DisplayEngine;                 // +0x0C
    CGameDefinitionManager*  DefinitionManager;             // +0x10
    CPlayerManager*          PlayerManager;                 // +0x14
    CThingSearchTools*       ThingSearchTools;              // +0x18
    long                     CurrentPlayer;                 // +0x1C
    long GetCurrentPlayer() const { return CurrentPlayer; }
};

class CScriptGameResourceObjectBase : public CBaseObject {  // vtable 0x0126008C
public:
    virtual ~CScriptGameResourceObjectBase() { }
};
class CScriptGameResourceObjectMovieBase : public CScriptGameResourceObjectBase {
public:
    CCountedPointer<CScriptGameResourceObjectMovieBase> PImp;   // +0x08
};
class CScriptGameResourceObjectMovie : public CScriptGameResourceObjectMovieBase {   // vtable 0x01260F00
public:
    virtual ~CScriptGameResourceObjectMovie();
    CGameScriptInterface*   ScriptInterface;                // +0x10
    bool                    SetConversationInputProcess;    // +0x14
};

CScriptGameResourceObjectMovie::~CScriptGameResourceObjectMovie()
{
    ScriptInterface->InternalEndMovieSequence();
    if (SetConversationInputProcess) {
        CPlayer& player = ScriptInterface->PlayerManager->GetPlayer(ScriptInterface->GetCurrentPlayer());
        player.RemoveMode((EPlayerMode)0x1b);
    }
}
