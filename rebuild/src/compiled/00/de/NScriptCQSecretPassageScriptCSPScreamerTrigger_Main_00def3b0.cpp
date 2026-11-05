// NScript::CQ_SecretPassageScript::CSP_ScreamerTrigger::Main() -- the secret-passage screamer trigger: wait until the hero is within 10 units of Me, then spawn as many undead sword soldiers as Me's data string says (each near a random "SP_ScreamerSpawnPoint", named "EnemySeekingHero" and set on the hero), and wait until every "EnemySeekingHero" is dead
// Leaf: ?Main@CSP_ScreamerTrigger@CQ_SecretPassageScript@NScript@@EAEXXZ
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8). Also: the non-template local CCountedPointer holds a CScriptThingImp* (its first member): it is CCountedPointer<CScriptThingImp>.
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00DEF3B0, length 616 bytes, plain `ret`.
// Callees: IsDistanceBetweenThingsUnder 0x00CBE2FF and AreAllThingsInVectorDead 0x00CBED00 (both __fastcall),
// GFCharStringToInt 0x0099E7F0 (__fastcall), IsActiveThreadTerminating 0x00F35B30, CCharString 0x0099EBF0 / 0x0099EAE0,
// CBaseClass() 0x0099A2D0, ~CScriptThing 0x004AA840, operator delete 0x00BFE9BC, vector<CScriptThing> dtor 0x008AC970
// (manifest label ~vector<CConversation> is propagated across byte-identical bodies); CScriptThing slot 3
// GetDataString (virtual on Me) and the inline slot 6 GetPos; CGameScriptInterface slots 7 NewScriptFrame, 70 GetHero,
// 74 GetRandomThingWithScriptName, 75 GetAllThingsWithScriptName, 92 CreateCreatureNearby, 451 GiveThingBestEnemyTarget.
// Literals "SP_ScreamerSpawnPoint" 0x012DDF24, "EnemySeekingHero" 0x012DDD98, "CREATURE_OK_UNDEAD_SOLDIER_SWORD"
// 0x0127D594; the zero vector 0x0143E8E0.
// Layout: Ego_r.pdb CSP_ScreamerTrigger: CEntityScriptBase {vptr, Game +4, Me +8}, ParentClass +0x14, PMasterData +0x18.
// Shape: the landed CHTG_AmbushPointBackup::Main 0x00D73C70 (CScriptThing with the inline CCountedPointer copy-assign and
// inline GetPos); the loop counter starts at 0 and doubles as the zero source of the two default constructions.
// Naming wave 35: identity reg 00decd7c "SP_ScreamerTrigger" in 00decad0; Alloc 00ded1a0; vtable 012ddd44 slot 1; PDB ?Main@CSP_ScreamerTrigger@CQ_SecretPassageScript@NScript@@; refs=1; Anniversary RTTI agrees (confirmed); was under-qualified CSP_ScreamerTrigger::Main (wave3/labels_alloc.tsv); PDB ?Main@CSP_ScreamerTrigger@CQ_SecretPassageScript@NScript@@EAEXXZ in Ego_r.pdb / FableWin.pdb.
//   Was `?Main@CSP_ScreamerTrigger@@QAEXXZ`; this source now defines the PDB member (vptr placeholder dropped (own, base CEntityScriptBase made polymorphic)); same bytes.
namespace NScript { class CPredicateBase; }
#include "engine/C3DVector.h"
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
long __fastcall GFCharStringToInt(const CCharString& text);   // 0x0099E7F0
void operator delete(void* p);                      // 0x00BFE9BC (MSVCR71)
struct CScriptThingImp {
    virtual void i0(); virtual void i1(); virtual void i2(); virtual void i3(); virtual void i4(); virtual void i5();
    virtual const C3DVector& GetPosition() const;   // slot 6 (+0x18)
};
#include "engine/CCountedPointer.h"
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0 (slot 0), implicit copy
class CScriptThing : public CBaseClass { public:                  // vtable 0x01238C8C
protected:
    static C3DVector NullCoord;                        // 0x0143E8E0 ?NullCoord@CScriptThing@@1VC3DVector@@A (Ego_r.pdb; engine/CScriptThing.h)
public:
    CCountedPointer<CScriptThingImp> PImp;
    CScriptThing() { }
    ~CScriptThing();                                // 0x004AA840
    CScriptThing& operator=(const CScriptThing& other) { PImp = other.PImp; return *this; }
    virtual void v1(); virtual void v2();
    virtual CCharString GetDataString() const;      // slot 3 (+0x0c)
    virtual void v4(); virtual void v5();
    virtual const C3DVector& GetPos() const { return !PImp.Data ? NullCoord : PImp.Data->GetPosition(); }   // slot 6
};
struct CScriptThingVector {                         // STLport vector<CScriptThing>, 12 bytes
    CScriptThing* _M_start;
    CScriptThing* _M_finish;
    CScriptThing* _M_end_of_storage;
    CScriptThingVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~CScriptThingVector();                          // 0x008AC970
};
bool __fastcall IsDistanceBetweenThingsUnder(const CScriptThing& a, const CScriptThing& b, float distance);   // 0x00CBE2FF
bool __fastcall AreAllThingsInVectorDead(const CScriptThingVector& things);   // 0x00CBED00
#define VPAD1(a) virtual void a() const;
#define VPAD2(a) VPAD1(a##0) VPAD1(a##1)
#define VPAD4(a) VPAD2(a##0) VPAD2(a##1)
#define VPAD8(a) VPAD4(a##0) VPAD4(a##1)
#define VPAD16(a) VPAD8(a##0) VPAD8(a##1)
#define VPAD32(a) VPAD16(a##0) VPAD16(a##1)
#define VPAD64(a) VPAD32(a##0) VPAD32(a##1)
#define VPAD128(a) VPAD64(a##0) VPAD64(a##1)
#define VPAD256(a) VPAD128(a##0) VPAD128(a##1)
class CGameScriptInterface { public:
    VPAD4(pa) VPAD2(pb) VPAD1(pc)                                                    // slots 0..6
    virtual void NewScriptFrame() const;                                             // slot 7 (+0x1c)
    VPAD32(qa) VPAD16(qb) VPAD8(qc) VPAD4(qd) VPAD2(qe)                              // slots 8..69
    virtual CScriptThing& GetHero();                                                 // slot 70 (+0x118)
    VPAD2(ra) VPAD1(rb)                                                              // slots 71..73
    virtual CScriptThing GetRandomThingWithScriptName(const CCharString& name) const;   // slot 74 (+0x128)
    virtual long GetAllThingsWithScriptName(const CCharString& name, CScriptThingVector& things) const;   // slot 75 (+0x12c)
    VPAD16(sa)                                                                       // slots 76..91
    virtual CScriptThing CreateCreatureNearby(const CCharString& def, const C3DVector& pos, float radius, const CCharString& scriptName, bool a) const;   // slot 92 (+0x170)
    VPAD256(ta) VPAD64(tb) VPAD32(tc) VPAD4(td) VPAD2(te)                           // slots 93..450
    virtual void GiveThingBestEnemyTarget(const CScriptThing& thing, const CScriptThing& target) const;   // slot 451 (+0x70c)
};
class CEntityScriptBase
{
    virtual ~CEntityScriptBase();   // retail slot 0 (the stand-in is polymorphic, as retail: the vptr moves here)

public:
    bool IsActiveThreadTerminating() const;   // 0x00F35B30
};

