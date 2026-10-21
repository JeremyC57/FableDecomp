// Behaviour test for CAIStateGroup_MinionSleep::IsStillValid (0x0092C5A0): a copy of the model with stub callees.
#include <cstdio>
#include <cmath>

struct C3DVector { float X, Y, Z; };
struct CThing;
struct CTCVillager { int village; };
struct CMode { long type; long GetModeType() { return type; } };
struct CTCCreatureModeManager { CMode* cur; CMode* GetCurrentMode() { return cur; } };
struct CTCModeManager42 { unsigned long active; bool IsModeActive(unsigned long t) const { return (active >> t) & 1; } };

struct CThing {
    unsigned char flags91;
    C3DVector pos;
    CTCVillager* villager;
    CTCCreatureModeManager* modes;
    CTCModeManager42* m42;
    const C3DVector& GetPosition() const { return pos; }
};
struct CBaseIntelligentPointer { CThing* item; CThing* GetPItem() const { return item; } };
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->flags91 & 1))
            return true;
    }
    return false;
}
struct CAIBrain { CThing* PCreature; };
static int g_villageMessage;
static int g_lastType; static const CThing* g_lastOrigin;

struct CAIStateGroup_MinionSleep {
    CAIBrain* PBrain;
    CBaseIntelligentPointer PBed;
    C3DVector BedPos;
    bool IsStillValid() const;
};

bool CAIStateGroup_MinionSleep::IsStillValid() const
{
    CTCVillager* villager = PBrain->PCreature->villager;
    if (villager->village != 0) {
        CThing* creature = PBrain->PCreature;
        g_lastType = 4; g_lastOrigin = creature;
        if (g_villageMessage)
            return false;
    }
    CTCCreatureModeManager* modes = PBrain->PCreature->modes;
    if (modes->GetCurrentMode()->GetModeType() == 0xd) {
        const C3DVector& pos = PBrain->PCreature->GetPosition();
        float dx = BedPos.X - pos.X;
        float dy = BedPos.Y - pos.Y;
        if ((float)std::sqrt(dy * dy + dx * dx) > 0.1f)
            return false;
    }
    const CThing* creature = PBrain->PCreature;
    CTCModeManager42* manager = creature->m42;
    if (manager->IsModeActive(6) || manager->IsModeActive(1))
        return false;
    if (IS_THING_ALIVE(PBed))
        return true;
    return false;
}

int main()
{
    CTCVillager vil = { 0 };
    CMode mode = { 0xd };
    CTCCreatureModeManager mm = { &mode };
    CTCModeManager42 m42 = { 0 };
    CThing creature; creature.flags91 = 0; creature.pos.X = 1.0f; creature.pos.Y = 2.0f; creature.pos.Z = 9.0f;
    creature.villager = &vil; creature.modes = &mm; creature.m42 = &m42;
    CThing bed; bed.flags91 = 0;
    CAIBrain brain = { &creature };
    CAIStateGroup_MinionSleep s; s.PBrain = &brain; s.PBed.item = &bed; s.BedPos.X = 1.05f; s.BedPos.Y = 2.05f; s.BedPos.Z = 0;
    int ok = 1;
    if (!s.IsStillValid()) ok = 0;                       // near the bed, alive
    s.BedPos.X = 1.2f; if (s.IsStillValid()) ok = 0;     // too far while sleeping
    mode.type = 3; if (!s.IsStillValid()) ok = 0;        // not in sleep mode: distance ignored
    m42.active = 1 << 6; if (s.IsStillValid()) ok = 0;
    m42.active = 1 << 1; if (s.IsStillValid()) ok = 0;
    m42.active = 0; bed.flags91 = 1; if (s.IsStillValid()) ok = 0;
    bed.flags91 = 0; s.PBed.item = 0; if (s.IsStillValid()) ok = 0;
    s.PBed.item = &bed; vil.village = 1; g_villageMessage = 1; if (s.IsStillValid()) ok = 0;
    if (g_lastType != 4 || g_lastOrigin != &creature) ok = 0;
    g_villageMessage = 0; if (!s.IsStillValid()) ok = 0;
    if (!ok) { std::printf("0092c5a0_TEST FAIL\n"); return 1; }
    std::printf("0092c5a0_TEST PASS\n");
    return 0;
}
