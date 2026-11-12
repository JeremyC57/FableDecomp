// CAIStateGroup_SleepOnFloor::Update() -- walk to the sleep point, play ST_SLEEP_ON_FLOOR_INTO, push a
// NCreatureMode::CModeSleepBed(ST_SLEEP_ON_FLOOR_IDLE, "") onto the creature's mode manager, mark Asleep and idle
// until the state group is stopped; always Terminate()
// Leaf: Update
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +8.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00969CB0, length 436 bytes.
// Callees: CBaseIntelligentPointer ctor @ 0x00A01B10 (by-value CIntelligentPointer<const CThing> argument, vtable
//          0x01237F1C), AIAction_MoveToThingProximity @ 0x0097E380, CCharString(const char*, long) @ 0x0099EBF0 /
//          ~CCharString @ 0x0099EAE0, CCreatureAction_PlayAnimation ctor @ 0x008424F0 / ~CCreatureActionBase
//          @ 0x00693EF0, AIAction_PerformAction @ 0x0097D8B0, CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface
//          0x31 = bit 17 of +0x24), operator new @ 0x00BFEA1A (0x34), NCreatureMode::CModeSleepBed ctor @ 0x0084B7C0,
//          CCountedPointer<CModeBase>(CModeBase*) @ 0x00718A00, CTCCreatureModeManager::AddMode @ 0x0073ACC0,
//          IsStopping @ 0x008FD010, AIAction_DoNothing @ 0x0097E200, Terminate @ 0x008FCF40.
// Layout: local stand-ins; CAIStateGroupBase +0x4 PBrain, +0x14 PSleepPoint (IP, 8 bytes), +0x1c Asleep (retail
//         disassembly). The mode strings are passed as const char* and converted implicitly (an explicit
//         CCharString temporary pushes each address right after its ctor instead of after both).
// Name fix (agentNF7 2026-11-10): the local CIntelligentPointer wrapper structs replaced by the shared CIntelligentPointer<T> (engine/CIntelligentPointer.h), so the vtables bind ??_7?$CIntelligentPointer@VCThing@@@@6B@ 0x01238C6C / ??_7?$CIntelligentPointer@$$CBVCThing@@@@6B@ 0x01237F1C. The converting construction from CIntelligentPointer<CThing> is FableWin.pdb ??$?0VCThing@@@?$CIntelligentPointer@$$CBVCThing@@@@QAE@AAV?$CIntelligentPointer@VCThing@@@@@Z (FABLE_RETAIL_CINTELLIGENTPOINTER_CONVERT_MUTABLE).
#include "fable_compiler.h"
class CTCBase;
class CThingCreatureBase;
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CBaseObject;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_CONVERT_MUTABLE
#include "engine/CIntelligentPointer.h"
// CIntelligentPointerThing = the shared CIntelligentPointer<CThing> (engine/CIntelligentPointer.h).
struct CTCOccupiable {                          // interface 0xac
    CThing* GetOccupant();                      // 0x0087C3E0
    void SetOccupant(CThing* occupant);         // 0x0087C390
};

