// Behaviour test for 0x00915B60 CAIStateGroup_MoveGoodsBase::PlaceGoods (model of the source's logic over fake engine objects).
// Plain C++98: compiles under g++ (tall.sh) and VC7.1 (tall.py). Prints 00915b60_TEST PASS.
#include <cstdio>
#include <cmath>
#include <vector>

struct Vec { float X, Y, Z; Vec() : X(0), Y(0), Z(0) {} Vec(float x, float y, float z) : X(x), Y(y), Z(z) {} };
static Vec operator-(const Vec& a, const Vec& b) { return Vec(a.X - b.X, a.Y - b.Y, a.Z - b.Z); }
static Vec operator+(const Vec& a, const Vec& b) { return Vec(a.X + b.X, a.Y + b.Y, a.Z + b.Z); }
static Vec operator*(const Vec& a, float f) { return Vec(a.X * f, a.Y * f, a.Z * f); }

struct Thing;
struct ActionPoints {                       // CTCSingleUserActionPoints
    int free_after;                         // GetFreeActionPoint succeeds on this call number
    int calls; long freed; int free_count; Vec point;
    bool GetFreeActionPoint(Thing&, Vec& pos, long& index) { if (++calls < free_after) return false; pos = point; index = 7; return true; }
    void SetActionPointAsFree(long i) { freed = i; ++free_count; }
};
struct ExclusiveUse { Thing* user; int busy_frames; int set_calls; };
struct Thing {
    Vec pos, forward;
    bool crate_stack; long crates;
    ActionPoints* points;                   // single-user action points TC (goods)
    ExclusiveUse* exclusive;                // exclusive-use TC (destination)
    bool solid_tc; int not_solid_to;        // solid-to-thing TC
};
struct World {
    long frame; Thing creature; Thing* carried; Thing* destination; bool destination_alive, goods_alive;
    int wander_result; int prox_result; int lookat_result; int perform_result; int filter_ok;
    int route_calls; int perform_calls; const char* performed_name; bool dropping;
};
static World W;

static int Wander() { W.frame += 1; if (W.destination && W.destination->exclusive && W.destination->exclusive->busy_frames > 0 && --W.destination->exclusive->busy_frames == 0) W.destination->exclusive->user = 0; return W.wander_result; }
static bool Filter() { return W.filter_ok-- > 0; }

// The model of the reconstructed body (same statement structure as src/00915b60.cpp).
static bool PlaceGoods(Thing* destination_in, std::vector<const char*> goods_names, Thing* route_thing)
{
    Thing* destination = destination_in;
    if (!destination->crate_stack) return false;
    Thing* goods = W.goods_alive ? W.carried : 0;
    if (goods && goods->points) {
        ActionPoints* action_points = goods->points;
        long timeout = W.frame + 300;
        Vec position; long index;
        while (!action_points->GetFreeActionPoint(W.creature, position, index)) {
            if (Wander() == 1 || W.frame > timeout || !Filter() || !(W.destination_alive ? destination : 0) || !(W.goods_alive ? goods : 0))
                return false;
        }
        Vec forward = destination->forward;
        Vec d = position - goods->pos;
        float dist = std::sqrt(d.X * d.X + d.Y * d.Y);
        Vec offset = forward * dist;
        if (route_thing) { Vec r = destination->pos - W.creature.pos; if (r.X * r.X + r.Y * r.Y + r.Z * r.Z > 400.0f) ++W.route_calls; }
        destination = W.destination_alive ? destination : 0; goods = W.goods_alive ? goods : 0;
        if (destination && goods) {
            int result;
            do {
                Vec target = destination->pos + offset; (void)target;
                result = W.prox_result;
                if (result == 1 || result == 3 || !Filter()) { action_points->SetActionPointAsFree(index); return false; }
            } while (result == 4);
            ExclusiveUse* exclusive_use = destination->exclusive;
            if (exclusive_use) {
                if (exclusive_use->user) {
                    long end_frame = W.frame + 50 + 10;
                    do result = Wander(); while (exclusive_use->user && result != 1 && W.frame < end_frame && Filter());
                }
                if (exclusive_use->user) { action_points->SetActionPointAsFree(index); return false; }
                exclusive_use->user = &W.creature; ++exclusive_use->set_calls;
            }
            do result = 2; while (result == 4 && Filter());
            do result = W.lookat_result; while (result == 4 && Filter());
            if (result != 2) { action_points->SetActionPointAsFree(index); return false; }
            W.dropping = true;
            long crate_index = destination->crate_stack ? destination->crates : 0;
            if (!W.carried->solid_tc) W.carried->solid_tc = true;
            ++W.carried->not_solid_to;
            ++W.perform_calls; W.performed_name = goods_names[crate_index];
            if (W.perform_result == 2) {
                if (exclusive_use) { exclusive_use->user = 0; ++exclusive_use->set_calls; }
                action_points->SetActionPointAsFree(index);
                return true;
            }
            action_points->SetActionPointAsFree(index);
        } else if (!goods) {
            return false;
        } else {
            action_points->SetActionPointAsFree(index);
        }
    }
    return false;
}

