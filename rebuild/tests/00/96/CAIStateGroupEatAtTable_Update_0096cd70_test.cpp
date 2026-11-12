// Behaviour test for CAIStateGroup_EatAtTable::Update (0x0096CD70): a standalone model of the state group's update logic --
// seat claim, village eating duration (+ random up to 45), occupant registration, walk, sit-down action, eat loop, terminate.
#include <cstdio>

enum EAIActionReturn { AI_ACTION_NULL = 0, AI_ACTION_TERMINATE_PROCESS = 1, AI_ACTION_FINISHED = 2, AI_ACTION_FAILED = 3, AI_ACTION_IN_PROGRESS = 4 };

class CVillageDef { public: long EatDuration; };
class CTCVillage { public: CVillageDef* PDef; };
class CTCVillageMember { public: CTCVillage* Village; CTCVillage* GetPVillage() const { return Village; } };
class CThingObject { public: int id; };
class CTCTavernTable { public:
    void* Occupants[4];
    bool SeatUsed[4];
    CThingObject* FoodQueue[8];
    int FoodPos;
    CTCTavernTable() : FoodPos(0) { for (int i = 0; i < 4; ++i) { Occupants[i] = 0; SeatUsed[i] = false; } for (int i = 0; i < 8; ++i) FoodQueue[i] = 0; }
    void SetSeatState(long seat, bool used) { SeatUsed[seat] = used; }
    CThingObject* GetFood(long) { return FoodQueue[FoodPos++]; }
};

struct Model {
    // inputs
    bool seat_ok, walk_ok, has_member;
    CTCVillageMember member;
    CTCTavernTable table;
    EAIActionReturn perform_result;
    int eat_ok_count;       // Eat returns true this many times
    EAIActionReturn idle_results[4];
    int idle_pos;
    float random_value;
    // state
    long SeatIndex;
    float Duration;
    int terminated, eaten, sat_down, idled;
    void* creature;

    bool GetSeat() { return seat_ok; }
    bool WalkTo() { return walk_ok; }
    void Terminate() { ++terminated; }
    EAIActionReturn AIAction_PerformAction() { ++sat_down; return perform_result; }
    bool Eat(CThingObject*) { ++eaten; return eaten <= eat_ok_count; }
    EAIActionReturn AIAction_DoNothing() { ++idled; return idle_results[idle_pos++]; }

    void Update()
    {
        if (!GetSeat()) {
            Terminate();
            return;
        }
        if (has_member && member.GetPVillage()) {
            Duration = (float)member.GetPVillage()->PDef->EatDuration;
            Duration += random_value;
        }
        table.Occupants[SeatIndex] = creature;
        if (!WalkTo()) {
            Terminate();
            return;
        }
        bool failed = AIAction_PerformAction() != AI_ACTION_FINISHED;
        if (!failed) {
            table.SetSeatState(SeatIndex, true);
            for (;;) {
                CThingObject* food = table.GetFood(SeatIndex);
                if (food) {
                    if (!Eat(food))
                        break;
                } else if (AIAction_DoNothing() != AI_ACTION_IN_PROGRESS)
                    break;
            }
        }
        Terminate();
    }
};

static Model Make()
{
    Model m;
    m.seat_ok = true; m.walk_ok = true; m.has_member = false;
    m.member.Village = 0;
    m.perform_result = AI_ACTION_FINISHED;
    m.eat_ok_count = 0;
    for (int i = 0; i < 4; ++i) m.idle_results[i] = AI_ACTION_FINISHED;
    m.idle_pos = 0; m.random_value = 0.0f;
    m.SeatIndex = 1; m.Duration = -1.0f;
    m.terminated = m.eaten = m.sat_down = m.idled = 0;
    m.creature = &m;
    return m;
}

int main()
{
    // no seat: terminate only
    Model a = Make(); a.seat_ok = false; a.Update();
    if (a.terminated != 1 || a.table.Occupants[1] != 0 || a.sat_down != 0) { std::printf("0096cd70_TEST FAIL\n"); return 1; }
    // village duration + random, walk fails
    CVillageDef def = { 30 }; CTCVillage village = { &def };
    Model b = Make(); b.has_member = true; b.member.Village = &village; b.random_value = 12.5f; b.walk_ok = false; b.Update();
    if (b.Duration != 42.5f || b.table.Occupants[1] != b.creature || b.terminated != 1 || b.sat_down != 0) { std::printf("0096cd70_TEST FAIL\n"); return 1; }
    // sit-down fails: no seat state, terminate once
    Model c = Make(); c.perform_result = AI_ACTION_FAILED; c.Update();
    if (c.table.SeatUsed[1] || c.terminated != 1 || c.sat_down != 1) { std::printf("0096cd70_TEST FAIL\n"); return 1; }
    // eat two items (second Eat fails) after one idle frame
    CThingObject f1 = { 1 }, f2 = { 2 };
    Model d = Make(); d.table.FoodQueue[0] = 0; d.table.FoodQueue[1] = &f1; d.table.FoodQueue[2] = &f2;
    d.idle_results[0] = AI_ACTION_IN_PROGRESS; d.eat_ok_count = 1; d.Update();
    if (!d.table.SeatUsed[1] || d.idled != 1 || d.eaten != 2 || d.terminated != 1) { std::printf("0096cd70_TEST FAIL\n"); return 1; }
    // no food and idling finishes: one idle, no eat
    Model e = Make(); e.Update();
    if (e.idled != 1 || e.eaten != 0 || e.terminated != 1 || e.Duration != -1.0f) { std::printf("0096cd70_TEST FAIL\n"); return 1; }
    std::printf("0096cd70_TEST PASS\n");
    return 0;
}