// 009385f0 behaviour test: CAIStateGroup_UsesSeat::MoveToSeat (control flow + data of the final.cpp body)
#ifndef _MSC_VER
#define __forceinline inline
#endif
#include <stdio.h>
#include <string.h>

struct C3DVector {
    float x, y, z;
    C3DVector() { }
    C3DVector(float ax, float ay, float az) : x(ax), y(ay), z(az) { }
    C3DVector operator+(const C3DVector& o) const { return C3DVector(x + o.x, y + o.y, z + o.z); }
    C3DVector(const C3DVector& o) : x(o.x), y(o.y), z(o.z) { }
};
struct CRightHandedSet { C3DVector Up; C3DVector Forward; };

struct CTCUserActionPointBase;
static CTCUserActionPointBase* g_point;
static int g_hasInterface;
struct CThing {
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (interfaceType == 0x2d && g_hasInterface) { *ppOut = (T*)g_point; return true; }
        return false;
    }
};
struct CThingCreature : CThing {
    C3DVector Pos;
    const C3DVector& GetPhysicsPosition() const { return Pos; }
};
static int g_freeOk;
static CThing* g_freeUser;
struct CTCUserActionPointBase {
    int dummy;
    bool GetFreeActionPoint(CThing* user, C3DVector* pos, CRightHandedSet* orientation, long* index)
    {
        g_freeUser = user;
        if (!g_freeOk) return false;
        *pos = C3DVector(1.0f, 2.0f, 3.0f);
        orientation->Forward = C3DVector(10.0f, 20.0f, 30.0f);
        *index = 7;
        return true;
    }
};
struct CIntelligentPointerThingObject {
    CThing* m_pItem;
    CThing* GetPItem() const { return m_pItem; }
};
struct CAIBrain { CThingCreature* PCreature; };

static int g_validCalls, g_invalidAt;          // IsSeatValid() false from call g_invalidAt on (0 = never)
static long g_moveResults[4]; static int g_moveCalls;
static long g_lookResults[4]; static int g_lookCalls;
static long g_moveMovement; static C3DVector g_movePos;
static C3DVector g_lookPos; static long g_lookFlags;

struct CAIStateGroupBase {
    CAIBrain* PBrain;
    bool IsSeatValid() { ++g_validCalls; return !(g_invalidAt && g_validCalls >= g_invalidAt); }
    long AIAction_MoveToPos(long repeat, C3DVector pos, bool* reached, long movement, bool, bool, CThing*, bool, float)
    {
        *reached = true; g_moveMovement = movement; g_movePos = pos; (void)repeat;
        return g_moveResults[g_moveCalls++];
    }
    long AIAction_LookAtPos(long, C3DVector pos, long flags, float)
    {
        g_lookPos = pos; g_lookFlags = flags;
        return g_lookResults[g_lookCalls++];
    }
};
struct CAIStateGroup_UsesSeat : CAIStateGroupBase {
    CIntelligentPointerThingObject PSeat;
    bool GotSeatPos;
    long SeatPosIndex;
    C3DVector SeatPos;
    CThingCreature* GetCreature() const { return PBrain->PCreature; }
    bool MoveToSeat(long movement);
};

bool CAIStateGroup_UsesSeat::MoveToSeat(long movement)
{
    if (PSeat.GetPItem() == 0 || !IsSeatValid())
        return false;
    CRightHandedSet orientation;
    {
        CTCUserActionPointBase* point;
        PSeat.GetPItem()->GetInterface(&point, 0x2d);
        if (!point->GetFreeActionPoint(GetCreature(), &SeatPos, &orientation, &SeatPosIndex))
            return false;
    }
    GotSeatPos = true;
    long result;
    {
        bool reached;
        do {
            if (PSeat.GetPItem() == 0 || !IsSeatValid())
                return false;
            result = AIAction_MoveToPos(2, SeatPos, &reached, movement, true, true, 0, true, 0.0f);
        } while (result == 4);
    }
    if (result != 2)
        return false;
    if (PSeat.GetPItem() == 0 || !IsSeatValid())
        return false;
    C3DVector forward = orientation.Forward;
    C3DVector lookPos = forward + PBrain->PCreature->GetPhysicsPosition();
    do {
        if (PSeat.GetPItem() == 0 || !IsSeatValid())
            return false;
        result = AIAction_LookAtPos(2, lookPos, 0x11, 0.0027777f);
    } while (result == 4);
    if (result != 2)
        return false;
    if (PSeat.GetPItem() == 0 || !IsSeatValid())
        return false;
    return true;
}

