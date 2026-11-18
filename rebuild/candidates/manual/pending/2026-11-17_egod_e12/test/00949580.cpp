// Behaviour test for CAIStateGroup_GoToPub::WalktoSeatAndSit (0x00949580): a standalone model of the row's logic -- walk loop guarded
// by pub alive / tavern time (when during_pub_time) / table alive / a TERMINATE_PROCESS result, the FINISHED test after the walk, the
// look loop at forward + creature position, the TERMINATE_PROCESS test after it, the sit-down action on a new tavern stool mode, then
// SetSeatState(SeatIndex, true) and Sitting = true. Every condition has a case that a one-decision mutant fails. Audit e12: the walk's
// constant flag arguments (case 29).
#include <cstdio>
#include <cstring>

enum EAIActionReturn { AI_ACTION_NULL = 0, AI_ACTION_TERMINATE_PROCESS = 1, AI_ACTION_FINISHED = 2, AI_ACTION_FAILED = 3, AI_ACTION_IN_PROGRESS = 4 };
struct C3DVector { float X, Y, Z; };
struct Thing { bool dead; };

struct Model {
    // inputs
    Thing* pub; Thing* table;
    int pub_dies_after;            // number of alive checks of the pub before it reports dead (-1 = never)
    bool tavern_time;
    EAIActionReturn move_res[6]; int move_n;
    EAIActionReturn look_res[6]; int look_n;
    EAIActionReturn perform_res;
    C3DVector pos, forward, creature_pos;
    long seat_index;
    // outputs
    int terminated, moves, looks, performs, seat_calls, pub_checks, tavern_calls;
    long seat_arg; bool seat_used; bool sitting;
    C3DVector move_pos, look_pos; long move_type, move_repeat, look_flags, look_repeat; float look_tol, move_speed;
    char anim_name[32]; long anim_index; bool wait_arg; bool mode_made; bool move_b1, move_b2, move_b3; long move_l;

    bool PubAlive() { ++pub_checks; if (pub_dies_after >= 0 && pub_checks > pub_dies_after) return false; return pub && !pub->dead; }
    int table_dies_after, table_checks;   // alive checks of the table before it reports dead (-1 = never)
    int tavern_ends_after;                // tavern-time calls before it reports false (-1 = never)
    bool TableAlive() { ++table_checks; if (table_dies_after >= 0 && table_checks > table_dies_after) return false; return table && !table->dead; }
    bool IsTavernTime() { ++tavern_calls; if (tavern_ends_after >= 0 && tavern_calls > tavern_ends_after) return false; return tavern_time; }
    void Terminate() { ++terminated; }
    EAIActionReturn MoveToPos(long repeat, C3DVector p, bool& nav, long type, bool b1, bool b2, long l, bool b3, float speed)
    { nav = true; move_repeat = repeat; move_pos = p; move_type = type; move_b1 = b1; move_b2 = b2; move_l = l; move_b3 = b3; move_speed = speed;
      return move_res[moves++]; }
    EAIActionReturn LookAtPos(long repeat, C3DVector p, long flags, float tol)
    { look_repeat = repeat; look_pos = p; look_flags = flags; look_tol = tol; return look_res[looks++]; }
    EAIActionReturn PerformActionAndWait(bool wait) { wait_arg = wait; ++performs; return perform_res; }

    bool WalktoSeatAndSit(bool during_pub_time)
    {
        EAIActionReturn ret = AI_ACTION_FAILED;
        do {
            if (!PubAlive() || (!IsTavernTime() && during_pub_time) || !TableAlive() || ret == AI_ACTION_TERMINATE_PROCESS) {
                Terminate();
                return false;
            }
            bool nav_route_calculated;
            ret = MoveToPos(2, pos, nav_route_calculated, 3, true, true, 0, true, 0.0f);
        } while (ret == AI_ACTION_IN_PROGRESS);
        if (ret != AI_ACTION_FINISHED) {
            Terminate();
            return false;
        }
        C3DVector required_facing_dir = forward;
        C3DVector pos_to_look_at;
        pos_to_look_at.X = required_facing_dir.X + creature_pos.X;
        pos_to_look_at.Y = required_facing_dir.Y + creature_pos.Y;
        pos_to_look_at.Z = required_facing_dir.Z + creature_pos.Z;
        do {
            if (!PubAlive() || (!IsTavernTime() && during_pub_time) || !TableAlive()) {
                Terminate();
                return false;
            }
            ret = LookAtPos(2, pos_to_look_at, 0x11, 0.01f);
        } while (ret == AI_ACTION_IN_PROGRESS);
        if (ret == AI_ACTION_TERMINATE_PROCESS) {
            Terminate();
            return false;
        }
        std::strcpy(anim_name, "SIT_AT_TABLE_INTO"); anim_index = 0;
        mode_made = true;
        if (PerformActionAndWait(false) != AI_ACTION_FINISHED) {
            Terminate();
            return false;
        }
        ++seat_calls; seat_arg = seat_index; seat_used = true;
        sitting = true;
        return true;
    }
};

