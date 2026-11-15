// Behaviour test for CRenderStateManager::SetDefaultRenderStates (0x00A059E0): a model of the row over the retail SetRenderState /
// AddStateToUpdateList logic -- every default is requested through the unchanged-value skip, the once-per-capture-block bookmark
// (old desired value pushed on the state stack, mask bit set) and the dirty list (each state queued once). Checks: the final desired
// value of every state (the second SetTextureFactor wins, stage 0 modulates texture by diffuse after the eight-stage reset, the
// stage loop gives each stage its own coordinate index), that a state already at its default is neither bookmarked nor queued, and
// that a state set twice in one block is bookmarked and queued only once with its ORIGINAL value on the stack.
#include <cstdio>
#include <cstring>

struct CStateInfo { unsigned long Desired, Mask; bool Dirty; };
enum { DITHER, ZEN, ZWRITE, ZFUNC, FILL, SHADE, ATEST, AREF, AFUNC, SRCBLEND, DSTBLEND, CULL, ABLEND, SPEC, TFACTOR, CLIP, FOG, FOGCOL,
       FOGMODE, FOGVMODE, FOGDENS, FOGSTART, FOGEND, VBLEND, CWRITE, DOFS, DSLOPE, PSPRITE, PSCALE, PSIZE, PSMIN, PSMAX, PSA, PSB, PSC,
       BLENDOP, LIGHT, NORM, STENCIL, SOPFAIL, SOPZFAIL, SOPPASS, SFUNC, SREF, SMASK, SWMASK, AMBSRC, DIFSRC, SPECSRC,
       STAGE0, PER_STAGE = 22, NSTATES = STAGE0 + 8 * PER_STAGE };
enum { COLOROP, ALPHAOP, ARG1, ARG2, AARG1, AARG2, MINF, MAGF, ADDRU, ADDRV, ADDRW, LODBIAS, MIPF, TEXIDX, TTF, B00, B10, B01, B11,
       WRAP, ANISO, BORDER };

struct Manager {
    CStateInfo State[NSTATES]; unsigned long CurrentBookmarkMask; int StackSize; int StackState[NSTATES * 2]; unsigned long StackValue[NSTATES * 2];
    int DirtySize; int DirtyList[NSTATES];
    void Set(int s, unsigned long v)
    {
        CStateInfo& st = State[s];
        if (st.Desired == v) return;
        if ((st.Mask & CurrentBookmarkMask) == 0) { st.Mask |= CurrentBookmarkMask; StackState[StackSize] = s; StackValue[StackSize] = st.Desired; ++StackSize; }
        st.Desired = v;
        if (!st.Dirty) { st.Dirty = true; DirtyList[DirtySize++] = s; }
    }
    void SetDefaultRenderStates();
};
static unsigned long F(float f) { unsigned long u; std::memcpy(&u, &f, 4); return u; }

// The row: D3D values the retail tables give for the engine enums the row passes (D3DCMP_LESSEQUAL 4, D3DFILL_SOLID 3, D3DSHADE_GOURAUD 2,
// D3DCMP_LESS 2 (alpha func table 0x01396DD8[1]), D3DBLEND_ONE 2 / ZERO 1, D3DCULL_CCW 3, D3DBLENDOP_ADD 1, D3DTOP_DISABLE 1 / MODULATE 4, D3DTA_TEXTURE 2 /
// DIFFUSE 0, D3DTEXF_LINEAR 2 (filter / mip tables 0x01396E58[1], 0x01396E64[1]), D3DTADDRESS_CLAMP 3, D3DSTENCILOP_KEEP 1, D3DCMP_ALWAYS 8, D3DMCS_MATERIAL 0).
void Manager::SetDefaultRenderStates()
{
    Set(DITHER, 1); Set(ZEN, 1); Set(ZWRITE, 1); Set(ZFUNC, 4); Set(FILL, 3); Set(SHADE, 2); Set(ATEST, 0); Set(AREF, 0x80); Set(AFUNC, 2);
    Set(SRCBLEND, 2); Set(DSTBLEND, 1); Set(CULL, 3); Set(ABLEND, 0); Set(SPEC, 0); Set(TFACTOR, 0xFF000000); Set(TFACTOR, 0xFFFFFFFF);
    Set(CLIP, 1); Set(FOG, 0); Set(FOGCOL, 0x00FFFFFF); Set(FOGMODE, 0); Set(FOGVMODE, 0); Set(FOGDENS, F(0.0f)); Set(FOGSTART, F(0.1f));
    Set(FOGEND, F(0.9f)); Set(VBLEND, 0); Set(CWRITE, 1); Set(DOFS, F(0.0f)); Set(DSLOPE, F(0.0f)); Set(PSPRITE, 0); Set(PSCALE, 0);
    Set(PSIZE, F(64.0f)); Set(PSMIN, F(1.0f)); Set(PSMAX, F(64.0f)); Set(PSA, F(1.0f)); Set(PSB, F(0.0f)); Set(PSC, F(0.0f)); Set(BLENDOP, 1);
    Set(LIGHT, 0); Set(NORM, 0);
    for (int stage = 0; stage < 8; ++stage) {
        int b = STAGE0 + stage * PER_STAGE;
        Set(b + COLOROP, 1); Set(b + ALPHAOP, 1); Set(b + ARG1, 2); Set(b + ARG2, 0); Set(b + AARG1, 2); Set(b + AARG2, 0); Set(b + MINF, 2);
        Set(b + MAGF, 2); Set(b + ADDRU, 3); Set(b + ADDRV, 3); Set(b + ADDRW, 3); Set(b + LODBIAS, F(0.0f)); Set(b + MIPF, 2);
        Set(b + TEXIDX, stage); Set(b + TTF, 0); Set(b + B00, F(0.0f)); Set(b + B10, F(0.0f)); Set(b + B01, F(0.0f)); Set(b + B11, F(0.0f));
        Set(b + WRAP, 0); Set(b + ANISO, 1); Set(b + BORDER, 0);
    }
    Set(STAGE0 + COLOROP, 4); Set(STAGE0 + ALPHAOP, 4); Set(STAGE0 + ARG1, 2); Set(STAGE0 + ARG2, 0); Set(STAGE0 + AARG1, 2); Set(STAGE0 + AARG2, 0);
    Set(STENCIL, 0); Set(SOPFAIL, 1); Set(SOPZFAIL, 1); Set(SOPPASS, 1); Set(SFUNC, 8); Set(SREF, 0); Set(SMASK, 0xFFFFFFFF); Set(SWMASK, 0xFFFFFFFF);
    Set(AMBSRC, 0); Set(DIFSRC, 0); Set(SPECSRC, 0);
}

