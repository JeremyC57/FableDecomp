// CFishCompOrganiser::Initialize() -- reset the conversation state, read the competition's rod/fish/prize counts from the script def and copy its rod and fish def names (stopping when the script is done), then set the organiser up (information marker, persistent, not pushable, no movement in actions) and attach it to the active quest
// Leaf: ?Initialize@CFishCompOrganiser@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E81D10, 412 bytes, plain `ret`.
// Callees: CCharString() 0x0099E4B0 / ~CCharString 0x0099EAE0 / operator= 0x0099EFB0, vector<CCharString>::resize 0x0043332C
//   (manifest label push_back), IsDone 0x00F35B30 (on `this`); game-interface slots 360 SetThingHasInformation,
//   477 SetThingPersistent, 844 SetIsPushableByHero (CScriptThing by value), 529 EntitySetAsUseMovementInActions,
//   655 GetActiveQuestName, 465 EntityAttachToScript.
// Layout: the landed CFishCompOrganiser model (ctor 0x00E7EF70, CheckForFish 0x00E81A60): Game +4, Me +8, CommentHitByHero
//   +0x48, Conversation +0x4c, FishingRodsInGame +0x50, FishTypesInGame +0x54, FishingPrizes +0x58, RodDef +0x60, FishDef +0x6c;
//   the script def 0x0143E90C holds the fish types +0x5a0, rods +0x5a4, prizes +0x5a8, rod defs +0x5b8, fish defs +0x5c4.

namespace NScript { class CPredicateBase; }
class CCharString { public:
    void* PStringData;
    CCharString();                                       // 0x0099E4B0
    ~CCharString();                                      // 0x0099EAE0
    const CCharString& operator=(const CCharString& other);    // 0x0099EFB0
};
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0 (slot 0), implicit copy
class CCPPointerInfo { public: long RefCount; void (__fastcall* DeleteFunc)(void*); void* Data; };
template <class T> class CCountedPointer { public:
    void* Data;
    CCPPointerInfo* Info;
    CCountedPointer(const CCountedPointer& p) : Data(p.Data), Info(p.Info) { if (Info) ++Info->RefCount; }
};
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// Retail expands these at this row's sites (the local model had them in class): defined inline after the header.
inline CScriptThing::CScriptThing(const CScriptThing& other) : CBaseClass(other), PImp(other.PImp) { }
inline CScriptThing& CScriptThing::operator=(const CScriptThing& other) { PImp = other.PImp; return *this; }
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
template <class T> struct vector {
    T* _M_start; T* _M_finish; T* _M_end_of_storage;
    void resize(unsigned int n, const T& value);         // 0x0043332C
    void resize(unsigned int n) { resize(n, T()); }
    T* begin() { return _M_start; }
    T& operator[](unsigned int n) { return *(begin() + n); }
};
class CScriptDef { public:
    unsigned char Data[0x5a0];
    long          FishTypesInGame;                       // +0x5a0
    long          FishingRodsInGame;                     // +0x5a4
    long          FishingPrizes;                         // +0x5a8
    unsigned char Pad[0xc];
    vector<CCharString> RodDefs;                         // +0x5b8
    vector<CCharString> FishDefs;                        // +0x5c4
};
extern CScriptDef* pGlobalScriptDef;                         // 0x0143E90C (Ego_r.pdb `pGlobalScriptDef`, a def pointer: its first word)

// CEntityScriptBase: the retail model from engine/CEntityScriptBase.h (vtable 0x012C3224, 7 slots, 0x14: Game +0x04, Me +0x08).
#include "engine/CEntityScriptBase.h"

class CFishCompOrganiser : public CEntityScriptBase {
public:
        unsigned char         Body[0x34];         // +0x14
    bool                  CommentHitByHero;   // +0x48
    long                  Conversation;       // +0x4c
    long                  FishingRodsInGame;  // +0x50
    long                  FishTypesInGame;    // +0x54
    long                  FishingPrizes;      // +0x58
    CCharString           CompetitionEntry;   // +0x5c
    vector<CCharString>   RodDef;             // +0x60
    vector<CCharString>   FishDef;            // +0x6c
    void Initialize();
};

void CFishCompOrganiser::Initialize()
{
    Conversation = -1;
    CommentHitByHero = false;
    FishingPrizes = pGlobalScriptDef->FishingPrizes;
    FishTypesInGame = pGlobalScriptDef->FishTypesInGame;
    FishingRodsInGame = pGlobalScriptDef->FishingRodsInGame;
    RodDef.resize(FishingRodsInGame);
    FishDef.resize(FishTypesInGame);
    for (long i = 0; i < FishingRodsInGame; ++i) {
        if (IsActiveThreadTerminating())
            return;
        RodDef[i] = pGlobalScriptDef->RodDefs[i];
    }
    if (IsActiveThreadTerminating())
        return;
    for (long j = 0; j < FishTypesInGame; ++j) {
        if (IsActiveThreadTerminating())
            return;
        FishDef[j] = pGlobalScriptDef->FishDefs[j];
    }
    if (IsActiveThreadTerminating())
        return;
    Game->SetThingHasInformation(Me, false, true, false);
    Game->SetThingPersistent(Me, true);
    Game->SetIsPushableByHero(Me, false);
    Game->EntitySetAsUseMovementInActions(Me, false);
    Game->EntityAttachToScript(Me, Game->GetActiveQuestName());
}