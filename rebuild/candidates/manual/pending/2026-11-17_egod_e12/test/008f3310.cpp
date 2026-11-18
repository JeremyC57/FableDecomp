// Behaviour test for CCreatureAction_DragonFireBlastBase::PerformAction (0x008F3310): a standalone model of the row's logic -- the
// weapon def's shot def, the creature position overwritten by the DRAGON_MOUTH_DUMMY lookup, the shot object created at the mouth with
// the creature's player number, and (only when created) PShot, the AddTC of the shot def's class name, and Init toward
// mouth + normalise(-(Up.X, Up.Y), Z = 0) * |target - mouth| with Z replaced by the target's Z. Every condition / value has a case.
#include <cstdio>
#include <cstring>
#include <cmath>

struct V { float X, Y, Z; };
static float InvSqrt(float s) { return 1.0f / std::sqrt(s); }   // retail: GFFastOneOverSqrt (table estimate + one Newton step)

struct Model {
    // inputs
    long weapon_shot;               // CWeaponDef::Shot
    long shot_object_def;           // CThingShotDef::ObjectDef
    long shot_class_name;           // CThingShotDef::ShotDefinitionClassName index
    V creature_pos, dummy_pos, dummy_up, target_pos_in;
    bool dummy_found; bool create_ok; long player; long accuracy;
    // outputs
    long getdef_index; long create_def; V create_pos; long create_player; char create_name[8];
    char dummy_name[32]; long dummy_index; unsigned long dummy_flags; float dummy_scale;
    bool pshot_set; int addtc_calls; long addtc_name; bool addtc_flag; int init_calls;
    V init_target; float init_acc, init_a, init_b; int init_weapon;

    void PerformAction()
    {
        getdef_index = weapon_shot;
        V mouth_pos = creature_pos;
        std::strcpy(dummy_name, "DRAGON_MOUTH_DUMMY"); dummy_index = 0; dummy_flags = 0; dummy_scale = 1.0f;
        V rhs_up = { 0.0f, 0.0f, 0.0f };
        if (dummy_found) { mouth_pos = dummy_pos; rhs_up = dummy_up; }
        create_def = shot_object_def; create_pos = mouth_pos; create_player = player; std::strcpy(create_name, "");
        if (create_ok) {
            pshot_set = true;
            long acc = accuracy;
            ++addtc_calls; addtc_name = shot_class_name; addtc_flag = true;
            V fire_direction = rhs_up;
            fire_direction.X *= -1.0f; fire_direction.Y *= -1.0f; fire_direction.Z *= -1.0f;
            fire_direction.Z = 0.0f;
            V norm = fire_direction;
            float f = InvSqrt(norm.X * norm.X + norm.Y * norm.Y + norm.Z * norm.Z);
            norm.X *= f; norm.Y *= f; norm.Z *= f;
            V line = { target_pos_in.X - mouth_pos.X, target_pos_in.Y - mouth_pos.Y, target_pos_in.Z - mouth_pos.Z };
            float len = std::sqrt(line.X * line.X + line.Y * line.Y + line.Z * line.Z);
            norm.X *= len; norm.Y *= len; norm.Z *= len;
            V target_pos = { mouth_pos.X + norm.X, mouth_pos.Y + norm.Y, mouth_pos.Z + norm.Z };
            target_pos.Z = target_pos_in.Z;
            ++init_calls; init_target = target_pos; init_acc = (float)acc; init_a = 1.0f; init_b = 0.0f; init_weapon = 0;
        }
    }
};

static Model Make()
{
    Model m; std::memset(&m, 0, sizeof m);
    m.weapon_shot = 17; m.shot_object_def = 230; m.shot_class_name = 41;
    m.creature_pos.X = 100.0f; m.creature_pos.Y = 200.0f; m.creature_pos.Z = 5.0f;
    m.dummy_pos.X = 10.0f; m.dummy_pos.Y = 20.0f; m.dummy_pos.Z = 8.0f;
    m.dummy_up.X = 0.6f; m.dummy_up.Y = -0.8f; m.dummy_up.Z = 0.9f;   // Z must be ignored
    m.target_pos_in.X = 13.0f; m.target_pos_in.Y = 24.0f; m.target_pos_in.Z = 20.0f; // |line| = sqrt(9+16+144) = 13
    m.dummy_found = true; m.create_ok = true; m.player = 3; m.accuracy = 75;
    return m;
}
static bool Near(float a, float b) { return std::fabs(a - b) < 1e-3f; }
static int fails = 0;
static void Check(bool ok, int id) { if (!ok) { std::printf("case %d failed\n", id); ++fails; } }

int main()
{
    { Model m = Make(); m.PerformAction();
      Check(m.getdef_index == 17 && m.create_def == 230 && m.create_player == 3 && m.create_name[0] == 0, 1);
      Check(std::strcmp(m.dummy_name, "DRAGON_MOUTH_DUMMY") == 0 && m.dummy_index == 0 && m.dummy_flags == 0 && m.dummy_scale == 1.0f, 2);
      Check(m.create_pos.X == 10.0f && m.create_pos.Y == 20.0f && m.create_pos.Z == 8.0f, 3);          // created at the mouth dummy
      Check(m.pshot_set && m.addtc_calls == 1 && m.addtc_name == 41 && m.addtc_flag && m.init_calls == 1, 4);
      // direction = normalise(-0.6, 0.8, 0) = (-0.6, 0.8, 0); scaled by 13 -> (-7.8, 10.4, 0)
      Check(Near(m.init_target.X, 10.0f - 7.8f) && Near(m.init_target.Y, 20.0f + 10.4f), 5);
      Check(m.init_target.Z == 20.0f, 6);                                                            // target's own Z
      Check(m.init_acc == 75.0f && m.init_a == 1.0f && m.init_b == 0.0f && m.init_weapon == 0, 7); }
    { Model m = Make(); m.create_ok = false; m.PerformAction();
      Check(!m.pshot_set && m.addtc_calls == 0 && m.init_calls == 0 && m.create_def == 230, 8); }
    { Model m = Make(); m.dummy_found = false; m.dummy_up.X = 0.0f; m.create_ok = false; m.PerformAction();
      Check(m.create_pos.X == 100.0f && m.create_pos.Y == 200.0f && m.create_pos.Z == 5.0f, 9); }   // creature position kept
    { Model m = Make(); m.dummy_up.X = 3.0f; m.dummy_up.Y = 4.0f; m.dummy_up.Z = -7.0f;          // unnormalised heading, Z ignored
      m.target_pos_in.X = 10.0f; m.target_pos_in.Y = 20.0f; m.target_pos_in.Z = 18.0f;            // line = (0,0,10): 3D length 10
      m.PerformAction();
      Check(Near(m.init_target.X, 10.0f - 6.0f) && Near(m.init_target.Y, 20.0f - 8.0f) && m.init_target.Z == 18.0f, 10); }
    if (fails) { std::printf("008f3310_TEST FAIL\n"); return 1; }
    std::printf("008f3310_TEST PASS\n");
    return 0;
}
