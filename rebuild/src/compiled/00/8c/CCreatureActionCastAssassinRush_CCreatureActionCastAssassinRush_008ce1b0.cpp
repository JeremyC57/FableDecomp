// CCreatureAction_CastAssassinRush::CCreatureAction_CastAssassinRush(CThingCreatureBase&, const C3DVector&, float, bool, long, CCountedPointer<CCreatureActionBase>) -- override form: base CCombatActionBase(owner, 150, zeroed sound), Override set, Dir, Distance, UseCollision, Level, FinalAction, IsZTargetting clear; animation "ST_CAST_ASSASSIN_RUSH"
// Leaf: ??0CCreatureAction_CastAssassinRush@@QAE@AAVCThingCreatureBase@@ABVC3DVector@@M_NJV?$CCountedPointer@VCCreatureActionBase@@@@@Z
// Name (agentNF32, 2026-11-12): Ego_r.pdb 0x00905670 + FableWin.pdb 0x029F92D0 (same mangled symbol): owner by reference and a
//   CCountedPointer<CCreatureActionBase> by value (engine/CCountedPointer.h); row 0x009735D0 RushToPos binds this symbol.
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008CE1B0, length 221 bytes (+ int3 padding), `ret 1Ch`. Vtable 0x01284254.
// Callees: the CCombatActionBase constructor (0x00858030 owner form / 0x00857F30 owner+target form), CCharString ctor 0x0099EBF0 /
// dtor 0x0099EAE0 ("ST_CAST_ASSASSIN_RUSH" @ 0x01284238), SetAnimationName 0x008574D0, operator delete 0x00BFE9BC (the by-value
// FinalAction's count block when its last reference goes).
// Shape: init-list members in declaration order (Override, Dir, Distance, Level, FinalAction copy -- `inc [Info]` when non-null --,
// UseCollision, IsZTargetting); the scoped named animation string; the by-value CCountedPointer<CCreatureActionBase> parameter is
// destroyed by the callee at the end (dec, destroy through the block's pointer-to-member, delete the block).
// Layout: byte-packed like the landed copy constructor 0x008CEF50 (PDB header + 0x18 = retail offsets in the comments).
class CThingCreatureBase;
class CThing;
class CCreatureActionBase;

#include "engine/CCountedPointer.h"

#include "engine/C3DVector.h"

class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                             // 0x0099EAE0
};

class CSoundPair { public:                         // two sound ids, zeroed by the inline constructor
    long FirstSound;
    long LastSound;
    CSoundPair() : FirstSound(0), LastSound(0) { }
};

class CCombatActionBase { public:                  // retail sizeof 0x114
    virtual void v00();
    unsigned char _body[0x114 - 4];
    CCombatActionBase(CThingCreatureBase* owner, long actionCode, const CSoundPair& sound);                     // 0x00858030
    CCombatActionBase(CThingCreatureBase* owner, CThing* target, long actionCode, const CSoundPair& sound);     // 0x00857F30
public:
    void SetCombatAnimation(const CCharString&);   // 0x008574D0
};

#pragma pack(push, 1)
class CCreatureAction_CastAssassinRush : public CCombatActionBase { public:   // vtable 0x01284254; retail sizeof 0x138
    bool          Override;                 // +0x114 (PDB +0x12c)
    C3DVector     Dir;                      // +0x115 (PDB +0x12d), unaligned
    unsigned char _pad121[3];               // +0x121
    float         Distance;                 // +0x124 (PDB +0x13c)
    long          Level;                    // +0x128 (PDB +0x140)
    CCountedPointer<CCreatureActionBase> FinalAction;   // +0x12c (PDB +0x144)
    bool          UseCollision;             // +0x134 (PDB +0x14c)
    bool          IsZTargetting;            // +0x135 (PDB +0x14d)
    unsigned char _pad136[2];               // +0x136
    virtual void v00();
    CCreatureAction_CastAssassinRush(CThingCreatureBase* owner, CThing* target, bool isZTargetting, long level, CCountedPointer<CCreatureActionBase> finalAction);
    CCreatureAction_CastAssassinRush(CThingCreatureBase* owner, long level, CCountedPointer<CCreatureActionBase> finalAction);
    CCreatureAction_CastAssassinRush(CThingCreatureBase& owner, const C3DVector& dir, float distance, bool useCollision, long level, CCountedPointer<CCreatureActionBase> finalAction);
};
#pragma pack(pop)

CCreatureAction_CastAssassinRush::CCreatureAction_CastAssassinRush(CThingCreatureBase& owner, const C3DVector& dir, float distance, bool useCollision, long level, CCountedPointer<CCreatureActionBase> finalAction)
    : CCombatActionBase(&owner, 150, CSoundPair())
    , Override(true)
    , Dir(dir)
    , Distance(distance)
    , Level(level)
    , FinalAction(finalAction)
    , UseCollision(useCollision)
    , IsZTargetting(false)
{
    {
        CCharString name("ST_CAST_ASSASSIN_RUSH");
        SetCombatAnimation(name);
    }
}