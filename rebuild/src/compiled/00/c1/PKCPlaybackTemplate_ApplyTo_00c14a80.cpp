// PKCPlaybackTemplate::ApplyTo(PKCPlaybackState& state, bool alternate, bool withRange) -- libpk: apply the template's base flag patch, then its normal or alternate patch, optionally copy its two range words; return the state
// Leaf: ?ApplyTo@PKCPlaybackTemplate@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C14A80, 76 bytes (+ int3 padding), __fastcall `ret 0Ch`, returns the state reference.
// Identity: libpk (0x00C08AE0..0x00C40000: no PDB, no RTTI; every name here is descriptive). The three 12-byte members at
//   +0x78 / +0x84 / +0x90 are one flag-patch class (vtable 0x012B4630, set by its ctor 0x00C2C6B0: vptr + four zeroed
//   words) whose ApplyTo(state) @ 0x00C2C670 merges its two (bits, mask) word pairs into the state through 0x00C27D40 /
//   0x00C27D80 and returns the state. Only caller: 0x00C2C7B0, which forwards (state, alternate, withRange) to the
//   template at its +4 and then applies its own patch at +0x38. The catalog's "pair<CCharString, C3DMeshVolume>"
//   constructor label is propagated (no string or mesh volume is touched).
// Callee: PKCFlagPatch::ApplyTo(PKCPlaybackState&) const @ 0x00C2C670 (three calls; results dropped).
// Shape: an if/else of two ApplyTo calls: VC7.1 hoists the shared `push state` above the test and merges the calls
//   (a `(alternate ? AlternatePatch : NormalPatch).ApplyTo(state)` ternary selects `this` before the push).
struct PKCPlaybackState {
    unsigned char _pad00[0x48];
    long RangeStart;                        // +0x48
    long RangeEnd;                          // +0x4C
};

class PKCFlagPatch {                        // vtable 0x012B4630, 12 bytes
public:
    void*          m_vtable;                // +0x00
    unsigned short m_setBitsB;              // +0x04
    unsigned short m_maskB;                 // +0x06
    unsigned short m_setBitsA;              // +0x08
    unsigned short m_maskA;                 // +0x0A
    PKCPlaybackState& ApplyTo(PKCPlaybackState& state) const;   // 0x00C2C670
};

class PKCPlaybackTemplate {
public:
    unsigned char _pad00[0x78];
    PKCFlagPatch BasePatch;                 // +0x78
    PKCFlagPatch NormalPatch;               // +0x84
    PKCFlagPatch AlternatePatch;            // +0x90
    long         RangeStart;                // +0x9C
    long         RangeEnd;                  // +0xA0
    PKCPlaybackState& ApplyTo(PKCPlaybackState& state, bool alternate, bool withRange);
};

PKCPlaybackState& PKCPlaybackTemplate::ApplyTo(PKCPlaybackState& state, bool alternate, bool withRange)
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