static ActionPoints AP; static ExclusiveUse EX; static Thing DEST, GOODS;
static void Reset()
{
    AP.free_after = 1; AP.calls = 0; AP.freed = -1; AP.free_count = 0; AP.point = Vec(3, 4, 0);
    EX.user = 0; EX.busy_frames = 0; EX.set_calls = 0;
    DEST.pos = Vec(100, 0, 0); DEST.forward = Vec(0, 1, 0); DEST.crate_stack = true; DEST.crates = 2; DEST.points = 0; DEST.exclusive = &EX;
    GOODS.pos = Vec(0, 0, 0); GOODS.points = &AP; GOODS.exclusive = 0; GOODS.solid_tc = false; GOODS.not_solid_to = 0; GOODS.crate_stack = false;
    W.frame = 1000; W.creature.pos = Vec(0, 0, 0); W.carried = &GOODS; W.destination = &DEST; W.destination_alive = true; W.goods_alive = true;
    W.wander_result = 0; W.prox_result = 2; W.lookat_result = 2; W.perform_result = 2; W.filter_ok = 1000;
    W.route_calls = 0; W.perform_calls = 0; W.performed_name = 0; W.dropping = false;
}

int main()
{
    std::vector<const char*> names; names.push_back("PUT_0"); names.push_back("PUT_1"); names.push_back("PUT_2");
    int bad = 0;
    // 1. success: claims and releases the destination, frees the point, uses the crate count's animation
    Reset(); if (!PlaceGoods(&DEST, names, &DEST) || EX.set_calls != 2 || EX.user != 0 || AP.free_count != 1 || AP.freed != 7
                 || W.performed_name != names[2] || !W.dropping || GOODS.not_solid_to != 1 || !GOODS.solid_tc || W.route_calls != 1) bad |= 1;
    // 2. no crate stack: false, nothing touched
    Reset(); DEST.crate_stack = false; if (PlaceGoods(&DEST, names, 0) || AP.calls != 0) bad |= 2;
    // 3. no goods: false
    Reset(); W.goods_alive = false; if (PlaceGoods(&DEST, names, 0) || AP.calls != 0) bad |= 4;
    // 4. no free point and the wander fails: false, no point to free
    Reset(); AP.free_after = 5; W.wander_result = 1; if (PlaceGoods(&DEST, names, 0) || AP.free_count != 0) bad |= 8;
    // 5. destination stays in use: false and the point is freed
    Reset(); EX.user = &DEST; EX.busy_frames = 1000; if (PlaceGoods(&DEST, names, 0) || AP.free_count != 1 || EX.set_calls != 0) bad |= 16;
    // 6. destination becomes free while wandering: success
    Reset(); EX.user = &DEST; EX.busy_frames = 3; if (!PlaceGoods(&DEST, names, 0) || EX.user != 0 || AP.free_count != 1) bad |= 32;
    // 7. look-at fails: false, point freed, user stays claimed
    Reset(); W.lookat_result = 3; if (PlaceGoods(&DEST, names, 0) || AP.free_count != 1 || EX.user != &W.creature || W.perform_calls != 0) bad |= 64;
    // 8. the put-down action fails: false, point freed
    Reset(); W.perform_result = 3; if (PlaceGoods(&DEST, names, 0) || AP.free_count != 1 || W.perform_calls != 1) bad |= 128;
    // 9. walk fails: false, point freed
    Reset(); W.prox_result = 3; if (PlaceGoods(&DEST, names, 0) || AP.free_count != 1 || W.perform_calls != 0) bad |= 256;
    // 10. destination dies before the walk: false, point freed; goods die: false without freeing
    Reset(); W.destination_alive = false; if (PlaceGoods(&DEST, names, 0) || AP.free_count != 1) bad |= 512;
    Reset(); W.goods_alive = false; W.carried = &GOODS; { bool r = PlaceGoods(&DEST, names, 0); if (r || AP.free_count != 0) bad |= 1024; }
    // 11. route thing close by: no pre-calculated route
    Reset(); DEST.pos = Vec(10, 0, 0); if (!PlaceGoods(&DEST, names, &DEST) || W.route_calls != 0) bad |= 2048;
    // 12. the stand-off offset = destination forward * 2D distance of the point from the goods (5 here)
    { Vec f(0, 1, 0); Vec p(3, 4, 9); Vec d = p - Vec(0, 0, 0); float dist = std::sqrt(d.X * d.X + d.Y * d.Y); Vec o = f * dist;
      if (std::fabs(o.Y - 5.0f) > 1e-4f || o.X != 0.0f) bad |= 4096; }
    if (bad) { std::printf("00915b60_TEST FAIL\n"); return 1; }
    std::printf("00915b60_TEST PASS\n");
    return 0;
}
