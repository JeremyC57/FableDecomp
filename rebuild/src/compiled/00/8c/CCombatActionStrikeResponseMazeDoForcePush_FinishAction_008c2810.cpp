// CCombatAction_StrikeResponseMazeDoForcePush::FinishAction() -- base FinishAction; when Maze (a boss past phase 0) is struck from within 4 units, count the hit: once the count passes the def's tutorial hit limit (or in phase 2) he answers with a CCombatAction_WillForcePushInto, otherwise the counter is advanced
// Leaf: ?FinishAction@CCombatAction_StrikeResponseMazeDoForcePush@@UAEXXZ
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Plan phase 2 (2026-11-04), CVectorMap wave (stand-in map): the local CInterfaceMap stand-in (and its same-TU LowerBound body) replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<..> > derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D; the same-TU noinline LowerBound is the header's); entries read as pair first/second, the map end through end(). Spellings unified: 0040f020: ?LowerBound@CInterfaceMap@@QAEPAUSEntry@1@PBJ@Z -> ?LowerBound@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@V?$CKeyPairCompareLess@W4ETCInterfaceType@@PAVCTCBase@@@@@@QAEPAU?$pair@W4ETCInterfaceType@@PAVCTCBase@@@std@@ABW4ETCInterfaceType@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x008C2810, 354 bytes (368 with padding), virtual FinishAction.
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50, CVectorMap::LowerBound 0x0040F020 (inlined GetInterface 0xAD =
//   CTCBoss, bit 13 of +0x34; result ignored, the absent arm reads the shared out word), CDefClassBase::GetSubDef
//   <CMazeBattleDef> 0x008C3510 (landed), CCombatAction_GenericStrikeResponseBase::FinishAction 0x008663D0, CThing::
//   GetPos 0x004C73D0, CTCBoss::GetCurrentPhase 0x007AED10 (catalog label CWorld::DrawGetEnvironment is a folded
//   name), 4.0f 0x0123088C, CCombatAction_WillForcePushInto(CThingCreatureBase*) 0x008C38B0 (vtable 0x012804FC RTTI; the
//   catalog label CCombatAction_Guildmaster_HealFollowersInto is wrong), CThingCreatureBase::SetCurrentAction 0x006644F0,
//   ~CCombatActionBase 0x0062E900; the def released inline (dec [def+4]; vcall slot 1).
// Shape: the unchecked GetInterface, then the def through an inline CThing::GetSubDef wrapper (the thing is fetched before
//   the out pointer is pushed); the hit counter read once after the base FinishAction; a C3DVector offset whose dead Z keeps
//   its frame word, its 2D length compared against 4.0f. Same-TU stand-ins for GetSubDef and LowerBound (auto_inline(off))
//   let the def stay in edi and the lookup key share the def's frame word.
// Layout: local model -- retail CCombatActionBase is 0x114 bytes (PCreature +0x08), the attacker pointer +0x114; CTCBoss
//   GenericCounter +0x10 / currentBossPhase +0x18 (Ego_r.pdb); CMazeBattleDef TutorialNumGetHits retail +0x50 (PDB +0x54,
//   compared as a long).
// Name fix (agentNF6 2026-11-10): CTCBoss::GetBossPhase 0x007AED10 is ?GetBossPhase@CTCBoss@@QBEJXZ (Ego_r.pdb / FableWin.pdb), was GetCurrentPhase / non-const.
#include "fable_compiler.h"
#include <math.h>
#pragma intrinsic(sqrt)
class CTCBase;
class CDefPointeeBase { public:
    virtual void v000();
    virtual void DestroyDef();                  // slot 1 (+4)
    long m_refCount;                            // +4
};
class CMazeBattleDef : public CDefPointeeBase { public:
    unsigned char _pad08[0x50 - 0x08];
    long TutorialNumGetHits;                    // +0x50 (PDB +0x54)
};
struct CMazeBattleDefPointer {                  // CDefPointer<const CMazeBattleDef>
    CMazeBattleDef* Object;
    CMazeBattleDefPointer() : Object(0) {}
    ~CMazeBattleDefPointer()
    {
        if (Object && --Object->m_refCount == 0)
            Object->DestroyDef();
    }
    CMazeBattleDef* operator->() const { return Object; }
};
class CDefClassBase : public CDefPointeeBase { public:
    bool GetSubDef(CMazeBattleDefPointer& out) const;   // 0x008C3510
};
void* FindDef_GetSubDef_008c3510();             // stand-in callee of the stand-in body below
#pragma auto_inline(off)
bool CDefClassBase::GetSubDef(CMazeBattleDefPointer& out) const
{
    void* def = FindDef_GetSubDef_008c3510();
    if (!def)
        return false;
    *(void**)&out = def;
    return true;
}
#pragma auto_inline(on)
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class C3DVector { public:
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    float Get2DLength() const { return sqrtf(X * X + Y * Y); }
};
class C2DVector { public:
    float X, Y;
    C2DVector(float x, float y) : X(x), Y(y) {}
    float GetLength() const { return sqrtf(X * X + Y * Y); }
};
class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad54[0x1c];
    CDefClassBase* PBaseDef;                    // +0x70 CDefPointer<const CDefClassBase> object word
    unsigned char _pad74[0x91 - 0x74];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    const C3DVector& GetPos() const;       // 0x004C73D0
    template<class T> FABLE_FORCEINLINE bool GetSubDef(T& out) const { return PBaseDef->GetSubDef(out); }
    template<class T> FABLE_FORCEINLINE bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::value_type* it = map->LowerBound((const ETCInterfaceType&)type);
            if (it == map->end() || interfaceType < it->first)
                it = map->end();
            *ppOut = (T*)it->second;
            return true;
        }
        return false;
    }
};
class CCombatActionBase;
class CThingCreatureBase : public CThing { public:
    bool SetCurrentAction(const CCombatActionBase& action);   // 0x006644F0
};
class CTCBoss { public:                                // interface type 0xAD
    unsigned char _pad00[0x10];
    long GenericCounter;                        // +0x10
    long GetBossPhase() const;                     // 0x007AED10 (currentBossPhase +0x18)
};
class CBaseObject;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
struct CCreaturePointer : CBaseIntelligentPointer {                       // CIntelligentPointer<CThingCreatureBase>
    CThingCreatureBase* GetPItem() const { return (CThingCreatureBase*)CBaseIntelligentPointer::GetPItem(); }   // inline over 0x00A01B50
};
class CCombatActionBase { public:                      // retail sizeof 0x114
    virtual void v00();
    unsigned char _pad04[4];
    CCreaturePointer PCreature;                 // +0x08
    unsigned char _pad10[0x114 - 0x10];
    virtual ~CCombatActionBase();                       // 0x0062E900
};
class CCombatAction_WillForcePushInto : public CCombatActionBase { public:   // vtable 0x012804FC
    CCombatAction_WillForcePushInto(CThingCreatureBase* owner);   // 0x008C38B0
};
class CCombatAction_GenericStrikeResponseBase : public CCombatActionBase { public:
    void FinishAction();                        // 0x008663D0
};
class CCombatAction_StrikeResponseMazeDoForcePush : public CCombatAction_GenericStrikeResponseBase { public:
    CBaseIntelligentPointer PAttacker;          // +0x114

public:
    virtual void FinishAction();
};