static Thing g_pub, g_table;
static Model Make()
{
    Model m; std::memset(&m, 0, sizeof m);
    g_pub.dead = false; g_table.dead = false;
    m.pub = &g_pub; m.table = &g_table; m.pub_dies_after = -1; m.table_dies_after = -1; m.tavern_ends_after = -1; m.tavern_time = true;
    m.move_res[0] = AI_ACTION_FINISHED; m.look_res[0] = AI_ACTION_FINISHED; m.perform_res = AI_ACTION_FINISHED;
    m.pos.X = 1.5f; m.pos.Y = 2.5f; m.pos.Z = 3.5f;
    m.forward.X = 0.25f; m.forward.Y = -0.5f; m.forward.Z = 0.75f;
    m.creature_pos.X = 10.0f; m.creature_pos.Y = 20.0f; m.creature_pos.Z = 30.0f;
    m.seat_index = 3; m.move_l = -1; m.move_speed = -1.0f;
    return m;
}
static int fails = 0;
static void Check(bool ok, int id) { if (!ok) { std::printf("case %d failed\n", id); ++fails; } }

int main()
{
    { Model m = Make(); bool r = m.WalktoSeatAndSit(true);          // happy path, every argument
      Check(r && m.terminated == 0 && m.moves == 1 && m.looks == 1 && m.performs == 1 && m.seat_calls == 1 && m.seat_arg == 3 &&
            m.seat_used && m.sitting && m.mode_made, 1);
      Check(m.move_repeat == 2 && m.move_type == 3 && m.move_speed == 0.0f && m.move_pos.X == 1.5f && m.move_pos.Y == 2.5f && m.move_pos.Z == 3.5f, 2);
      Check(m.look_repeat == 2 && m.look_flags == 0x11 && m.look_tol == 0.01f && m.look_pos.X == 10.25f && m.look_pos.Y == 19.5f &&
            m.look_pos.Z == 30.75f, 3);
      Check(std::strcmp(m.anim_name, "SIT_AT_TABLE_INTO") == 0 && m.anim_index == 0 && m.wait_arg == false, 4);
      Check(m.move_b1 && m.move_b2 && m.move_l == 0 && m.move_b3, 29); }
    { Model m = Make(); m.pub = 0; Check(!m.WalktoSeatAndSit(true) && m.terminated == 1 && m.moves == 0, 5); }           // pub null
    { Model m = Make(); g_pub.dead = true; Check(!m.WalktoSeatAndSit(true) && m.terminated == 1 && m.moves == 0, 6); }   // pub dead
    { Model m = Make(); m.tavern_time = false; Check(!m.WalktoSeatAndSit(true) && m.terminated == 1 && m.moves == 0, 7); } // not tavern time, during
    { Model m = Make(); m.tavern_time = false; Check(m.WalktoSeatAndSit(false) && m.terminated == 0 && m.sitting, 8); }    // not tavern time, not during
    { Model m = Make(); m.table = 0; Check(!m.WalktoSeatAndSit(false) && m.terminated == 1 && m.moves == 0, 9); }         // table null
    { Model m = Make(); g_table.dead = true; Check(!m.WalktoSeatAndSit(false) && m.terminated == 1 && m.moves == 0, 10); } // table dead
    { Model m = Make(); m.move_res[0] = AI_ACTION_IN_PROGRESS; m.move_res[1] = AI_ACTION_IN_PROGRESS; m.move_res[2] = AI_ACTION_FINISHED;
      Check(m.WalktoSeatAndSit(true) && m.moves == 3 && m.terminated == 0, 11); }                                          // walk loop runs
    { Model m = Make(); m.move_res[0] = AI_ACTION_FAILED; Check(!m.WalktoSeatAndSit(true) && m.moves == 1 && m.terminated == 1 && m.looks == 0, 12); }
    { Model m = Make(); m.move_res[0] = AI_ACTION_TERMINATE_PROCESS; Check(!m.WalktoSeatAndSit(true) && m.moves == 1 && m.terminated == 1 && m.looks == 0, 13); }
    { Model m = Make(); m.move_res[0] = AI_ACTION_NULL; Check(!m.WalktoSeatAndSit(true) && m.moves == 1 && m.terminated == 1, 14); }
    { Model m = Make(); m.move_res[0] = AI_ACTION_IN_PROGRESS; m.move_res[1] = AI_ACTION_FINISHED; m.pub_dies_after = 1;    // pub dies mid-walk
      Check(!m.WalktoSeatAndSit(true) && m.moves == 1 && m.terminated == 1, 15); }
    { Model m = Make(); m.pub_dies_after = 1; Check(!m.WalktoSeatAndSit(true) && m.moves == 1 && m.looks == 0 && m.terminated == 1, 16); } // dies before look
    { Model m = Make(); m.look_res[0] = AI_ACTION_IN_PROGRESS; m.look_res[1] = AI_ACTION_IN_PROGRESS; m.look_res[2] = AI_ACTION_FINISHED;
      Check(m.WalktoSeatAndSit(true) && m.looks == 3 && m.sitting, 17); }
    { Model m = Make(); m.look_res[0] = AI_ACTION_TERMINATE_PROCESS; Check(!m.WalktoSeatAndSit(true) && m.looks == 1 && m.performs == 0 && m.terminated == 1, 18); }
    { Model m = Make(); m.look_res[0] = AI_ACTION_FAILED; Check(m.WalktoSeatAndSit(true) && m.performs == 1 && m.sitting, 19); }   // look failure still sits
    { Model m = Make(); m.look_res[0] = AI_ACTION_NULL; Check(m.WalktoSeatAndSit(true) && m.performs == 1, 20); }
    { Model m = Make(); m.tavern_time = false; m.move_res[0] = AI_ACTION_FINISHED; Check(m.WalktoSeatAndSit(false) && m.tavern_calls == 2, 21); }
    { Model m = Make(); m.table_dies_after = 1; Check(!m.WalktoSeatAndSit(true) && m.moves == 1 && m.looks == 0 && m.terminated == 1, 23); } // table dies before look
    { Model m = Make(); m.tavern_ends_after = 1; Check(!m.WalktoSeatAndSit(true) && m.moves == 1 && m.looks == 0 && m.terminated == 1, 24); } // tavern time ends before look
    { Model m = Make(); m.tavern_ends_after = 1; Check(m.WalktoSeatAndSit(false) && m.looks == 1 && m.sitting, 25); }                       // ...ignored when not during
    { Model m = Make(); m.move_res[0] = AI_ACTION_IN_PROGRESS; m.move_res[1] = AI_ACTION_FINISHED; m.table_dies_after = 1;                // table dies mid-walk
      Check(!m.WalktoSeatAndSit(true) && m.moves == 1 && m.terminated == 1, 26); }
    { Model m = Make(); m.look_res[0] = AI_ACTION_IN_PROGRESS; m.look_res[1] = AI_ACTION_FINISHED; m.table_dies_after = 2;                // table dies mid-look
      Check(!m.WalktoSeatAndSit(true) && m.looks == 1 && m.performs == 0 && m.terminated == 1, 27); }
    { Model m = Make(); m.perform_res = AI_ACTION_IN_PROGRESS; Check(!m.WalktoSeatAndSit(true) && m.terminated == 1 && m.seat_calls == 0, 28); }
    { Model m = Make(); m.perform_res = AI_ACTION_FAILED; Check(!m.WalktoSeatAndSit(true) && m.terminated == 1 && m.seat_calls == 0 && !m.sitting, 22); }
    { Model m = Make(); m.perform_res = AI_ACTION_IN_PROGRESS; Check(!m.WalktoSeatAndSit(true) && m.terminated == 1 && !m.sitting, 23); }
    { Model m = Make(); m.perform_res = AI_ACTION_TERMINATE_PROCESS; Check(!m.WalktoSeatAndSit(true) && m.terminated == 1 && !m.sitting, 24); }
    if (fails) { std::printf("00949580_TEST FAIL\n"); return 1; }
    std::printf("00949580_TEST PASS\n");
    return 0;
}
