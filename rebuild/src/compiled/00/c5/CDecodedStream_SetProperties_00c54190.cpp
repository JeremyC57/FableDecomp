// CDecodedStream::SetProperties(const SSourceUpdate* update) -- slot 5: copy a new set of 3D/reverb source properties into the stream, pushing each one that changed into the +0x290 pending-parameter block (through its clamped setter, millibel levels converted to amplitudes), then reconfigure the voice effect from that block
// Leaf: ?SetProperties@CDecodedStream@@UAEXPBUSSourceUpdate@@@Z
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C54190, 889 bytes, thiscall `ret 4`.
// Identity (descriptive, libpk: no PDB/RTTI): slot 5 of the CDecodedStream vtable 0x012B89C8 (stored by the ctor 0x00C54090 /
//   dtor 0x00C54110; slot 1 the landed Open 0x00C547A0, slot 6 the landed PumpBlock 0x00C546E0); same TU as the
//   landed CDecodedStream::Initialise 0x00C54510 (the +0x224 current properties, the +0x290 pending block, the +0x2F8 voice
//   effect) and the TU's own MillibelsToAmplitude copy 0x00C53B60. The method and the descriptor names are descriptive; the
//   property names follow the EAX 3.0 listener set the clamp ranges of the landed setters reproduce (decay 0.1..20 s,
//   ratios 0.1..2, reflections 0..3.16 / reverb 0..10 amplitude, delays 0..0.3 / 0..0.1 s, echo 0.075..0.25 s, modulation
//   0.04..4 s, size 1..100, references 20..1000 / 1000..20000 Hz).
// Callees (landed spellings, one per address): C3DSoundSource::SetPosition 0x00C53D50 / SetVelocity 0x00C53E40 (the same
//   object: they write +0x224 / +0x230 and the pending pans +0x2DC / +0x2E8, so they are called on `this`), the sixteen
//   CHLRvbParams setters of the +0x290 block (NF102, q11576: the block is GameCODA's CHLRvbParams on engine/CHLRvbParams.h; the
//   setters are the GameCODA 2.10 CodeView members, byte-identical to the CODAApi.lib SalSwReverb.obj COMDATs, and the SDK
//   CSalSwReverb::Update relocations name every call below at the same offset), MillibelsToAmplitude_00c53b60 0x00C53B60 and
//   CFloatReverbEngine::Commit 0x00C6AEF0 (mode false; NF99P2 q11498: the GameCODA 2.10 engine on engine/CFloatReverbEngine.h).
//   The level words are the SDK header inlines SetLevel / SetLevelHF / SetLevelLF. Property names follow the SDK _CENVIRONMENT
//   (CODA.h): +0x1c is iLevelHF, +0x20 iLevelLF, +0x28 fDecayHFRatio, +0x2c fDecayLFRatio, +0x44 fDiffusion, +0x48 fDensity.
// Shape: one inline compare-and-set helper per property taking the value BY VALUE (the float copies go through edx and
//   the dead argument slot for the `fld ; fld ; fucompp`, the stores and the push reuse the register); `member != value`;
//   the unsigned references convert with the 2^32 fix-up; the HF reference is pushed unconditionally.
#include "engine/C3DVector.h"
#define FABLE_RETAIL_PASS22_FLOATREVERB_CREATE
#define FABLE_RETAIL_CFLOATREVERBENGINE_COMMIT
#define FABLE_RETAIL_CHLRVBPARAMS_SETTERS
#define FABLE_RETAIL_CHLRVBPARAMS_SETLEVEL_INLINE
#include "engine/CFloatReverbEngine.h"
#include "engine/CHLRvbParams.h"
struct C3DSoundSource {
    void SetPosition(const C3DVector& position);           // 0x00C53D50
    void SetVelocity(const C3DVector& velocity);           // 0x00C53E40
};
float __cdecl MillibelsToAmplitude_00c53b60(long millibels);   // 0x00C53B60

struct SSourceProperties {                                 // 0x6C bytes (CDecodedStream +0x224)
    C3DVector     Position;                                // +0x00
    C3DVector     Velocity;                                // +0x0c
    long          Room;                                    // +0x18 (millibels)
    long          RoomHF;                                  // +0x1c (SDK iLevelHF)
    long          RoomLF;                                  // +0x20 (SDK iLevelLF)
    float         DecayTime;                               // +0x24
    float         DecayHFRatio;                            // +0x28
    float         DecayLFRatio;                            // +0x2c
    long          Reflections;                             // +0x30 (millibels)
    float         ReflectionsDelay;                        // +0x34
    long          Reverb;                                  // +0x38 (millibels)
    float         ReverbDelay;                             // +0x3c
    float         EnvironmentSize;                         // +0x40
    float         Diffusion;                               // +0x44 (pending +0x34)
    float         Density;                                 // +0x48 (pending +0x38)
    float         EchoTime;                                // +0x4c
    float         EchoDepth;                               // +0x50
    float         ModulationTime;                          // +0x54
    float         ModulationDepth;                         // +0x58
    unsigned long HFReference;                             // +0x5c
    unsigned long LFReference;                             // +0x60
    float         m_64, m_68;                              // +0x64
};
struct SSourceUpdate {                            // the slot-5 argument (header words unused here)
    unsigned char     _pad00[0xc];
    SSourceProperties Properties;                          // +0x0c
};

