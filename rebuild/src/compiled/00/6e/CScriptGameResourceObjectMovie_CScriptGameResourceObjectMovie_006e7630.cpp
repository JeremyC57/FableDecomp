// CScriptGameResourceObjectMovie::CScriptGameResourceObjectMovie(CGameScriptInterface* scriptInterface, bool setConversationInputProcess) -- empty PImp, own vtable, the two arguments kept; when asked, the current player is initialised and switched from mode 2 to mode 0x1b
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006E7630, length 95 bytes.
// Leaf: ??0CScriptGameResourceObjectMovie
// Retail bytes: push esi ; mov esi,ecx ; call CBaseIntelligentPointer::CBaseIntelligentPointer ; mov cl,[esp+0Ch] ; test cl,cl ; mov eax,[esp+8] ;
//               mov dword ptr [esi+8],0 ; mov dword ptr [esi+0Ch],0 ; mov dword ptr [esi],offset vftable(0x01260F00) ; mov [esi+10h],eax ; mov [esi+14h],cl ; je E ;
//               mov ecx,[eax+1Ch] ; push edi ; push ecx ; mov ecx,[eax+14h] ; call GetPlayer ; mov edi,eax ; mov ecx,edi ; call InitialiseIfUninitialised ;
//               push 2 ; mov ecx,edi ; call RemoveMode ; push 0 ; push 1Bh ; mov ecx,edi ; call AddMode ; pop edi ; E: mov eax,esi ; pop esi ; ret 8
// Callees: the root constructor CBaseIntelligentPointer @ 0x0099A380, CPlayerManager::GetPlayer(long) @ 0x004498C0,
// CPlayer::InitialiseIfUninitialised @ 0x00488A80, CPlayer::RemoveMode(EPlayerMode) @ 0x00634190, CPlayer::AddMode(EPlayerMode, bool)
// @ 0x006345C0. Vtable 0x01260F00 = CScriptGameResourceObjectMovie (rtti-vtables.tsv); the intermediate CScriptGameResourceObjectBase
// and CScriptGameResourceObjectMovieBase vtable stores are dead and dropped. Layout (PDB): MovieBase's PImp counted pointer at +8,
// ScriptInterface +0x10, SetConversationInputProcess +0x14; engine/CGameScriptInterface.h gives PlayerManager (+0x14) and
// CurrentPlayer (+0x1c). Shape: the test and the player lookup use the ARGUMENTS (cl, eax), not the stored members.
#include "engine/CGameScriptInterface.h"
#include "engine/CPlayerManager.h"
#include "engine/CPlayer.h"

enum EPlayerMode { PLAYER_MODE_2 = 2, PLAYER_MODE_1B = 0x1b };

struct CPlayerCalls : CPlayer {                // CPlayer (engine/CPlayer.h layout) plus the three members called here
    void InitialiseIfUninitialised();          // 0x00488A80
    void RemoveMode(EPlayerMode mode);         // 0x00634190
    void AddMode(EPlayerMode mode, bool flag); // 0x006345C0
};

struct CPlayerManagerCalls : CPlayerManager {  // engine/CPlayerManager.h layout plus the lookup
    CPlayerCalls* GetPlayer(long player);      // 0x004498C0
};

struct SCountBlock;
struct CCountedPointerRetail {                 // {object, count block}
    void*        m_pObject;
    SCountBlock* m_pCount;
    CCountedPointerRetail() : m_pObject(0), m_pCount(0) {}
};

#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"                       // : CBaseClass, vtable 0x0129A7D8 (~ slot 0, Validate slot 1)

class CScriptGameResourceObjectBase : public CBaseObject { public:   // vtable 0x0126008C
    virtual void v00();
};

class CScriptGameResourceObjectMovieBase : public CScriptGameResourceObjectBase { public:   // vtable 0x01260EF4
    virtual void v00();
    CCountedPointerRetail PImp;                // +0x08
};

struct CScriptGameResourceObjectMovie : CScriptGameResourceObjectMovieBase {   // vtable 0x01260F00
    virtual void v00();
    CGameScriptInterface* ScriptInterface;     // +0x10
    bool                  SetConversationInputProcess;   // +0x14
    CScriptGameResourceObjectMovie(CGameScriptInterface* scriptInterface, bool setConversationInputProcess);
};

CScriptGameResourceObjectMovie::CScriptGameResourceObjectMovie(CGameScriptInterface* scriptInterface, bool setConversationInputProcess)
    : ScriptInterface(scriptInterface), SetConversationInputProcess(setConversationInputProcess)
{
    if (setConversationInputProcess) {
        CPlayerCalls* player = static_cast<CPlayerManagerCalls*>(scriptInterface->PlayerManager)->GetPlayer(scriptInterface->CurrentPlayer);
        player->InitialiseIfUninitialised();
        player->RemoveMode(PLAYER_MODE_2);
        player->AddMode(PLAYER_MODE_1B, false);
    }
}