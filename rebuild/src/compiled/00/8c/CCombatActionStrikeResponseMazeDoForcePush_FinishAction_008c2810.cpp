// CCombatAction_StrikeResponseMazeDoForcePush::FinishAction() -- base FinishAction; when Maze (a boss past phase 0) is struck from within 4 units, count the hit: once the count passes the def's tutorial hit limit (or in phase 2) he answers with a CCombatAction_WillForcePushInto, otherwise the counter is advanced
// Leaf: ?FinishAction@CCombatAction_StrikeResponseMazeDoForcePush@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008C2810, 354 bytes (368 with padding), virtual FinishAction.
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50, CVectorMap::LowerBound 0x0040F020 (inlined GetInterface 0xAD =
//   CTCBoss, bit 13 of +0x34; result ignored, the absent arm reads the shared out word), CDefClassBase::GetSubDef
//   <CMazeBattleDef> 0x008C3510 (landed), CCombatAction_GenericStrikeResponseBase::FinishAction 0x008663D0, CThing::
//   GetPosition 0x004C73D0, CTCBoss::GetCurrentPhase 0x007AED10 (catalog label CWorld::DrawGetEnvironment is a folded
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
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
#pragma auto_inline(off)
// Same-TU stand-in for CVectorMap::LowerBound 0x0040F020 (its retail body: STLport lower_bound over the 8-byte
// {type, component} entries): compiled above the caller so VC7.1 knows the key's address does not escape, which lets
// the key share its frame word with later locals (retail's LTCG knowledge).
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    SEntry* it = first;
    int len = int(last - first);
    while (len > 0) {
        int half = len / 2;
        SEntry* middle = it + half;
        if (middle->type < *type) {
            it = middle + 1;
            len = len - half - 1;
        } else
            len = half;
    }
    return it;
}
#pragma auto_inline(on)
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
    unsigned char _pad4c[0x70 - 0x4c];
    CDefClassBase* PBaseDef;                    // +0x70 CDefPointer<const CDefClassBase> object word
    unsigned char _pad74[0x91 - 0x74];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    const C3DVector& GetPosition() const;       // 0x004C73D0
    template<class T> __forceinline bool GetSubDef(T& out) const { return PBaseDef->GetSubDef(out); }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
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
    long GetCurrentPhase();                     // 0x007AED10 (currentBossPhase +0x18)
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +4
    CThing* GetPItem() const;                   // 0x00A01B50
};
struct CCreaturePointer {                       // CIntelligentPointer<CThingCreatureBase>
    virtual void v00();
    CThingCreatureBase* m_pItem;                // +4
    CThingCreatureBase* GetPItem() const;       // 0x00A01B50
};
class CCombatActionBase { public:                      // retail sizeof 0x114
    virtual void v00();
    unsigned char _pad04[4];
    CCreaturePointer PCreature;                 // +0x08
    unsigned char _pad10[0x114 - 0x10];
    ~CCombatActionBase();                       // 0x0062E900
};
class CCombatAction_WillForcePushInto : public CCombatActionBase { public:   // vtable 0x012804FC
    CCombatAction_WillForcePushInto(CThingCreatureBase* owner);   // 0x008C38B0
};
class CCombatAction_GenericStrikeResponseBase : public CCombatActionBase { public:
    void FinishAction();                        // 0x008663D0
};
class CCombatAction_StrikeResponseMazeDoForcePush : public CCombatAction_GenericStrikeResponseBase { public:
    CBaseIntelligentPointer PAttacker;          // +0x114
    void FinishAction();
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
    if (PAttacker.GetPItem() && !(PAttacker.GetPItem()->m_flags91 & 1)) {
        CThingCreatureBase* me = PCreature.GetPItem();
        const C3DVector& attackerPosition = PAttacker.GetPItem()->GetPosition();
        const C3DVector& position = me->GetPosition();
        C3DVector offset(position.X - attackerPosition.X, position.Y - attackerPosition.Y, position.Z - attackerPosition.Z);
        if (boss->GetCurrentPhase() > 0 && offset.Get2DLength() < 4.0f) {
            if (hits > battleDef->TutorialNumGetHits || boss->GetCurrentPhase() == 2)
                creature->SetCurrentAction(CCombatAction_WillForcePushInto(creature));
            else
                boss->GenericCounter = hits;
        }
    }
}