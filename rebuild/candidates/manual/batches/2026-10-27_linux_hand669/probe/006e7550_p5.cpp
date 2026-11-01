// CScriptGameResourceObjectMovie::~CScriptGameResourceObjectMovie() -- movie script resource dtor: end the script interface's movie sequence and, when it set the conversation input process, take player mode 0x1B off the current player; then the base members (the PImp counted pointer) and ~CScriptGameResourceObjectBase
// Leaf: ??1CScriptGameResourceObjectMovie
#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"
#include "engine/CCountedPointer.h"

enum EPlayerMode { };
class CPlayer {
public:
    void RemoveMode(EPlayerMode mode);                      // 0x00634190
};
class CPlayerManager {
public:
    CPlayer& GetPlayer(long player);                        // 0x004498C0
};
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
    if (SetConversationInputProcess)
        ScriptInterface->PlayerManager->GetPlayer(ScriptInterface->GetCurrentPlayer()).RemoveMode((EPlayerMode)0x1b);
}