static int g_fail;
static void Check(const char* name, bool cond) { if (!cond) { printf("FAIL %s\n", name); g_fail = 1; } }

static bool Run(bool haveSeat, int freeOk, int invalidAt, long m0, long m1, long l0, long l1,
    CAIStateGroup_UsesSeat& g, CThingCreature& creature, CThing& seat)
{
    g.PBrain->PCreature = &creature;
    g.PSeat.m_pItem = haveSeat ? &seat : 0;
    g.GotSeatPos = false; g.SeatPosIndex = -1; g.SeatPos = C3DVector(0, 0, 0);
    g_freeOk = freeOk; g_invalidAt = invalidAt; g_validCalls = 0;
    g_moveResults[0] = m0; g_moveResults[1] = m1; g_moveCalls = 0;
    g_lookResults[0] = l0; g_lookResults[1] = l1; g_lookCalls = 0;
    g_lookPos = C3DVector(0, 0, 0); g_moveMovement = 0; g_freeUser = 0;
    return g.MoveToSeat(5);
}

int main()
{
    CTCUserActionPointBase point; point.dummy = 0; g_point = &point; g_hasInterface = 1;
    CThingCreature creature; creature.Pos = C3DVector(100.0f, 200.0f, 300.0f);
    CThing seat;
    CAIBrain brain; brain.PCreature = &creature;
    CAIStateGroup_UsesSeat g; g.PBrain = &brain;

    Check("no seat", !Run(false, 1, 0, 2, 2, 2, 2, g, creature, seat) && g_validCalls == 0 && !g.GotSeatPos);
    Check("invalid seat", !Run(true, 1, 1, 2, 2, 2, 2, g, creature, seat) && g_moveCalls == 0);
    Check("no free point", !Run(true, 0, 0, 2, 2, 2, 2, g, creature, seat) && !g.GotSeatPos && g_moveCalls == 0
        && g_freeUser == &creature);
    bool ok = Run(true, 1, 0, 2, 2, 2, 2, g, creature, seat);
    Check("happy", ok && g.GotSeatPos && g.SeatPosIndex == 7 && g_moveCalls == 1 && g_lookCalls == 1
        && g_moveMovement == 5 && g_movePos.x == 1.0f && g_movePos.z == 3.0f
        && g_lookPos.x == 110.0f && g_lookPos.y == 220.0f && g_lookPos.z == 330.0f && g_lookFlags == 0x11
        && g_validCalls == 5);
    Check("move retry", Run(true, 1, 0, 4, 2, 2, 2, g, creature, seat) && g_moveCalls == 2 && g_validCalls == 6);
    Check("move fails", !Run(true, 1, 0, 3, 2, 2, 2, g, creature, seat) && g_lookCalls == 0 && g.GotSeatPos);
    Check("look retry", Run(true, 1, 0, 2, 2, 4, 2, g, creature, seat) && g_lookCalls == 2);
    Check("look fails", !Run(true, 1, 0, 2, 2, 1, 2, g, creature, seat) && g_lookCalls == 1);
    Check("invalid in move loop", !Run(true, 1, 2, 2, 2, 2, 2, g, creature, seat) && g_moveCalls == 0);
    Check("invalid after move", !Run(true, 1, 3, 2, 2, 2, 2, g, creature, seat) && g_moveCalls == 1 && g_lookCalls == 0);
    Check("invalid at end", !Run(true, 1, 5, 2, 2, 2, 2, g, creature, seat) && g_lookCalls == 1);
    if (g_fail) return 1;
    printf("009385f0_TEST PASS\n");
    return 0;
}