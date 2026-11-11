// Behaviour test for 0x00951430 CAIStateGroup_JackDragon::IntoGroundSwoop: a self-contained model of the row (same routes, same
// order of AI calls, same constants), driven through both sides, both random outcomes and both terminate exits, and checked against
// the expected routes, flame points and call trace. Plain C++98; no row object linked.
#include <cstdio>
#include <cstring>
#include <cmath>

struct V3 {
    float X, Y, Z;
    V3() : X(0), Y(0), Z(0) { }
    V3(float x, float y, float z) : X(x), Y(y), Z(z) { }
    V3 operator+(const V3& o) const { return V3(X + o.X, Y + o.Y, Z + o.Z); }
    V3 operator*(float s) const { return V3(X * s, Y * s, Z * s); }
    void RotateXY(float a) { float c = (float)std::cos(a), s = (float)std::sin(a); float x = X * c - Y * s; Y = X * s + Y * c; X = x; }
};
static bool Near(const V3& a, float x, float y, float z) { return std::fabs(a.X - x) < 1e-3f && std::fabs(a.Y - y) < 1e-3f && std::fabs(a.Z - z) < 1e-3f; }

struct Route { V3 p[8]; int n; Route() : n(0) { } void push_back(const V3& v) { p[n++] = v; } void clear() { n = 0; } };

struct Dragon {
    // world model
    bool marker_alive, marker_has_physics, enemy_alive, stopping_after_first, stopping;
    float marker_heading; V3 marker_pos, marker_facing, creature_pos;
    unsigned long seed;
    // members
    long GoLeft, CurrentState; V3 PreviousFlightPos;
    V3 OutOfGround1, OutOfGround2, OutOfGround3Left, OutOfGround3Right, OutOfGround4Left, OutOfGround4Right;
    V3 IntoGround1Left, IntoGround1Right, IntoGround2Left, IntoGround2Right, IntoGround3;
    // observations
    Route flights[2]; V3 flight_prev[2]; int nflights; Route burn_route; V3 burn_prev, burn_start, burn_end; float burn_a, burn_b;
    char trace[256];
    void Log(const char* s) { std::strcat(trace, s); std::strcat(trace, ","); }
    void StartFlying(Route& r, V3& prev) { flights[nflights] = r; flight_prev[nflights] = prev; ++nflights; Log("fly"); if (stopping_after_first) stopping = true; }
    bool IsStillRunnable() { return enemy_alive && !stopping; }
    void Terminate() { Log("terminate"); }
    void Burninate(Route& r, V3& prev, float a, float b, V3& s, V3& e) { burn_route = r; burn_prev = prev; burn_a = a; burn_b = b; burn_start = s; burn_end = e; Log("burn"); }
    unsigned long Random2() { seed = seed * 0x24a1 + 0x24df; seed = (seed >> 13) | (seed << 19); return seed % 2; }
    long ChooseHoverOrStrafeState() { Log("choose"); return 7; }

    void IntoGroundSwoop()
    {
        V3 swoop_offset(0.0f, -37.93019f, 15.0f);
        V3 far_offset(0.0f, -80.0f, 30.0f);
        if (marker_alive && marker_has_physics) {
            swoop_offset.RotateXY(marker_heading);
            far_offset.RotateXY(marker_heading);
        }
        Route route;
        route.push_back(creature_pos);
        if (GoLeft) { route.push_back(IntoGround1Left); route.push_back(IntoGround2Left); }
        else { route.push_back(IntoGround1Right); route.push_back(IntoGround2Right); }
        route.push_back(IntoGround3);
        route.push_back(swoop_offset + marker_pos);
        StartFlying(route, PreviousFlightPos);
        if (!IsStillRunnable()) { Terminate(); return; }
        V3 flame_start = marker_pos;
        V3 flame_end = marker_facing * 20.0f + marker_pos;
        V3 out_pos = marker_facing * 10.0f + marker_pos + V3(0.0f, 0.0f, 8.0f);
        PreviousFlightPos = IntoGround3;
        route.clear();
        route.push_back(creature_pos);
        route.push_back(out_pos);
        route.push_back(OutOfGround1);
        Burninate(route, PreviousFlightPos, 0.1f, 0.6f, flame_start, flame_end);
        route.clear();
        route.push_back(creature_pos);
        GoLeft = (long)Random2();
        if (GoLeft) {
            route.push_back(OutOfGround2); route.push_back(OutOfGround3Left); route.push_back(OutOfGround4Left);
            PreviousFlightPos = OutOfGround3Left;
        } else {
            route.push_back(OutOfGround2); route.push_back(OutOfGround3Right); route.push_back(OutOfGround4Right);
            PreviousFlightPos = OutOfGround3Right;
        }
        StartFlying(route, PreviousFlightPos);
        if (!IsStillRunnable()) { Terminate(); return; }
        CurrentState = ChooseHoverOrStrafeState();
    }
};

