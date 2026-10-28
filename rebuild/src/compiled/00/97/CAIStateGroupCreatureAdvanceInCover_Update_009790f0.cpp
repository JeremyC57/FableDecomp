// CAIStateGroup_CreatureAdvanceInCover::Update() -- advance to the next hidden cover position, crouch there when the enemy is alive, then refresh the cover list
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009790F0, length 361 bytes.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CAIStateGroup_CreatureUseCoverBase::GetHiddenCoverPosAndSetUpCoverThing
//          @ 0x00970CD0 (landed), SetFailedToFindCover @ 0x00970590 (manifest "SetAtFailed"), Terminate @ 0x008FCF40, CThing::GetPos
//          @ 0x004C73D0, CCreatureAction_ProjectileCrouch(creature, enemy, const C3DVector&) @ 0x008B8310, AIAction_PerformActionAndWait
//          @ 0x0097DB70, ~CCombatActionBase @ 0x0062E900, MoveIntoCover(C3DVector&) @ 0x009711D0, DuckBehindCover(bool) @ 0x009713E0,
//          CAIStateGroup_CreatureAdvanceInCover::IsStillRunnable @ 0x00978F10 (landed), vector<CIntelligentPointer<CThing> >::erase
//          (first, last) @ 0x004AC0F0 (the inline clear), RefreshCoverList @ 0x009710D0, RemoveThingFromCoverList @ 0x009708A0.
// Globals: 1.8f @ 0x0129885C.
// Shape: the previous cover thing is captured first (null unless alive); a missing cover position fails the group; when the
//          XY distance to the new position (a named float from fsqrt) exceeds 1.8, a creature that had a live cover crouches
//          at the new one while the enemy lives (a dead enemy terminates), then moves into cover and ducks. A group that is
//          no longer runnable clears its cover list; otherwise the list is refreshed, the current cover removed from it, and
//          an empty list terminates. Both IS_THING_ALIVE forms (raw pointer: one GetPItem; intelligent pointer: two) are
//          __forceinline, and the creature comes through an inline GetCreature() accessor.
// Layout: engine/CAIStateGroup_CreatureUseCoverBase.h (pack(1): enemy +0x14, PCoverThing +0x1F, CoverList +0x28) behind
//          engine/CAIStateGroupBase.h.
#define CAIStateGroup_CreatureUseCoverBase CAIStateGroup_CreatureUseCoverBaseLayout
#include "engine/CAIStateGroup_CreatureUseCoverBase.h"
#undef CAIStateGroup_CreatureUseCoverBase

class CAIStateGroupBase
{
public:
    void Terminate();   // 0x008FCF40
};
class CAIStateGroup_EnemyCreatureBase : public CAIStateGroupBase
{
public:
};
class CAIStateGroup_CreatureUseCoverBase : public CAIStateGroup_EnemyCreatureBase, public CAIStateGroup_CreatureUseCoverBaseLayout
{
};

extern "C" double __cdecl sqrt(double);
#pragma intrinsic(sqrt)

class C3DVector { public: float X, Y, Z; };
class C2DVector { public:
    float x, y;
    float GetLength() const { return (float)sqrt(x * x + y * y); }
};

class CThing { public:
    unsigned char _00[0x91];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    const C3DVector& GetPos() const;             // 0x004C73D0
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;
    CThing* GetPItem() const;                   // 0x00A01B50
};
__forceinline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}
__forceinline bool IS_THING_ALIVE(CThing* const& thing)
{
    if (thing != 0) {
        if (!(thing->m_flags91 & 1))
            return true;
    }
    return false;
}
struct CIntelligentPointerVector {              // vector<CIntelligentPointer<CThing> > (retail: first, last, end of storage)
    CBaseIntelligentPointer* first;
    CBaseIntelligentPointer* last;
    CBaseIntelligentPointer* endOfStorage;
    unsigned long size() const { return (unsigned long)(last - first); }
    void erase(CBaseIntelligentPointer* from, CBaseIntelligentPointer* to);   // 0x004AC0F0
    void clear() { erase(first, last); }
};

class CCombatActionBase { public:
    unsigned char _body[0x114 - 4];
    virtual ~CCombatActionBase();                       // 0x0062E900
};
class CCreatureAction_ProjectileCrouch : public CCombatActionBase { public:
    unsigned char _crouch[0x124 - 0x114];       // the crouch action is 0x124 bytes (the frame slot it takes)
    CCreatureAction_ProjectileCrouch(CThing* creature, CThing* enemy, const C3DVector& coverPos);   // 0x008B8310
};

class CAIBrain { public:
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};

struct CAIStateGroupCreatureAdvanceInCoverMethods : CAIStateGroup_CreatureUseCoverBase {
    CAIBrain* Brain() const { return *(CAIBrain* const*)_pad_0x04; }
    CThing* GetCreature() const { return Brain()->PCreature; }
    CBaseIntelligentPointer& Enemy() { return *(CBaseIntelligentPointer*)_pad_0x14; }
    CBaseIntelligentPointer& Cover() { return *(CBaseIntelligentPointer*)PCoverThing; }
    CIntelligentPointerVector& Candidates() { return *(CIntelligentPointerVector*)CoverList; }
    bool GetHiddenCoverPosAndSetUpCoverThing(C3DVector& result);   // 0x00970CD0
    void SetFailedToFindCover();                // 0x00970590
    bool MoveIntoCover(C3DVector& pos);         // 0x009711D0
    bool DuckBehindCover(bool dive);            // 0x009713E0
    bool IsStillRunnable();                     // 0x00978F10
    void RefreshCoverList();                    // 0x009710D0
    void RemoveThingFromCoverList(CThing* thing);   // 0x009708A0
    long AIAction_PerformActionAndWait(CCombatActionBase* action, bool stopWhenReplaced);   // 0x0097DB70
    void Update();
};

void CAIStateGroupCreatureAdvanceInCoverMethods::Update()
{
    CThing* previousCover = 0;
    if (IS_THING_ALIVE(Cover().GetPItem()))
        previousCover = Cover().GetPItem();
    C3DVector coverPos;
    if (!GetHiddenCoverPosAndSetUpCoverThing(coverPos)) {
        SetFailedToFindCover();
        Terminate();
        return;
    }
    const C3DVector& pos = GetCreature()->GetPos();
    C2DVector delta;
    delta.x = coverPos.X - pos.X;
    delta.y = coverPos.Y - pos.Y;
    float distance = delta.GetLength();
    if (distance > 1.8f) {
        if (IS_THING_ALIVE(previousCover)) {
            if (!IS_THING_ALIVE(Enemy())) {
                Terminate();
                return;
            }
            CThing* creature = GetCreature();
            AIAction_PerformActionAndWait(&CCreatureAction_ProjectileCrouch(creature, Enemy().GetPItem(), coverPos), false);
        }
        if (MoveIntoCover(coverPos))
            DuckBehindCover(true);
    }
    if (!IsStillRunnable()) {
        Candidates().clear();
        Terminate();
        return;
    }
    RefreshCoverList();
    RemoveThingFromCoverList(Cover().GetPItem());
    if (Candidates().size() == 0)
        Terminate();
}