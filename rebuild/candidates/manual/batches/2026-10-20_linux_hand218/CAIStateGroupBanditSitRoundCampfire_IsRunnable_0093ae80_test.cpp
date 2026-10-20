// Behaviour test for 0x0093AE80 CAIStateGroup_BanditSitRoundCampfire::IsRunnable (model copy over stubs).
#include <cstdio>
struct CThing { unsigned long bits74; bool hasMember; void* member; unsigned char m_flags91; };
static long g_frame = 100;
long GFGetCurrentFrame() { return g_frame; }
long g_framesPerSecond = 30;
struct CTCVillage { bool curfew; bool IsCurfewTime() { return curfew; } };
struct CTCVillageMember { CTCVillage* village; CTCVillage* GetVillage() { return village; } };
struct CThingObject;
struct CThingT : CThing {
    bool HasTC(long type) const { return type == 0x4a ? bits74 != 0 : hasMember; }
    template<class T> bool GetInterface(T** out, long type) { if (type == 0x23 && hasMember) { *out = (T*)member; return true; } return false; }
};
static int g_ptrSets = 0;
struct CBaseIntelligentPointer {
    CThing* m_pItem;
    CBaseIntelligentPointer() : m_pItem(0) {}
    CThing* GetPItem() const { return m_pItem; }
    void SetPItem(CThing* item) { m_pItem = item; ++g_ptrSets; }
};
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}
struct CIntelligentPointerThingObject : CBaseIntelligentPointer {
    CIntelligentPointerThingObject(CThing* item) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentPointerThingObject& operator=(const CIntelligentPointerThingObject& other)
    {
        CThing* item = other.GetPItem();
        SetPItem(item);
        return *this;
    }
};
struct CAIBrain { CThing* PCreature; };
static CThing* g_near = 0;
struct CAIStateGroup_BanditSitRoundCampfire {
    CAIBrain* PBrain;
    CIntelligentPointerThingObject PCampfire;
    long SeatPosIndex;
    bool GotSeatPos;
    bool Sitting;
    unsigned long FrameToNextCheckIsRunnable;
    CAIStateGroup_BanditSitRoundCampfire() : PCampfire(0) {}
    CThingObject* GetPNearCampfire() const { return (CThingObject*)g_near; }
    bool IsRunnable();
};
bool CAIStateGroup_BanditSitRoundCampfire::IsRunnable()
{
    if (FrameToNextCheckIsRunnable > (unsigned long)GFGetCurrentFrame())
        return false;
    CThingT* creature = (CThingT*)PBrain->PCreature;
    if (!creature->HasTC(0x4a))
        return false;
    CTCVillageMember* member;
    if (creature->GetInterface(&member, 0x23)) {
        if (member->GetVillage() && !member->GetVillage()->IsCurfewTime())
            return false;
    }
    GotSeatPos = false;
    Sitting = false;
    SeatPosIndex = -1;
    PCampfire = CIntelligentPointerThingObject(0);
    PCampfire = CIntelligentPointerThingObject((CThing*)GetPNearCampfire());
    bool alive = IS_THING_ALIVE(PCampfire);
    if (!alive) {
        long framesPerSecond = g_framesPerSecond;
        FrameToNextCheckIsRunnable = GFGetCurrentFrame() + framesPerSecond * 5;
    }
    return alive;
}

static int fail(int n) { std::printf("0093ae80_TEST FAIL (%d)\n", n); return 1; }
int main()
{
    CTCVillage village; village.curfew = true;
    CTCVillageMember member; member.village = &village;
    CThingT bandit; bandit.bits74 = 1; bandit.hasMember = true; bandit.member = &member; bandit.m_flags91 = 0;
    CThing fire; fire.m_flags91 = 0;
    CAIBrain brain; brain.PCreature = &bandit;
    CAIStateGroup_BanditSitRoundCampfire g; g.PBrain = &brain; g.FrameToNextCheckIsRunnable = 0;
    g.GotSeatPos = true; g.Sitting = true; g.SeatPosIndex = 4;
    g_near = &fire;
    if (!g.IsRunnable() || g.PCampfire.GetPItem() != &fire || g.GotSeatPos || g.Sitting || g.SeatPosIndex != -1) return fail(1);
    village.curfew = false;
    if (g.IsRunnable()) return fail(2);                       // village not in curfew: not runnable
    village.curfew = true; g_near = 0;
    if (g.IsRunnable() || g.FrameToNextCheckIsRunnable != 100 + 150) return fail(3);   // no campfire: re-arm 5 s later
    g_near = &fire;
    if (g.IsRunnable()) return fail(4);                       // too early
    g_frame = 250;
    if (!g.IsRunnable()) return fail(5);
    bandit.bits74 = 0; g_frame = 400;
    if (g.IsRunnable()) return fail(6);
    std::printf("0093ae80_TEST PASS\n");
    return 0;
}
