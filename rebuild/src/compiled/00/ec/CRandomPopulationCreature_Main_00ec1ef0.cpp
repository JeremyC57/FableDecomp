// CRandomPopulationCreature::Main() -- a randomly populated creature: register an alive-filter over Me; the first time through, attach Me to its village ("PopulationVillageEvil" for type 2, "PopulationVillageGood" for types 0/1) when that village is alive; make Me non-persistent, take it under scripted control and walk it to its exit point until within 2 units; then (non-evil types) lower the current region's population scale by 0.05 clamped to [0,1], remove Me and idle
// Leaf: Main
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00EC1EF0, length 836 bytes, plain `ret`.
// Callees: CIsThingAlive (vtable 0x012C32F0) over the CScriptThing copy ctor 0x004ABE90, ~CIsThingAlive 0x00E241B0,
// RegisterFilter 0x00F35B10, IsActiveThreadTerminating 0x00F35B30; CScriptThing::IsAlive 0x004AB130 (devirtualised on a
// local), ~CScriptThing 0x004AA840 (village exits) and its inline expansion (the GetScriptThing temporary: vtable
// 0x01238C8C, counted-pointer release, operator delete 0x00BFE9BC, ~CBaseClass 0x0099A2E0); scripted-thing handle (vtable
// 0x0127094C) over CBaseObject 0x0099A380, IsScripting 0x00CD23B9, Release 0x00CD2770, MoveToPosition 0x007E72F0,
// IsPerformingScriptTask 0x007E7450, GetScriptThing 0x007E7490, dtor 0x007E74D0; IsDistanceFromThingToPositionOver
// 0x00CBE45C (__fastcall); CCharString 0x0099EBF0 / 0x0099EAE0; CGameScriptInterface slots 7 NewScriptFrame,
// 8 StartScriptingEntity, 72 GetThingWithScriptName, 108 RemoveThing, 477 SetThingPersistent, 492 EntityAttachToVillage.
// Literals "PopulationVillageEvil" 0x012F13DC, "PopulationVillageGood" 0x012F13C4; floats 0.05f 0x0125A15C, 0.0f
// 0x0122DEDC, 1.0f 0x0122DED8.
// Layout: Ego_r.pdb CRandomPopulationCreature (48 bytes): CEntityScriptBase {vptr, Game +4, Me +8}, ParentClass +0x14,
// PMasterData +0x18, Exit +0x1c, MyType +0x28, AttachedToVillage +0x2c; CV_RandomPopulationSimScript is retail = PDB
// - 0x14: CurrentPopulationScale[4] +0x48, CurrentPopulationRegion +0x5c.
// Shape: the two village arms are identical apart from the literal and VC7.1 cross-jumps them; the landed
// CHTG_FreedomFighter::Main 0x00D701C0 walk loop; the village ~CScriptThing exits sit under `#pragma inline_depth(0)`.
namespace NScript { class CPredicateBase; }
class C3DVector { public: float X, Y, Z; };
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
void operator delete(void* p);                          // 0x00BFE9BC (MSVCR71)
class CBaseClass { public:                                     // vtable 0x01231710
    virtual ~CBaseClass();                              // 0x0099A2E0
    CBaseClass() { }
};
struct CCountBlock { long References; void (__fastcall* DeleteFunc)(void* data); void* Data; };
struct CCountedPointer {                                // CCountedPointer<CScriptThing>: {Data, Info}
    void* Data;
    CCountBlock* Info;
    __forceinline ~CCountedPointer()
    {
        if (Info) {
            --Info->References;
            if (Info->References == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
        Data = 0;
        Info = 0;
    }
};
#define VPAD1(a) virtual void a() const;
#define VPAD2(a) VPAD1(a##0) VPAD1(a##1)
#define VPAD4(a) VPAD2(a##0) VPAD2(a##1)
#define VPAD8(a) VPAD4(a##0) VPAD4(a##1)
#define VPAD16(a) VPAD8(a##0) VPAD8(a##1)
#define VPAD32(a) VPAD16(a##0) VPAD16(a##1)
#define VPAD64(a) VPAD32(a##0) VPAD32(a##1)
#define VPAD128(a) VPAD64(a##0) VPAD64(a##1)
#define VPAD256(a) VPAD128(a##0) VPAD128(a##1)
class CScriptThing : public CBaseClass { public:                      // 12 bytes, vtable 0x01238C8C {vptr, PImp}
    VPAD4(ta) VPAD1(tb)                                 // slots 1..5
    virtual const C3DVector& GetPos() const;            // slot 6 (+0x18)
    VPAD64(tc) VPAD4(td)                                // slots 7..74
    virtual bool IsAlive() const;                       // slot 75 (+0x12c) 0x004AB130
    CCountedPointer PImp;
    CScriptThing(const CScriptThing& other);            // 0x004ABE90
    __forceinline ~CScriptThing() { }                   // out of line 0x004AA840
};
namespace NScript {
class CPredicateBase
{
public:
    virtual void v00();
};
}
namespace NScript {
class CIsThingAlive : public NScript::CPredicateBase { public:                                  // vtable 0x012C32F0
    CScriptThing Thing;
    CIsThingAlive(const CScriptThing& thing) : Thing(thing) { }
    virtual ~CIsThingAlive();                                   // 0x00E241B0
};
}
using NScript::CIsThingAlive;
struct CCountedPointerWords { void* Data; void* Info; };
class CBaseObject { public:
    void* m_pIntelligentPointer;
protected:
    CBaseObject();                                       // 0x0099A380
    virtual ~CBaseObject();                              // 0x0099A430
};
class CScriptGameResourceObjectScriptedThingBase : public CBaseObject { public:   // vtable 0x0127094C
    CCountedPointerWords PImp;
    CScriptGameResourceObjectScriptedThingBase() { PImp.Data = 0; PImp.Info = 0; }
    virtual ~CScriptGameResourceObjectScriptedThingBase();   // 0x007E74D0
    bool IsScripting();                                 // 0x00CD23B9
    void Release();                                     // 0x00CD2770
    void MoveToPosition(const C3DVector& pos, float speed, long moveType, bool a, bool b);   // 0x007E72F0
    bool IsPerformingScriptTask() const;                // 0x007E7450
    CScriptThing GetScriptThing() const;                // 0x007E7490
};
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_NONE };
bool __fastcall IsDistanceFromThingToPositionOver(const CScriptThing& thing, const C3DVector& pos, float distance);   // 0x00CBE45C
class CGameScriptInterface { public:
    VPAD4(pa) VPAD2(pb) VPAD1(pc)                                                    // slots 0..6
    virtual void NewScriptFrame() const;                                             // slot 7 (+0x1c)
    virtual bool StartScriptingEntity(const CScriptThing& thing, CScriptGameResourceObjectScriptedThingBase& object, EScriptAIPriority priority);   // slot 8 (+0x20)
    VPAD32(qa) VPAD16(qb) VPAD8(qc) VPAD4(qd) VPAD2(qe) VPAD1(qf)                    // slots 9..71
    virtual CScriptThing GetThingWithScriptName(const CCharString& name) const;      // slot 72 (+0x120)
    VPAD32(ra) VPAD2(rb) VPAD1(rc)                                                   // slots 73..107
    virtual void RemoveThing(const CScriptThing& thing, bool a, bool b) const;       // slot 108 (+0x1b0)
    VPAD256(sa) VPAD64(sb) VPAD32(sc) VPAD16(sd)                           // slots 109..476
    virtual void SetThingPersistent(const CScriptThing& thing, bool persistent) const;   // slot 477 (+0x774)
    VPAD8(ua) VPAD4(ub) VPAD2(uc)                                                    // slots 478..491
    virtual void EntityAttachToVillage(const CScriptThing& thing, const CScriptThing& village) const;   // slot 492 (+0x7b0)
};
struct CV_RandomPopulationSimScript {
    unsigned char _pad00[0x48];
    float CurrentPopulationScale[4];                    // +0x48
    bool  HighDetailSimulationActive;                   // +0x58
    long  CurrentPopulationRegion;                      // +0x5c
};
class CEntityScriptBase
{
public:
    bool IsActiveThreadTerminating() const;   // 0x00F35B30
    void SetPredicate(const NScript::CPredicateBase&);   // 0x00F35B10
};

struct CRandomPopulationCreature : public CEntityScriptBase {
    void* VTable;
    CGameScriptInterface* Game;                         // +0x04
    CScriptThing Me;                                    // +0x08
    CV_RandomPopulationSimScript* ParentClass;          // +0x14
    void* PMasterData;                                  // +0x18 CQ_SunnyvaleMasterData*
    CScriptThing Exit;                                  // +0x1c
    long MyType;                                        // +0x28
    bool AttachedToVillage;                             // +0x2c
    void Main();
};

void CRandomPopulationCreature::Main()
{
    {
        CIsThingAlive filter(Me);
        SetPredicate(filter);
    }
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    if (!AttachedToVillage) {
        if (IsActiveThreadTerminating())
            return;
        AttachedToVillage = true;
#pragma inline_depth(0)
        if (MyType == 2) {
            if (IsActiveThreadTerminating())
                return;
            CScriptThing village = Game->GetThingWithScriptName("PopulationVillageEvil");
            if (village.IsAlive()) {
                if (IsActiveThreadTerminating())
                    return;
                Game->EntityAttachToVillage(Me, village);
            }
        } else if (MyType == 0 || MyType == 1) {
            if (IsActiveThreadTerminating())
                return;
            CScriptThing village = Game->GetThingWithScriptName("PopulationVillageGood");
            if (village.IsAlive()) {
                if (IsActiveThreadTerminating())
                    return;
                Game->EntityAttachToVillage(Me, village);
            }
        }
#pragma inline_depth()
    }
    Game->SetThingPersistent(Me, false);
    CScriptGameResourceObjectScriptedThingBase handle;
    if (handle.IsScripting())
        handle.Release();
    while (!Game->StartScriptingEntity(Me, handle, (EScriptAIPriority)1)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    {
    C3DVector exitPos = Exit.GetPos();
    while (IsDistanceFromThingToPositionOver(handle.GetScriptThing(), exitPos, 2.0f)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
        handle.MoveToPosition(exitPos, 0.0f, 0, false, true);
        while (handle.IsPerformingScriptTask()) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
    }
    }
    if (IsActiveThreadTerminating())
        return;
    if (MyType != 2) {
        if (IsActiveThreadTerminating())
            return;
        ParentClass->CurrentPopulationScale[ParentClass->CurrentPopulationRegion] -= 0.05f;
        if (ParentClass->CurrentPopulationScale[ParentClass->CurrentPopulationRegion] < 0.0f)
            ParentClass->CurrentPopulationScale[ParentClass->CurrentPopulationRegion] = 0.0f;
        else if (ParentClass->CurrentPopulationScale[ParentClass->CurrentPopulationRegion] > 1.0f)
            ParentClass->CurrentPopulationScale[ParentClass->CurrentPopulationRegion] = 1.0f;
    }
    Game->RemoveThing(Me, false, true);
    do
        Game->NewScriptFrame();
    while (!IsActiveThreadTerminating());
}