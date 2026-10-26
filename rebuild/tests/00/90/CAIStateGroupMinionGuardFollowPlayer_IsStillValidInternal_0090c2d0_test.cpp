// Behaviour test for CAIStateGroup_MinionGuardFollowPlayer::IsStillValidInternal (0x0090C2D0): model copy, scripted stubs.
#include <cstdio>

static int g_released;
struct CBuildingSimDefinition { int Count; void* Owner; };
struct CDefPointer {
    CBuildingSimDefinition* Object;
    CDefPointer(CBuildingSimDefinition* p) : Object(p) { if (p) ++p->Count; }
    CDefPointer(const CDefPointer& o) : Object(o.Object) { if (Object) ++Object->Count; }
    ~CDefPointer() { if (Object) { if (--Object->Count == 0) ++g_released; Object = 0; } }
    CBuildingSimDefinition* operator->() const { return Object; }
};
struct CTCShopSign { bool m_closed; };
struct CTCBuildingGuarded { bool Guarded; };
class CThing { public:
    unsigned char ThingType;
    unsigned char m_flags91;
    bool has53;
    CTCShopSign* sign;
    CTCBuildingGuarded* guarded;
    CBuildingSimDefinition* sim;
    bool HasTC53() const { return has53; }
    bool GetInterface(CTCShopSign** out) { if (!sign) return false; *out = sign; return true; }
    bool GetInterface(CTCBuildingGuarded** out) { if (!guarded) return false; *out = guarded; return true; }
    CDefPointer PeekPSimDefinition() const { return CDefPointer(sim); }
};
static bool IsAlive(const CThing* thing)
{
    if (thing && !(thing->m_flags91 & 1))
        return true;
    return false;
}
struct CVillage { int NextFollowFrame; };
static CThing* g_player;
static CThing* g_inside;          // the player's 0x75 building pointer (0: no component)
static bool g_hasInsideTC;
static CVillage* g_village;
static bool g_member;
static int g_frame;
struct CAIStateGroup_MinionGuardFollowPlayer {
    bool IsStillValidInternal(bool);
};

bool CAIStateGroup_MinionGuardFollowPlayer::IsStillValidInternal(bool)
{
    CThing* player = g_player;
    if (player && !(player->m_flags91 & 1)) {
        if (g_hasInsideTC && IsAlive(g_inside)) {
            CThing* building = g_inside;
            if (building->ThingType == 3 && g_member && g_village) {
                CVillage* village = g_village;
                CTCShopSign* sign;
                if (building->GetInterface(&sign) && sign->m_closed) {
                    if (!building->HasTC53())
                        return false;
                } else {
                    if (building->HasTC53())
                        return false;
                    bool unowned = !building->PeekPSimDefinition().Object || !building->PeekPSimDefinition()->Owner;
                    if (unowned)
                        return false;
                }
                int nextFrame = village->NextFollowFrame;
                if (g_frame < nextFrame)
                    return false;
                CTCBuildingGuarded* guarded;
                if (building->GetInterface(&guarded) && guarded->Guarded && !building->HasTC53())
                    return false;
                return true;
            }
        }
    }
    return false;
}

int main()
{
    int ok = 1;
    int owner = 1;
    CBuildingSimDefinition sim = { 1, &owner };
    CTCShopSign sign = { false };
    CTCBuildingGuarded guarded = { false };
    CThing hero = { 1, 0, false, 0, 0, 0 };
    CThing house = { 3, 0, false, &sign, &guarded, &sim };
    CVillage village = { 10 };
    CAIStateGroup_MinionGuardFollowPlayer s;
    g_player = &hero; g_inside = &house; g_hasInsideTC = true; g_village = &village; g_member = true; g_frame = 20;
    // an owned open building, frame passed: valid; the temporaries leave the count as it was
    if (!s.IsStillValidInternal(false) || sim.Count != 1 || g_released) ok = 0;
    // frame not yet reached
    g_frame = 5;
    if (s.IsStillValidInternal(false)) ok = 0;
    g_frame = 20;
    // unowned / no sim definition
    sim.Owner = 0;
    if (s.IsStillValidInternal(false)) ok = 0;
    house.sim = 0;
    if (s.IsStillValidInternal(false)) ok = 0;
    house.sim = &sim; sim.Owner = &owner;
    // an open building carrying 0x53 is invalid; a closed shop needs it
    house.has53 = true;
    if (s.IsStillValidInternal(false)) ok = 0;
    sign.m_closed = true;
    if (!s.IsStillValidInternal(false)) ok = 0;
    house.has53 = false;
    if (s.IsStillValidInternal(false)) ok = 0;
    sign.m_closed = false;
    // a guarded building without 0x53 is invalid
    guarded.Guarded = true;
    if (s.IsStillValidInternal(false)) ok = 0;
    guarded.Guarded = false;
    // not a building / dead building / no village / dead player
    house.ThingType = 2;
    if (s.IsStillValidInternal(false)) ok = 0;
    house.ThingType = 3; house.m_flags91 = 1;
    if (s.IsStillValidInternal(false)) ok = 0;
    house.m_flags91 = 0; g_village = 0;
    if (s.IsStillValidInternal(false)) ok = 0;
    g_village = &village; hero.m_flags91 = 1;
    if (s.IsStillValidInternal(false)) ok = 0;
    hero.m_flags91 = 0;
    if (!s.IsStillValidInternal(true) || sim.Count != 1) ok = 0;
    if (!ok) { std::printf("0090c2d0_TEST FAIL\n"); return 1; }
    std::printf("0090c2d0_TEST PASS\n");
    return 0;
}