namespace NScript {
class CQ_SecretPassageScript {    // the PDB owner script; only its nested entity class is modelled here
public:
    class CSP_ScreamerTrigger;
};
}
using NScript::CQ_SecretPassageScript;
namespace NScript {
class CQ_SecretPassageScript::CSP_ScreamerTrigger : public CEntityScriptBase {
public:
        CGameScriptInterface* Game;                     // +0x04
    CScriptThing Me;                                // +0x08
    void* ParentClass;                              // +0x14 CQ_SecretPassageScript*
    void* PMasterData;                              // +0x18
private:
    virtual void Main();                          // PDB ?Main@CSP_ScreamerTrigger@CQ_SecretPassageScript@NScript@@EAEXXZ
};
}

void NScript::CQ_SecretPassageScript::CSP_ScreamerTrigger::Main()
{
    while (!IsDistanceBetweenThingsUnder(Game->GetHero(), Me, 10.0f)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    long count = GFCharStringToInt(Me.GetDataString());
    CScriptThing spawnPoint;
    CScriptThingVector enemies;
    for (long i = 0; i < count; ++i) {
        if (IsActiveThreadTerminating())
            return;
        spawnPoint = Game->GetRandomThingWithScriptName("SP_ScreamerSpawnPoint");
        CScriptThing enemy = Game->CreateCreatureNearby("CREATURE_OK_UNDEAD_SOLDIER_SWORD", spawnPoint.GetPos(), 5.0f, "EnemySeekingHero", true);
        Game->GiveThingBestEnemyTarget(enemy, Game->GetHero());
    }
    if (IsActiveThreadTerminating())
        return;
    Game->GetAllThingsWithScriptName("EnemySeekingHero", enemies);
    while (!AreAllThingsInVectorDead(enemies)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    IsActiveThreadTerminating();
}