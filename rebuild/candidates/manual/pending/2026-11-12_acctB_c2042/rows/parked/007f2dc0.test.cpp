// Behaviour test for 0x007F2DC0 CTCLightningNode::FrameUpdate: a host model of the node update with the engine callees mocked:
// no def -> nothing (not even the timer); lifetime out (Timer > LifeTime * fps with LifeTime > 0), a dead parent or a parent without a
// lightning node -> RemoveTC(TCI_LIGHTNING_NODE); otherwise (NodeDefIndex > 0 and MaxNumberOfBranches > 0) every thing found in range
// gets a lightning node (parent, faction source, hit source, node def) and a new branch; then, when the def's special effects name
// LIGHTNING_ORB_BRANCH, each branch is erased when its target is dead or out of range, else its particle beam is (re)created and fed
// LIGHTNING_START / LIGHTNING_END, and every DamagePeriod * fps frames a game-object target is hit (DAMAGE_LIGHTNING, DamagePerCycle,
// no hit response) and electrocuted. The timer counts every update that had a def. Kept in step with the row source.
#include <cstdio>
#include <cmath>

struct V3 { float X, Y, Z; };
static V3 Make(float x, float y, float z) { V3 v; v.X = x; v.Y = y; v.Z = z; return v; }

struct Thing {
    bool alive; bool has_node; bool game_object; V3 focal; int hits; float last_damage; int last_attribute; bool last_response;
    bool electrocuted_tc; int electrocuted_resets; int nodes_added;
};
struct Branch { Thing* target; int effect; long timer; };
struct Def { float Range, LifeTime; long MaxNumberOfBranches; float DamagePeriod, DamagePerCycle; long NodeDefIndex; };

struct Node {
    Def* def; long Timer; Thing* parent; Thing* owner; Thing* hit_source; bool removed;
    Branch branches[16]; int count; bool has_effect_def; long effect_index; int effects_created; int last_start_end;
    Thing* found[8]; int found_count;

    void Erase(int i) { for (int j = i; j + 1 < count; ++j) branches[j] = branches[j + 1]; --count; }
    void FrameUpdate(long fps)
    {
        if (def == 0) return;
        long timer = Timer;
        if ((def->LifeTime > 0.0f && timer > def->LifeTime * fps) || parent == 0 || !parent->alive || !parent->has_node) {
            owner->has_node = false; removed = true;                                   // ParentThing->RemoveTC(TCI_LIGHTNING_NODE)
        } else {
            if (def->NodeDefIndex > 0 && def->MaxNumberOfBranches > 0) {
                int it = 0;
                if (it != found_count) {
                    do {
                        Thing* thing = found[it];
                        thing->has_node = true; ++thing->nodes_added;                  // AddTC + SetPParent / sources / SetPDef
                        Branch b; b.target = thing; b.effect = 0; b.timer = 0;
                        branches[count++] = b;                                         // LightningBranches.push_back(new CLightningBranch)
                        ++it;
                    } while (it != found_count);
                }
            }
            long effect = -1;
            if (has_effect_def && (effect = effect_index, true)) {
                V3 start = owner->focal;
                for (int it = 0; it != count; ) {
                    Thing* target = branches[it].target;
                    if (target == 0 || !target->alive) { Erase(it); continue; }
                    V3 end = target->focal;
                    V3 d = Make(start.X - end.X, start.Y - end.Y, start.Z - end.Z);
                    if (d.X * d.X + d.Z * d.Z + d.Y * d.Y > def->Range * def->Range) { Erase(it); continue; }
                    if (branches[it].effect == 0) { branches[it].effect = (int)effect + 100; ++effects_created; }
                    if (branches[it].effect != 0) {
                        last_start_end = 2;                                            // LIGHTNING_START + LIGHTNING_END inserted
                        if (def->DamagePerCycle > 1e-4f && target->game_object) {
                            if (branches[it].timer++ % (long)(fps * def->DamagePeriod) == 0) {
                                ++target->hits; target->last_damage = def->DamagePerCycle; target->last_attribute = 2;
                                target->last_response = false;
                                target->electrocuted_tc = true; ++target->electrocuted_resets;
                            }
                        }
                    }
                    ++it;
                }
            }
        }
        ++Timer;
    }
};

static Thing MakeThing(float x) { Thing t = {}; t.alive = true; t.has_node = false; t.game_object = true; t.focal = Make(x, 0.0f, 0.0f); return t; }

int main()
{
    int fail = 0;
    Def def = { 10.0f, 2.0f, 4, 1.0f, 5.0f, 7 };
    Thing owner = MakeThing(0.0f); owner.has_node = true;
    Thing parent = MakeThing(0.0f); parent.has_node = true;
    Thing a = MakeThing(3.0f), b = MakeThing(20.0f);
    Node n = {}; n.def = 0; n.Timer = 5; n.owner = &owner; n.parent = &parent;
    n.FrameUpdate(15);
    if (n.Timer != 5 || n.removed) fail |= 1;                                          // no def: untouched
    n.def = &def; n.found[0] = &a; n.found[1] = &b; n.found_count = 2; n.has_effect_def = true; n.effect_index = 3;
    n.Timer = 0;
    n.FrameUpdate(15);
    if (n.Timer != 1 || n.removed) fail |= 2;
    if (!a.has_node || !b.has_node) fail |= 4;
    if (n.count != 1 || n.branches[0].target != &a) fail |= 8;                         // b out of range (20 > 10): erased
    if (a.hits != 1 || a.last_attribute != 2 || std::fabs(a.last_damage - 5.0f) > 1e-6f || n.effects_created != 1) fail |= 16;
    n.found_count = 0;
    for (int i = 0; i < 14; ++i) n.FrameUpdate(15);                                     // timers 1..14: period 15, no new hit
    if (a.hits != 1) fail |= 32;
    n.FrameUpdate(15);                                                                 // branch timer 15: second hit
    if (a.hits != 2 || a.electrocuted_resets != 2) fail |= 64;
    a.alive = false; n.FrameUpdate(15);
    if (n.count != 0) fail |= 128;                                                     // dead target erased
    n.Timer = 31; n.FrameUpdate(15);                                                   // 31 > 2 * 15: expire
    if (!n.removed || owner.has_node || n.Timer != 32) fail |= 256;
    Node m = {}; m.def = &def; m.owner = &owner; m.parent = &parent; parent.alive = false; owner.has_node = true;
    m.FrameUpdate(15);
    if (!m.removed || m.Timer != 1) fail |= 512;                                       // dead parent: removed, timer counted
    if (fail) { std::printf("007f2dc0_TEST FAIL\n"); return 1; }
    std::printf("007f2dc0_TEST PASS\n");
    return 0;
}
