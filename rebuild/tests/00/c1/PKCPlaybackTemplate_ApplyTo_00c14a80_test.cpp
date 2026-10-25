// Behaviour test for PKCPlaybackTemplate::ApplyTo (0x00C14A80): base patch, then normal or alternate patch, optional range copy.
#include <cstdio>

struct PKCPlaybackState {
    unsigned short FlagsA;
    unsigned short FlagsB;
    long RangeStart;
    long RangeEnd;
};

class PKCFlagPatch {
public:
    unsigned short m_setBitsB, m_maskB, m_setBitsA, m_maskA;
    PKCFlagPatch(unsigned short sb, unsigned short mb, unsigned short sa, unsigned short ma)
        : m_setBitsB(sb), m_maskB(mb), m_setBitsA(sa), m_maskA(ma) {}
    PKCPlaybackState& ApplyTo(PKCPlaybackState& state) const
    {
        // stand-in for 0x00C2C670: keep the unmasked bits, then or in the patch's bits
        state.FlagsA = (unsigned short)((state.FlagsA & ~m_maskA) | m_setBitsA);
        state.FlagsB = (unsigned short)((state.FlagsB & ~m_maskB) | m_setBitsB);
        return state;
    }
};

class PKCPlaybackTemplate {
public:
    PKCFlagPatch BasePatch, NormalPatch, AlternatePatch;
    long RangeStart, RangeEnd;
    PKCPlaybackTemplate()
        : BasePatch(0x1, 0x1, 0x10, 0x10), NormalPatch(0x2, 0x2, 0x20, 0x20), AlternatePatch(0x4, 0x4, 0x40, 0x40),
          RangeStart(100), RangeEnd(200) {}
    PKCPlaybackState& ApplyTo(PKCPlaybackState& state, bool alternate, bool withRange)
    {
        BasePatch.ApplyTo(state);
        if (alternate)
            AlternatePatch.ApplyTo(state);
        else
            NormalPatch.ApplyTo(state);
        if (withRange) {
            state.RangeStart = RangeStart;
            state.RangeEnd = RangeEnd;
        }
        return state;
    }
};

int main()
{
    PKCPlaybackTemplate t;
    PKCPlaybackState s = { 0, 0, -1, -1 };
    PKCPlaybackState& r = t.ApplyTo(s, false, false);
    if (&r != &s || s.FlagsA != 0x30 || s.FlagsB != 0x3 || s.RangeStart != -1 || s.RangeEnd != -1) { std::printf("00c14a80_TEST FAIL\n"); return 1; }
    PKCPlaybackState u = { 0, 0, -1, -1 };
    t.ApplyTo(u, true, true);
    if (u.FlagsA != 0x50 || u.FlagsB != 0x5 || u.RangeStart != 100 || u.RangeEnd != 200) { std::printf("00c14a80_TEST FAIL\n"); return 1; }
    std::printf("00c14a80_TEST PASS\n");
    return 0;
}