static void Init(Dragon& d)
{
    std::memset(d.trace, 0, sizeof(d.trace));
    d.marker_alive = true; d.marker_has_physics = true; d.enemy_alive = true; d.stopping_after_first = false; d.stopping = false;
    d.marker_heading = 0.0f; d.marker_pos = V3(100, 200, 10); d.marker_facing = V3(1, 0, 0); d.creature_pos = V3(5, 6, 7);
    d.seed = 1; d.GoLeft = 1; d.CurrentState = 0; d.PreviousFlightPos = V3(-1, -1, -1); d.nflights = 0;
    d.OutOfGround1 = V3(1, 0, 0); d.OutOfGround2 = V3(2, 0, 0); d.OutOfGround3Left = V3(3, 1, 0); d.OutOfGround3Right = V3(3, 2, 0);
    d.OutOfGround4Left = V3(4, 1, 0); d.OutOfGround4Right = V3(4, 2, 0);
    d.IntoGround1Left = V3(11, 1, 0); d.IntoGround1Right = V3(11, 2, 0); d.IntoGround2Left = V3(12, 1, 0); d.IntoGround2Right = V3(12, 2, 0);
    d.IntoGround3 = V3(13, 0, 0);
}

int main()
{
    bool ok = true;
    // 1. left side, full run
    Dragon d; Init(d);
    d.IntoGroundSwoop();
    ok = ok && std::strcmp(d.trace, "fly,burn,fly,choose,") == 0 && d.CurrentState == 7 && d.nflights == 2;
    ok = ok && d.flights[0].n == 5 && Near(d.flights[0].p[0], 5, 6, 7) && Near(d.flights[0].p[1], 11, 1, 0) && Near(d.flights[0].p[2], 12, 1, 0)
            && Near(d.flights[0].p[3], 13, 0, 0) && Near(d.flights[0].p[4], 100, 200 - 37.93019f, 25);
    ok = ok && Near(d.flight_prev[0], -1, -1, -1);
    ok = ok && d.burn_route.n == 3 && Near(d.burn_route.p[1], 110, 200, 18) && Near(d.burn_route.p[2], 1, 0, 0) && Near(d.burn_prev, 13, 0, 0);
    ok = ok && Near(d.burn_start, 100, 200, 10) && Near(d.burn_end, 120, 200, 10) && d.burn_a == 0.1f && d.burn_b == 0.6f;
    unsigned long s = 1; s = s * 0x24a1 + 0x24df; s = (s >> 13) | (s << 19);
    long expect_left = (long)(s % 2);
    ok = ok && d.GoLeft == expect_left && d.flights[1].n == 4;
    if (expect_left) ok = ok && Near(d.flights[1].p[2], 3, 1, 0) && Near(d.flights[1].p[3], 4, 1, 0) && Near(d.PreviousFlightPos, 3, 1, 0);
    else ok = ok && Near(d.flights[1].p[2], 3, 2, 0) && Near(d.flights[1].p[3], 4, 2, 0) && Near(d.PreviousFlightPos, 3, 2, 0);
    // 2. right side, marker without physics (offsets not rotated), other random outcome
    Init(d); d.GoLeft = 0; d.marker_has_physics = false; d.marker_heading = 1.0f; d.seed = 2;
    d.IntoGroundSwoop();
    ok = ok && Near(d.flights[0].p[1], 11, 2, 0) && Near(d.flights[0].p[2], 12, 2, 0) && Near(d.flights[0].p[4], 100, 200 - 37.93019f, 25);
    // 3. rotated offset (heading 90 deg about Z)
    Init(d); d.marker_heading = 1.5707963f;
    d.IntoGroundSwoop();
    ok = ok && Near(d.flights[0].p[4], 100 + 37.93019f, 200, 25);
    // 4. enemy gone: terminate after the first flight
    Init(d); d.enemy_alive = false;
    d.IntoGroundSwoop();
    ok = ok && std::strcmp(d.trace, "fly,terminate,") == 0 && d.CurrentState == 0;
    // 5. stopping after the second flight is checked again
    Init(d); d.stopping_after_first = true;
    d.IntoGroundSwoop();
    ok = ok && std::strcmp(d.trace, "fly,terminate,") == 0;
    if (!ok) { std::printf("00951430_TEST FAIL\n"); return 1; }
    std::printf("00951430_TEST PASS\n");
    return 0;
}