static int g_fail = 0;
#define CHECK(x) do { if (!(x)) { std::printf("FAIL line %d: %s\n", __LINE__, #x); g_fail = 1; } } while (0)
static int Stacked(const Manager& m, int s) { int n = 0; for (int i = 0; i < m.StackSize; ++i) if (m.StackState[i] == s) ++n; return n; }
static int Queued(const Manager& m, int s) { int n = 0; for (int i = 0; i < m.DirtySize; ++i) if (m.DirtyList[i] == s) ++n; return n; }

int main()
{
    static Manager m; std::memset(&m, 0, sizeof m);
    for (int i = 0; i < NSTATES; ++i) m.State[i].Desired = 0x12345678;   // nothing at its default yet
    m.State[CLIP].Desired = 1;                                            // already at its default: skipped entirely
    m.State[TFACTOR].Desired = 0xABCDEF01;
    m.CurrentBookmarkMask = 4;
    m.SetDefaultRenderStates();
    CHECK(m.State[TFACTOR].Desired == 0xFFFFFFFF);                        // the second factor wins
    CHECK(Stacked(m, TFACTOR) == 1 && Queued(m, TFACTOR) == 1);          // bookmarked / queued once
    for (int i = 0; i < m.StackSize; ++i) if (m.StackState[i] == TFACTOR) CHECK(m.StackValue[i] == 0xABCDEF01);
    CHECK(Stacked(m, CLIP) == 0 && Queued(m, CLIP) == 0 && !m.State[CLIP].Dirty && m.State[CLIP].Mask == 0);
    CHECK(m.State[STAGE0 + COLOROP].Desired == 4 && m.State[STAGE0 + ARG2].Desired == 0);    // stage 0 modulate texture x diffuse
    CHECK(m.State[STAGE0 + PER_STAGE + COLOROP].Desired == 1);                              // stage 1 stays disabled
    CHECK(Stacked(m, STAGE0 + COLOROP) == 1 && Queued(m, STAGE0 + COLOROP) == 1);
    for (int s = 0; s < 8; ++s) CHECK(m.State[STAGE0 + s * PER_STAGE + TEXIDX].Desired == (unsigned long)s);
    CHECK(m.State[FOGSTART].Desired == F(0.1f) && m.State[PSIZE].Desired == F(64.0f) && m.State[SFUNC].Desired == 8);
    CHECK(m.State[SMASK].Desired == 0xFFFFFFFF && m.State[AREF].Desired == 0x80 && m.State[ZFUNC].Desired == 4);
    CHECK(m.StackSize == NSTATES - 1 && m.DirtySize == NSTATES - 1);   // every state but CLIP, each once
    // A second call in a new capture block: only the texture factor (0xFF000000 then 0xFFFFFFFF) and stage 0's colour / alpha operation
    // (the loop disables them, then they return to modulate) move, so exactly those three are bookmarked again with their old values;
    // all are still queued, the dirty list does not grow.
    int stack = m.StackSize, dirty = m.DirtySize; m.CurrentBookmarkMask = 8; m.SetDefaultRenderStates();
    CHECK(m.StackSize == stack + 3 && m.DirtySize == dirty);
    CHECK(m.StackState[stack] == TFACTOR && m.StackValue[stack] == 0xFFFFFFFF);
    CHECK(m.StackState[stack + 1] == STAGE0 + COLOROP && m.StackValue[stack + 1] == 4 && m.StackState[stack + 2] == STAGE0 + ALPHAOP && m.StackValue[stack + 2] == 4);
    CHECK(m.State[STAGE0 + COLOROP].Desired == 4 && m.State[STAGE0 + ALPHAOP].Desired == 4);
    // With no capture block (mask 0) the old values are still pushed (mask test 0 & x == 0), as retail does.
    static Manager n; std::memset(&n, 0, sizeof n); for (int i = 0; i < NSTATES; ++i) n.State[i].Desired = 0x55;
    n.SetDefaultRenderStates(); CHECK(n.StackSize > NSTATES); CHECK(Stacked(n, TFACTOR) == 2);
    if (g_fail) return 1;
    std::printf("00a059e0_TEST PASS\n");
    return 0;
}