struct SCountBlock;
struct CModeBase {
    virtual void v00();
};
struct CCountedModePointer {                    // CCountedPointer<CModeBase>
    CModeBase* m_pObject;
    SCountBlock* m_pCount;
    CCountedModePointer(CModeBase* object);     // 0x00718A00
    CCountedModePointer(const CCountedModePointer& other);
    ~CCountedModePointer();                     // run by the callee
};
namespace NCreatureMode {
class CModeSleepBed : public CModeBase { public:              // 0x34 bytes, vtable 0x01275760
    unsigned char _body[0x30];
    CModeSleepBed(CThing* thing, const CCharString& sleepAnimName, const CCharString& group);   // 0x0084B7C0
};
}
class CTCCreatureModeManager { public:                 // interface 0x31
    bool AddMode(CCountedModePointer mode);     // 0x0073ACC0
};
class CSoundPair { public:
    long m_first;
    long m_second;
    CSoundPair() : m_first(0), m_second(0) { }
};
class CCreatureActionBase { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10();
    virtual void* Clone() const;                    // slot 11 (+0x2c)
    unsigned char _body[0xa4];
    virtual ~CCreatureActionBase();                         // 0x00693EF0
};
class CCreatureAction_PlayAnimation : public CCreatureActionBase { public:
    unsigned char _anim[0x0c];
    CCreatureAction_PlayAnimation(CThingCreatureBase* creature, const CCharString& animation,
        const CSoundPair& sound, bool a, bool b, bool c, long d, long e);   // 0x008424F0
};
struct CCountBlock { long Count; void (__fastcall* Destroy)(void* object); void* Object; };
struct CCountedPointer {
    void* Object;
    CCountBlock* Block;
    void Reset(void* object);                       // 0x00668D90
    ~CCountedPointer()
    {
        if (Block) {
            --Block->Count;
            if (Block->Count == 0) {
                Block->Destroy(Block->Object);
                delete Block;
            }
        }
        Object = 0;
        Block = 0;
    }
};
class CBrainOutput { public:                               // 0x48 bytes
    unsigned long Flags;                            // +0x00 (8: perform action)
    unsigned char _pad04[0x20 - 0x04];
    CCountedPointer PAction;                        // +0x20
    unsigned char _pad28[0x40 - 0x28];
    CBaseIntelligentPointer PTarget;                // +0x40
    CBrainOutput();                                 // 0x00835620
};
struct CThingCreature : CThing {
    bool IsFreeToPerformAction(const CCreatureActionBase* action);   // 0x00662210
};
class CAIBrain { public:
    unsigned char _pad[0x18];
    CThingCreature* PCreature;                      // +0x18
    void SetOutput(const CBrainOutput& output);  // 0x0088B8F0 ?SetOutput@CAIBrain@@QAEXABVCBrainOutput@@@Z (Ego_r.pdb + FableWin.pdb; was the stand-in SetOutput; name-fix 2026-11-12)
};
class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;
    unsigned char Enabled;
    unsigned char _pad_0x13;
public:
    void Terminate();   // 0x008FCF40
protected:
    bool IsStopping() const;   // 0x008FD010 (Ego_r.pdb protected const)
};
// CIntelligentPointerConstThing = the shared CIntelligentPointer<const CThing> (engine/CIntelligentPointer.h).
struct CAIStateGroup_SleepOnFloor : CAIStateGroupBase {
    CIntelligentPointer<CThing> PSleepPoint;       // +0x14
    bool Asleep;                                // +0x1c
    CThingCreatureBase* GetCreature() const { return (CThingCreatureBase*)PBrain->PCreature; }
    CThingCreature* GetCreatureThing() const { return PBrain->PCreature; }
    long AIAction_MoveToThingProximity(long repeat, CIntelligentPointer<const CThing> target, float distance, bool* reached,
        long movement, bool a, bool b, bool c, bool d, float speed);         // 0x0097E380
    long AIAction_PerformAction(CCreatureActionBase* action);   // 0x0097D8B0
    long AIAction_DoNothing();                  // 0x0097E200
    virtual void Update();
};

void CAIStateGroup_SleepOnFloor::Update()
{
    bool reached;
    if (AIAction_MoveToThingProximity(1, PSleepPoint, 1.0f, &reached, 3, true, false, true, true, 0.0f) == 2) {
        if (AIAction_PerformAction(&CCreatureAction_PlayAnimation(GetCreature(), "ST_SLEEP_ON_FLOOR_INTO", CSoundPair(),
                false, true, false, 0, 0)) == 2) {
            CTCCreatureModeManager* modes;
            PBrain->PCreature->GetTC(TCI_ENTITY_MODE_MANAGER, &modes);
            modes->AddMode(CCountedModePointer(new NCreatureMode::CModeSleepBed(GetCreatureThing(),
                "ST_SLEEP_ON_FLOOR_IDLE", "")));
            Asleep = true;
            while (!IsStopping())
                AIAction_DoNothing();
        }
    }
    Terminate();
}