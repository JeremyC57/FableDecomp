// Behaviour test for 0x008EBC90 std::__find_if over NCreatureMode::CModeHoverBase::CHoverState with CIsValidHoverStateForVelocity
// The model mirrors the source: STLport 4x-unrolled random-access find_if and the PDB functor, over the landed
// CRadialArea::IsPosInArea (0x008EA3F0) logic.
#include <cstdio>
#include <cmath>

class C2DVector { public: float X, Y; };
struct CRadialArea {
    float ThetaLow, ThetaHigh, RadHigh;
    bool IsPosInArea(const C2DVector& pos, bool checkRadius) const
    {
        float length = (float)std::sqrt(pos.X * pos.X + pos.Y * pos.Y);
        if (length < 0.0001f) return true;
        double tl = ThetaLow * (double)6.2831855f, th = ThetaHigh * (double)6.2831855f;
        float lx = (float)std::sin(tl), ly = (float)std::cos(tl), hx = (float)std::sin(th), hy = (float)std::cos(th);
        float dx = pos.X / length, dy = pos.Y / length;
        if (!checkRadius || length < RadHigh) {
            float arcCos = hx * lx + hy * ly;
            if (dx * lx + dy * ly >= arcCos && dx * hx + dy * hy >= arcCos) return true;
        }
        return false;
    }
};
struct CHoverState { void* AnimName; CRadialArea Area; int MovementType; float MovementSpeed; };

struct Pred {
    C2DVector m_velocity;
    int       m_movementType;
    bool operator()(const CHoverState& s) const { return s.Area.IsPosInArea(m_velocity, true); }
};

template <class It, class P> It find_if4(It first, It last, P pred)
{
    int trip = (int)(last - first) >> 2;
    for (; trip > 0; --trip) {
        if (pred(*first)) return first; ++first;
        if (pred(*first)) return first; ++first;
        if (pred(*first)) return first; ++first;
        if (pred(*first)) return first; ++first;
    }
    switch (last - first) {
    case 3: if (pred(*first)) return first; ++first;
    case 2: if (pred(*first)) return first; ++first;
    case 1: if (pred(*first)) return first;
    case 0:
    default: return last;
    }
}

int main()
{
    int fail = 0;
    CHoverState states[12];
    for (int i = 0; i < 12; ++i) {
        states[i].AnimName = 0;
        // quarter-turn sectors around the clock; radius 5
        states[i].Area.ThetaLow = (float)(i % 4) * 0.25f;
        states[i].Area.ThetaHigh = (float)(i % 4) * 0.25f + 0.25f;
        states[i].Area.RadHigh = 5.0f;
        states[i].MovementType = i / 4;
        states[i].MovementSpeed = (float)i;
    }
    // velocity (sin 0.3*2pi, cos 0.3*2pi) * 3 lies in sector index 1 (0.25..0.5 turn)
    Pred p;
    p.m_velocity.X = (float)std::sin(0.3 * 6.2831853) * 3.0f;
    p.m_velocity.Y = (float)std::cos(0.3 * 6.2831853) * 3.0f;
    p.m_movementType = 2;
    for (int n = 0; n <= 12; ++n) {
        const CHoverState* r = find_if4((const CHoverState*)states, (const CHoverState*)states + n, p);
        int want = n;
        for (int i = 0; i < n; ++i) if (i % 4 == 1) { want = i; break; }
        if (r != states + want) fail |= 1;
    }
    if (find_if4((const CHoverState*)states, (const CHoverState*)states + 12, p) != states + 1) fail |= 2;
    p.m_velocity.X *= 4.0f; p.m_velocity.Y *= 4.0f;      // beyond RadHigh
    if (find_if4((const CHoverState*)states, (const CHoverState*)states + 12, p) != states + 12) fail |= 4;
    if (fail) { std::printf("008ebc90_TEST FAIL\n"); return 1; }
    std::printf("008ebc90_TEST PASS\n");
    return 0;
}