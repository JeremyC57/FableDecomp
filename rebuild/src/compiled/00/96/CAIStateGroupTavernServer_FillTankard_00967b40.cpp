// CAIStateGroup_TavernServer::FillTankard() -- with a live tankard and jug: pin the server, claim the tankard, play the pour action, wait up to 4 seconds while the serving point stays valid, fill the tankard, use a jug serving, play the second action, free the bar point and unpin
// Leaf: FillTankard
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +8.
// Naming wave 18: the global stand-in g_FramesPerSecond (0x01375550) is the protected static long CMainGameComponent::ConstantFPS, read through the
//   inline static accessor GetConstantFPS() as retail code outside the class does (name-only: the bytes are unchanged).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00967B40, 468 bytes.
// Callees: IsAlive via CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PTankard +0x2c, PJug +0x14), Terminate @ 0x008FCF40,
//   SetPushableByHero @ 0x0097E330, CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface 0xba = +0x34 bit 26 tankard,
//   0xbb = +0x34 bit 27 jug; the miss path reads the never-written key slot), tankard user setter @ 0x0094B400 (manifest
//   "CVillageTaskCarryCrate::AssignCreatureToTask" is a folded label), the two one-argument creature actions @ 0x00842A80 /
//   0x00842AF0 (both labelled "CCreatureAction_AlertToPlayer"; sizeof 0xa8 from the 0xac frame), ~CCreatureActionBase
//   @ 0x00693EF0, AIAction_PerformActionAndWait @ 0x0097DB70, AIAction_DoNothing @ 0x0097E200, CWorld::GetWorldFrame (static)
//   @ 0x0049D870 (manifest "GFGetPreMainMemoryUsed"), frames-per-second global @ 0x01375550, IsServingPointValid @ 0x00967250,
//   FreeBarPoint @ 0x009670A0.
// Layout: engine/CAIStateGroup_TavernServer.h (PJug +0x14 .. PPub +0x34) over the landed local CAIStateGroupBase model;
//   tankard fill level (float) and jug serving count at +0x0c of their components (descriptive names).
// Shape: every failure path written out in full (`FreeBarPoint(); SetPushableByHero(true); return false;`) instead of a
//   shared goto label -- VC7.1 then cross-jumps the copies and keeps the first one as the fall-through of the first test
//   (a goto to one shared block gets that block sunk below the loop); the frames-per-second global read into a local
//   before the GetWorldFrame call; the creature through an inline getter (`mov eax,[esi+4] ; mov eax,[eax+18h]`).
#include "fable_compiler.h"
class CTCBase;
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CBaseObject;
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"

FABLE_FORCEINLINE bool IS_THING_ALIVE(const CIntelligentPointer<CThing>& p)
{
    if (p.Get() != 0) {
        if (!p.Get()->IsDead())
            return true;
    }
    return false;
}
struct CTCTankard {                             // interface 0xba
    unsigned char _pad[0xc];
    float FillLevel;                            // +0x0c
    void SetUser(CThing* user);                 // 0x0094B400
};
struct CTCJug {                                 // interface 0xbb
    unsigned char _pad[0xc];
    long Servings;                              // +0x0c
};
class CThingAICreature;
class CCreatureActionBase { public:
    virtual void v00();
    unsigned char _body[0xa4];
    virtual ~CCreatureActionBase();                     // 0x00693EF0
};
struct CCreatureAction_FillTankardFromJug : CCreatureActionBase {
    CCreatureAction_FillTankardFromJug(CThingAICreature* creature);   // 0x00842A80
};
class CCreatureAction_PutDownJug : public CCreatureActionBase { public:
    CCreatureAction_PutDownJug(CThingAICreature* creature);           // 0x00842AF0
};
class CWorld { public: static long __fastcall GetFrame(); };  // 0x0049D870
class CMainGameComponent {
protected:
    static long ConstantFPS;                             // 0x01375550 ?ConstantFPS@CMainGameComponent@@1JA (protected static)
public:
    static long __fastcall GetConstantFPS() { return ConstantFPS; }   // Ego_r / FableWin ?GetConstantFPS@CMainGameComponent@@SIJXZ (inline)
};
class CAIBrain { public:
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};
enum EAIActionReturn { };
class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;
    unsigned char Enabled;
    unsigned char _pad_0x13;
    void Terminate();                           // 0x008FCF40
    CThingAICreature* GetAICreature() const { return (CThingAICreature*)PBrain->PCreature; }
    void SetPushableByHero(bool pushable);      protected: // 0x0097E330
    EAIActionReturn AIAction_DoNothing(); public:                  protected: // 0x0097E200
    EAIActionReturn AIAction_PerformActionAndWait(const CCreatureActionBase& action, bool b); public:   // 0x0097DB70
};
struct CAIStateGroup_TavernServer : CAIStateGroupBase {
    CIntelligentPointer<CThing> PJug;               // +0x14
    CIntelligentPointer<CThing> PBar;               // +0x1c
    CIntelligentPointer<CThing> PTable;             // +0x24
    CIntelligentPointer<CThing> PTankard;           // +0x2c
    CIntelligentPointer<CThing> PPub;               // +0x34
    void FreeBarPoint_009670a0();                        // 0x009670A0
    bool IsServingPointValid();                 // 0x00967250
    bool FillTankard();
};

bool CAIStateGroup_TavernServer::FillTankard()
{
    if (!IS_THING_ALIVE(PTankard) || !IS_THING_ALIVE(PJug)) {
        Terminate();
        return false;
    }
    SetPushableByHero(false);
    CTCTankard* tankard;
    PTankard.Get()->GetTC(TCI_TAVERN_TANKARD, &tankard);
    tankard->SetUser(PBrain->PCreature);
    if (AIAction_PerformActionAndWait(CCreatureAction_FillTankardFromJug(GetAICreature()), false) != 2) {
        FreeBarPoint_009670a0();
        SetPushableByHero(true);
        return false;
    }
    long fps = CMainGameComponent::GetConstantFPS();
    long endFrame = CWorld::GetFrame() + fps * 4;
    do {
        if (AIAction_DoNothing() != 4) {
            FreeBarPoint_009670a0();
            SetPushableByHero(true);
            return false;
        }
    } while (endFrame > CWorld::GetFrame() && IsServingPointValid());
    tankard->FillLevel = 1.0f;
    CTCJug* jug;
    PJug.Get()->GetTC(TCI_TAVERN_JUG, &jug);
    jug->Servings--;
    if (AIAction_PerformActionAndWait(CCreatureAction_PutDownJug(GetAICreature()), false) != 2) {
        FreeBarPoint_009670a0();
        SetPushableByHero(true);
        return false;
    }
    FreeBarPoint_009670a0();
    SetPushableByHero(true);
    return true;
}