void CCombatAction_StrikeResponseMazeDoForcePush::FinishAction()
{
    CTCBoss* boss;
    PCreature.GetPItem()->GetInterface(&boss, 0xAD);
    CMazeBattleDefPointer battleDef;
    PCreature.GetPItem()->GetSubDef(battleDef);
    CThingCreatureBase* creature = PCreature.GetPItem();
    CCombatAction_GenericStrikeResponseBase::FinishAction();
    long hits = boss->GenericCounter + 1;
    if (PAttacker.GetPItem() && !(((CThing*)PAttacker.GetPItem())->m_flags91 & 1)) {
        CThingCreatureBase* me = PCreature.GetPItem();
        const C3DVector& attackerPosition = ((CThing*)PAttacker.GetPItem())->GetPos();
        const C3DVector& position = me->GetPos();
        C3DVector offset(position.X - attackerPosition.X, position.Y - attackerPosition.Y, position.Z - attackerPosition.Z);
        if (boss->GetBossPhase() > 0 && offset.Get2DLength() < 4.0f) {
            if (hits > battleDef->TutorialNumGetHits || boss->GetBossPhase() == 2)
                creature->SetCurrentAction(CCombatAction_WillForcePushInto(creature));
            else
                boss->GenericCounter = hits;
        }
    }
}