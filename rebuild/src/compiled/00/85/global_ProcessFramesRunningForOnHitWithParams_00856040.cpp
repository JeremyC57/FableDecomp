// ProcessFramesRunningForOnHitWithParams(float& runningTime, float lastRunningTime, const std::pair<C3DVector,C3DVector>& previous, const std::pair<C3DVector,C3DVector>& current, const C3DVector& hitPosition) -- when enabled, wind a strike's running time back to the moment its weapon tip passed the hit: the hit's projection fraction along this frame's tip movement interpolates between the last and the current running time
// Leaf: ?ProcessFramesRunningForOnHitWithParams@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00856040, 266 bytes, /Gr __fastcall (runningTime in ecx, previous in edx; the
//   float, current and hitPosition on the stack), `ret 0Ch`.
// Identity: Ego_r.pdb ?ProcessFramesRunningForOnHitWithParams@?A0xad05d24c@@YIXAAMMABU?$pair@VC3DVector@@V1@@std@@1AB
//   VC3DVector@@@Z (the combat TU's anonymous namespace); its call site is CCombatActionBase::
//   ProcessPossibleCreatureStrikeEvent 0x00858130 (landed), `(RunningTimeSeconds, LastRunningTimeSeconds, previous,
//   current, hitPosition)` where each pair is (weapon base, weapon tip).
// Globals: the enabling flag 0x013B86CD (a .bss bool read only here; name not in the PDB dump), g_Epsilon 0x0129BA3C
//   (0.0001f).
// Shape: the tip movement and the tip-to-hit offset are C3DVector differences assigned into pre-declared locals (each
//   built in the shared 12-byte temporary and copied out), `lenSq > g_Epsilon` (`test ah,41h ; jne`), the fraction
//   |offset . movement| / lenSq (`fabs ; fdivrp`), and the inline Clamp(value, lo, hi) (`value < lo` first) of
//   lastRunningTime + (runningTime - lastRunningTime) * fraction, stored back through runningTime.
extern "C" double __cdecl fabs(double x);
#pragma intrinsic(fabs)

class C3DVector {
public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    float operator*(const C3DVector& o) const { return X * o.X + Y * o.Y + Z * o.Z; }   // dot product
};

namespace std {
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
};
}

template <class T> inline T Clamp(const T& value, const T& lo, const T& hi)
{
    if (value < lo)
        return lo;
    if (value > hi)
        return hi;
    return value;
}

extern const float g_Epsilon;                          // 0x0129BA3C (0.0001f)
extern bool g_ProcessFramesRunningForOnHit;            // 0x013B86CD

namespace {
void __fastcall ProcessFramesRunningForOnHitWithParams(float& runningTime, float lastRunningTime,
                                                       const std::pair<C3DVector, C3DVector>& previous,
                                                       const std::pair<C3DVector, C3DVector>& current,
                                                       const C3DVector& hitPosition)
{
    if (g_ProcessFramesRunningForOnHit) {
        C3DVector movement;
        movement = current.second - previous.second;
        C3DVector offset;
        offset = hitPosition - previous.second;
        float lengthSquared = movement * movement;
        if (lengthSquared > g_Epsilon) {
            float fraction = (float)fabs(offset * movement) / lengthSquared;
            runningTime = Clamp(lastRunningTime + (runningTime - lastRunningTime) * fraction, lastRunningTime, runningTime);
        }
    }
}
}

void (__fastcall *g_keepProcessFramesRunningForOnHitWithParams)(float&, float, const std::pair<C3DVector, C3DVector>&,
    const std::pair<C3DVector, C3DVector>&, const C3DVector&) = &ProcessFramesRunningForOnHitWithParams;   // keeps the body emitted