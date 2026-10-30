// NScript::CV_BordelloScript::CBordelloClient::Init() -- a new bordello client: one more client alive, brain state 1, a random 0-4 sex chance, first line, the first free client slot taken; persistent, and the 'don't scream' opinion reaction mask
// Leaf: ?Init@CBordelloClient@CV_BordelloScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E3B1A0, 137 bytes, thiscall, plain `ret` (private virtual, entity vtable slot 2).
// Identity (proven): slot 2 (Init) of the RTTI-less entity vtable 0x012E3B48, which the landed
//   NScript::CV_BordelloScript::CBordelloClient::Alloc 0x00E3B0F0 stores (site 0x00E3B126); Ego_r.pdb has ?Init@CBordelloClient@CV_BordelloScript@NScript@@EAEXXZ.
//   Entity slot order (landed Init / OnPredicateFail rows, Ego_r.pdb CEntityScriptBase): 0 dtor, 1 Main, 2 Init,
//   3 GetParentScript, 4 OnPersist, 5 OnPredicateFail, 6 OnInterrupted.
// Layout: Ego_r.pdb CBordelloClient (entity offsets equal retail): ParentClass +0x14, PMasterData +0x18, members from +0x1c;
//   the parent level script's members sit at Ego_r offset - 0x14 (retail CScriptBase is 0x40).
// Callees: rand 0x00BFEB16; interface slots 477 SetThingPersistent, 611 EntitySetOpinionReactionMask
//   ("OPINION_REACTION_MASK_DONT_SCREAM" temporary); CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0.
// Parent members: the agent501 CV_BordelloScript model (0x00E3B230, the client's OnPredicateFail that frees the slot).
// Shape: the slot search uses a local index (retail keeps ClientID = 0 when every slot is taken).
extern "C" int __cdecl rand();                         // 0x00BFEB16 (MSVCR71 import thunk)
class CCharString {
public:
    void* PData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0 (slot 0), implicit copy
class CCPPointerInfo {
public:
    unsigned long RefCount;
};
template <class T> class CCountedPointer {
public:
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info) { if (Info) ++Info->RefCount; }
};
class CScriptThing : public CBaseClass {                // 12 bytes, vtable 0x01238C8C
public:
    virtual ~CScriptThing();                            // slot 0
    CCountedPointer<CScriptThing> PImp;                 // +0x04 Data, +0x08 Info
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
class CScriptBase;
class CPersistContext;
class CEntityScriptBase {                               // Ego_r.pdb: vptr, Game +0x04, Me +0x08 (20 bytes)
public:
    virtual ~CEntityScriptBase();                       // slot 0
    virtual void Main() = 0;                            // slot 1
    virtual void Init();                                // slot 2
    virtual CScriptBase* GetParentScript() const = 0;   // slot 3
    virtual void OnPersist(CPersistContext& context);   // slot 4
    virtual void OnPredicateFail();                     // slot 5
    virtual void OnInterrupted();                       // slot 6
    bool IsActiveThreadTerminating() const;             // 0x00F35B30
    CGameScriptInterface* Game;                         // +0x04
    CScriptThing          Me;                           // +0x08
};
class CPersistContext;
class CScriptBase {                                     // retail 0x40 bytes (Ego_r 0x54: debug containers)
public:
    virtual ~CScriptBase();                             // slot 0
protected:
    virtual void RegisterMain();                        // slot 1
    virtual void Main();                                // slot 2
    virtual void Init();                                // slot 3
public:
    virtual void OnPersist(CPersistContext& context);   // slot 4
    bool IsActiveThreadTerminating();                   // 0x00CB7940
    unsigned char _body[0x3c];
};
template <class TData> class CLevelScriptBase : public CScriptBase {
public:
    CGameScriptInterface* Game;                         // +0x40
    TData*                MasterScriptData;             // +0x44
};

namespace NScript {
class CQ_SunnyvaleMasterData;
class CV_BordelloScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    bool HeroPartying;                                  // +0x48
    bool HeroTricking;                                  // +0x49
    bool MagicianSleeping;                              // +0x4a
    bool CutscenePlaying;                               // +0x4b
    bool PlayerOwned;                                   // +0x4c
    bool BecomeNunnery;                                 // +0x4d
    bool PlayerHasLeftRegion;                           // +0x4e
    bool HeroFoundDeeds;                                // +0x4f
    bool HeroFoundDeedsLocation;                        // +0x50
    long ClientsAlive;                                  // +0x54
    bool ClientInUse[3];                                // +0x58
    class CBordelloClient;
};
class CV_BordelloScript::CBordelloClient : public CEntityScriptBase {
public:
    CV_BordelloScript* ParentClass;   // +0x14
    CQ_SunnyvaleMasterData* PMasterData;   // +0x18
    long BrainState;                                    // +0x1c
    long ClientID;                                      // +0x20
    long SexChance;                                     // +0x24
    long NextLine;                                      // +0x28
private:
    virtual void Init();
};

void CV_BordelloScript::CBordelloClient::Init()
{
    ++ParentClass->ClientsAlive;
    BrainState = 1;
    SexChance = rand() % 5;
    NextLine = 1;
    ClientID = 0;
    for (long i = 0; i < 3; ++i) {
        if (!ParentClass->ClientInUse[i]) {
            ClientID = i;
            ParentClass->ClientInUse[i] = true;
            break;
        }
    }
    Game->SetThingPersistent(Me, true);
    Game->EntitySetOpinionReactionMask(Me, "OPINION_REACTION_MASK_DONT_SCREAM");
}
}