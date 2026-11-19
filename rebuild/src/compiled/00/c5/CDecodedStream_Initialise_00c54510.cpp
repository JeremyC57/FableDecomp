// CDecodedStream::Initialise() -- set up the stream's voice: initialise the +0x2F8 voice effect for a 4-unit, 44100 Hz format (false when that fails), reset the current source properties (+0x224) to the static defaults, reset every pending reverb property of the +0x290 block through its clamped setters, then configure the voice effect from that block
// Leaf: ?Initialise@CDecodedStream@@QAE_NXZ
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C54510, 456 bytes, thiscall, plain `ret`, EH frame (the format local's inline
//   virtual destructor is the unwind action).
// Identity: the landed caller CDecodedStream::Open 0x00C547A0 (slot 1 of the CDecodedStream vtable 0x012B89C8, whose ctor
//   0x00C54090 / dtor 0x00C54110 store it) binds this address as `?Initialise@CDecodedStream@@QAE_NXZ` and returns
//   `Initialise() ? 0 : -9`. libpk: no PDB/RTTI, all names descriptive.
// Callees: CFloatReverbEngine::Create 0x00C6B050 (thiscall on +0x2F8, takes the 7-dword CReverbConstants, fills its zero size
//   fields from the rate and copies it to engine+0x234), CFloatReverbEngine::Commit 0x00C6AEF0 (NF99P2, q11498: the GameCODA 2.10
//   engine on engine/CFloatReverbEngine.h; this row bound both through the local stand-in CVoiceEffect as Initialise / Configure),
//   and the sixteen CHLRvbParams setters of the +0x290 block (NF102, q11576: GameCODA 2.10 CodeView names, the out-of-line copies
//   of the SDK header inlines; byte-identical to the CODAApi.lib SalSwReverb.obj COMDATs), in the SDK CSalSwReverb::Initialize order:
//   SetDecayTime 0x00C53440, SetDecayHFRatio 0x00C53500, SetDecayLFRatio 0x00C534A0, SetReflectionsGain 0x00C53560,
//   SetReflectionsDelay 0x00C535C0, SetReverbGain 0x00C53650, SetReverbDelay 0x00C536B0, SetSize 0x00C53A40, SetDiffusion 0x00C539E0,
//   SetDensity 0x00C53980, SetEchoTime 0x00C53800, SetEchoDepth 0x00C53860, SetModulationTime 0x00C538C0, SetModulationDepth
//   0x00C53920, SetLFReference 0x00C53740, SetHFReference 0x00C537A0.
// Globals: the zero vector 0x012B89E8 (an _RVB_VECTOR: the SDK function-local static vZero of CSalSwReverb::Initialize; the
//   descriptive name kept) and the 0x6C-byte default source properties 0x012B89F8 (.rdata, referenced only here), the
//   CReverbConstants vtable 0x012B89C4 (one slot: the scalar deleting destructor 0x00C53AE0).
// Shape: the format local is GameCODA's CReverbConstants (engine/CReverbConstants.h; was the local CVtable_012b89c4 model) with
//   its SDK header-inline defaults (opt-in FABLE_RETAIL_CREVERBCONSTANTS_INLINE_CTOR: rate and channel count overwritten, so only
//   the vptr and the four zero words survive) and its header-inline virtual destructor (EH state 0, no call at the exits);
//   `this + 0x2F8` is CSE'd and spilled; the pans are the SDK inline SetReflectionsPan / SetReverbPan (_RVB_VECTOR copies through
//   `lea ecx`), the three level words the SDK inline SetLevel / SetLevelHF / SetLevelLF (`value ; m_uiUpdateFlags |=`), each reached through the inline accessor GetPendingParameters() (retail
//   reloads the dirty word after every store: `or [esi],800h` / `or [esi],4000h` after the pan copies, then load / or / store
//   per level word); the sixteen out-of-line setters use the `p` reference.
#include "engine/C3DVector.h"
#define FABLE_RETAIL_PASS22_FLOATREVERB_CREATE
#define FABLE_RETAIL_CREVERBCONSTANTS_INLINE_CTOR
#define FABLE_RETAIL_CFLOATREVERBENGINE_COMMIT
#define FABLE_RETAIL_CHLRVBPARAMS_SETTERS
#define FABLE_RETAIL_CHLRVBPARAMS_SETLEVEL_INLINE
#define FABLE_RETAIL_CHLRVBPARAMS_SETPAN_INLINE
#include "engine/CReverbConstants.h"
#include "engine/CFloatReverbEngine.h"
#include "engine/CHLRvbParams.h"

struct SSourceProperties {                                 // +0x224 current properties, 0x6C bytes
    C3DVector     Position;                                // +0x00
    C3DVector     Velocity;                                // +0x0c
    long          m_levels[3];                             // +0x18
    float         m_values[18];                            // +0x24
};
extern const _RVB_VECTOR g_zeroVector_012b89e8;            // 0x012B89E8
extern const SSourceProperties g_defaultSourceProperties_012b89f8;   // 0x012B89F8

class CDecodedStream {                                     // vtable 0x012B89C8
public:
    virtual ~CDecodedStream();
    unsigned char          _pad004[0x224 - 4];
    SSourceProperties      m_properties;                   // +0x224
    CHLRvbParams           m_pending;                      // +0x290 (GameCODA CHLRvbParams, 0x68 bytes)
    CFloatReverbEngine     m_effect;                       // +0x2f8 (GameCODA float reverb engine)
    // Inline accessor (descriptive name, no symbol): each call yields a fresh pointer temp, so VC7.1 reloads the dirty word
    // after every inline setter exactly like retail; through one reference it folds the five updates into one register RMW.
    CHLRvbParams* GetPendingParameters() { return &m_pending; }
    bool Initialise();
};

bool CDecodedStream::Initialise()
{
    CReverbConstants format;
    format.m_uiSamplesPerSecond = 44100;
    format.m_uiNumOutputs = 4;
    if (!m_effect.Create(&format))
        return false;
    m_properties = g_defaultSourceProperties_012b89f8;
    CHLRvbParams& p = m_pending;
    GetPendingParameters()->SetReflectionsPan(g_zeroVector_012b89e8);
    GetPendingParameters()->SetReverbPan(g_zeroVector_012b89e8);
    GetPendingParameters()->SetLevel(-10000);
    GetPendingParameters()->SetLevelHF(0);
    GetPendingParameters()->SetLevelLF(0);
    p.SetDecayTime(1.0f);
    p.SetDecayHFRatio(0.5f);
    p.SetDecayLFRatio(1.0f);
    p.SetReflectionsGain(0.0f);
    p.SetReflectionsDelay(0.02f);
    p.SetReverbGain(0.0f);
    p.SetReverbDelay(0.04f);
    p.SetSize(7.5f);
    p.SetDiffusion(1.0f);
    p.SetDensity(1.0f);
    p.SetEchoTime(0.075f);
    p.SetEchoDepth(0.0f);
    p.SetModulationTime(0.04f);
    p.SetModulationDepth(0.0f);
    p.SetLFReference(200.0f);
    p.SetHFReference(5000.0f);
    m_effect.Commit(&p, true);
    return true;
}