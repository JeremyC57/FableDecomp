// Behaviour test for 0x0087C6B0 CTurningAnimCompare::operator()(const CAnimationEntry*) const: true when the entry's
// rotation (a fraction of a turn) lies within 45 degrees of the functor's wanted rotation, wrapping around the turn.
#include <stdio.h>
#include <math.h>
static const float TWO_PI = 6.2831855f;
static const float QUARTER_PI = 0.78539819f;

class CAnimationEntry {
public:
    void GetRotationInAnimation(float* rotation) const; // 0x005DBFD0
};

class C2DVector { public:
    float X, Y;
    C2DVector(float x, float y) : X(x), Y(y) { }
    float Dot(const C2DVector& o) const { float d = Y * o.Y; d += X * o.X; return d; }
};

inline float Clamp(float v, float lo, float hi) { return v < lo ? lo : (v > hi ? hi : v); }

namespace {
class CTurningAnimCompare {                             // Ego_r.pdb, 4 bytes (a class: mangled V)
public:
    float RotationAngle;                                // +0x00
    bool operator()(const CAnimationEntry* entry) const;
};

bool CTurningAnimCompare::operator()(const CAnimationEntry* entry) const
{
    float rotation;
    entry->GetRotationInAnimation(&rotation);
    double entryAngle = rotation * (double)TWO_PI;
    C2DVector entryDirection((float)sin(entryAngle), (float)cos(entryAngle));
    double wantedAngle = RotationAngle * (double)TWO_PI;
    C2DVector wantedDirection((float)sin(wantedAngle), (float)cos(wantedAngle));
    float dot = Clamp(wantedDirection.Dot(entryDirection), -1.0f, 1.0f);
    if (dot > cos(QUARTER_PI))
        return true;
    return false;
}
}

static float g_rotation = 0.0f;
void CAnimationEntry::GetRotationInAnimation(float* rotation) const { *rotation = g_rotation; }

int main()
{
    int ok = 1;
    CAnimationEntry entry;
    CTurningAnimCompare cmp;
    struct { float wanted, entry; bool expect; } cases[] = {
        { 0.0f,   0.0f,   true  },
        { 0.0f,   0.1f,   true  },   // 36 degrees
        { 0.0f,   0.15f,  false },   // 54 degrees
        { 0.25f,  0.3f,   true  },
        { 0.25f,  0.5f,   false },   // 90 degrees
        { 0.95f,  0.05f,  true  },   // wraps: 36 degrees apart
        { 0.0f,   0.5f,   false },   // opposite
        { -0.1f,  0.0f,   true  },
    };
    for (unsigned int k = 0; k < sizeof(cases) / sizeof(cases[0]); ++k) {
        cmp.RotationAngle = cases[k].wanted;
        g_rotation = cases[k].entry;
        if (cmp(&entry) != cases[k].expect) ok = 0;
    }
    printf("0087c6b0_TEST %s\n", ok ? "PASS" : "FAIL");
    return ok ? 0 : 1;
}