class CDecodedStream {                                     // vtable 0x012B89C4
public:
    virtual ~CDecodedStream();
    bool                   m_audible;                      // +0x04 (room above -10000 mB)
    unsigned char          _pad005[0x224 - 5];
    SSourceProperties      m_properties;                   // +0x224
    CHLRvbParams           m_pending;                      // +0x290 (GameCODA CHLRvbParams, 0x68 bytes)
    CFloatReverbEngine     m_effect;                       // +0x2f8 (GameCODA float reverb engine)
    void SetRoom(long v)
    {
        if (m_properties.Room != v) {
            m_properties.Room = v;
            m_pending.SetLevel(v);
        }
    }
    void SetRoomHF(long v)
    {
        if (m_properties.RoomHF != v) {
            m_properties.RoomHF = v;
            m_pending.SetLevelHF(v);
        }
    }
    void SetRoomLF(long v)
    {
        if (m_properties.RoomLF != v) {
            m_properties.RoomLF = v;
            m_pending.SetLevelLF(v);
        }
    }
    void SetDecayTime(float v)
    {
        if (m_properties.DecayTime != v) {
            m_properties.DecayTime = v;
            m_pending.SetDecayTime(v);
        }
    }
    void SetDecayHFRatio(float v)
    {
        if (m_properties.DecayHFRatio != v) {
            m_properties.DecayHFRatio = v;
            m_pending.SetDecayHFRatio(v);
        }
    }
    void SetDecayLFRatio(float v)
    {
        if (m_properties.DecayLFRatio != v) {
            m_properties.DecayLFRatio = v;
            m_pending.SetDecayLFRatio(v);
        }
    }
    void SetReflections(long v)
    {
        if (m_properties.Reflections != v) {
            m_properties.Reflections = v;
            m_pending.SetReflectionsGain(MillibelsToAmplitude_00c53b60(v));
        }
    }
    void SetReflectionsDelay(float v)
    {
        if (m_properties.ReflectionsDelay != v) {
            m_properties.ReflectionsDelay = v;
            m_pending.SetReflectionsDelay(v);
        }
    }
    void SetReverb(long v)
    {
        if (m_properties.Reverb != v) {
            m_properties.Reverb = v;
            m_pending.SetReverbGain(MillibelsToAmplitude_00c53b60(v));
        }
    }
    void SetReverbDelay(float v)
    {
        if (m_properties.ReverbDelay != v) {
            m_properties.ReverbDelay = v;
            m_pending.SetReverbDelay(v);
        }
    }
    void SetEnvironmentSize(float v)
    {
        if (m_properties.EnvironmentSize != v) {
            m_properties.EnvironmentSize = v;
            m_pending.SetSize(v);
        }
    }
    void SetDiffusion(float v)
    {
        if (m_properties.Diffusion != v) {
            m_properties.Diffusion = v;
            m_pending.SetDiffusion(v);
        }
    }
    void SetDensity(float v)
    {
        if (m_properties.Density != v) {
            m_properties.Density = v;
            m_pending.SetDensity(v);
        }
    }
    void SetEchoTime(float v)
    {
        if (m_properties.EchoTime != v) {
            m_properties.EchoTime = v;
            m_pending.SetEchoTime(v);
        }
    }
    void SetEchoDepth(float v)
    {
        if (m_properties.EchoDepth != v) {
            m_properties.EchoDepth = v;
            m_pending.SetEchoDepth(v);
        }
    }
    void SetModulationTime(float v)
    {
        if (m_properties.ModulationTime != v) {
            m_properties.ModulationTime = v;
            m_pending.SetModulationTime(v);
        }
    }
    void SetModulationDepth(float v)
    {
        if (m_properties.ModulationDepth != v) {
            m_properties.ModulationDepth = v;
            m_pending.SetModulationDepth(v);
        }
    }
    void SetLFReference(unsigned long v)
    {
        if (m_properties.LFReference != v) {
            m_properties.LFReference = v;
            m_pending.SetLFReference((float)v);
        }
    }
    virtual void SetProperties(const SSourceUpdate* update);
};

void CDecodedStream::SetProperties(const SSourceUpdate* update)
{
    const SSourceProperties& p = update->Properties;
    m_audible = p.Room > -10000;
    reinterpret_cast<C3DSoundSource*>(this)->SetPosition(p.Position);
    reinterpret_cast<C3DSoundSource*>(this)->SetVelocity(p.Velocity);
    SetRoom(p.Room);
    SetRoomHF(p.RoomHF);
    SetRoomLF(p.RoomLF);
    SetDecayTime(p.DecayTime);
    SetDecayHFRatio(p.DecayHFRatio);
    SetDecayLFRatio(p.DecayLFRatio);
    SetReflections(p.Reflections);
    SetReflectionsDelay(p.ReflectionsDelay);
    SetReverb(p.Reverb);
    SetReverbDelay(p.ReverbDelay);
    SetEnvironmentSize(p.EnvironmentSize);
    SetDiffusion(p.Diffusion);
    SetDensity(p.Density);
    SetEchoTime(p.EchoTime);
    SetEchoDepth(p.EchoDepth);
    SetModulationTime(p.ModulationTime);
    SetModulationDepth(p.ModulationDepth);
    SetLFReference(p.LFReference);
    m_pending.SetHFReference((float)p.HFReference);
    m_effect.Commit(&m_pending, false);
}