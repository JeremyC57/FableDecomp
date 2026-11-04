// CDecodedStream::SetProperties(const SSourceUpdate_00c54190* update) -- slot 6: copy a new set of 3D/reverb source properties into the stream, pushing each one that changed into the +0x290 pending-parameter block (through its clamped setter, millibel levels converted to amplitudes), then reconfigure the voice effect from that block
// Leaf: ?SetProperties@CDecodedStream@@UAEXPBUSSourceUpdate_00c54190@@@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C54190, 889 bytes, thiscall `ret 4`.
// Identity (descriptive, libpk: no PDB/RTTI): slot 6 of the CDecodedStream vtable 0x012B89C4 (slot 0 the scalar deleting
//   destructor 0x00C53AE0, slot 2 the landed Open 0x00C547A0, slot 7 the landed PumpBlock 0x00C546E0); same TU as the
//   landed CDecodedStream::Initialise 0x00C54510 (the +0x224 current properties, the +0x290 pending block, the +0x2F8 voice
//   effect) and the TU's own MillibelsToAmplitude copy 0x00C53B60. The method and the descriptor names are descriptive; the
//   property names follow the EAX 3.0 listener set the clamp ranges of the landed setters reproduce (decay 0.1..20 s,
//   ratios 0.1..2, reflections 0..3.16 / reverb 0..10 amplitude, delays 0..0.3 / 0..0.1 s, echo 0.075..0.25 s, modulation
//   0.04..4 s, size 1..100, references 20..1000 / 1000..20000 Hz).
// Callees (landed spellings, one per address): C3DSoundSource::SetPosition 0x00C53D50 / SetVelocity 0x00C53E40 (the same
//   object: they write +0x224 / +0x230 and the pending pans +0x2DC / +0x2E8, so they are called on `this`), the sixteen
//   clamped setters of the +0x290 block under their four landed class spellings (CAttribute_00c53xxx,
//   CUnprovenReverbParameters, CUnprovenPendingParams_00c53740; NAME_FIXES.tsv carries the one-class proposal),
//   MillibelsToAmplitude_00c53b60 0x00C53B60 and CVoiceEffect::Configure 0x00C6AEF0 (mode 0).
// Shape: one inline compare-and-set helper per property taking the value BY VALUE (the float copies go through edx and
//   the dead argument slot for the `fld ; fld ; fucompp`, the stores and the push reuse the register); `member != value`;
//   the unsigned references convert with the 2^32 fix-up; the HF reference is pushed unconditionally.
class C3DVector { public:
    float X, Y, Z;
};
struct C3DSoundSource {
    void SetPosition(const C3DVector& position);           // 0x00C53D50
    void SetVelocity(const C3DVector& velocity);           // 0x00C53E40
};
float __cdecl MillibelsToAmplitude_00c53b60(long millibels);   // 0x00C53B60

struct SSourceProperties {                                 // 0x6C bytes (CDecodedStream +0x224)
    C3DVector     Position;                                // +0x00
    C3DVector     Velocity;                                // +0x0c
    long          Room;                                    // +0x18 (millibels)
    long          RoomLF;                                  // +0x1c
    long          RoomHF;                                  // +0x20
    float         DecayTime;                               // +0x24
    float         DecayLFRatio;                            // +0x28
    float         DecayHFRatio;                            // +0x2c
    long          Reflections;                             // +0x30 (millibels)
    float         ReflectionsDelay;                        // +0x34
    long          Reverb;                                  // +0x38 (millibels)
    float         ReverbDelay;                             // +0x3c
    float         EnvironmentSize;                         // +0x40
    float         m_44;                                    // +0x44 (pending +0x34)
    float         m_48;                                    // +0x48 (pending +0x38)
    float         EchoTime;                                // +0x4c
    float         EchoDepth;                               // +0x50
    float         ModulationTime;                          // +0x54
    float         ModulationDepth;                         // +0x58
    unsigned long HFReference;                             // +0x5c
    unsigned long LFReference;                             // +0x60
    float         m_64, m_68;                              // +0x64
};
struct SSourceUpdate_00c54190 {                            // the slot-6 argument (header words unused here)
    unsigned char     _pad00[0xc];
    SSourceProperties Properties;                          // +0x0c
};

// The +0x290 pending-property block (dirty word at +0): Configure 0x00C6AEF0 takes it as SVoiceEffectParameters.
struct SVoiceEffectParameters {
    unsigned long DirtyFlags;                              // +0x00
    long          m_room;                                  // +0x04 (dirty 0x20)
    long          m_roomHF;                                // +0x08 (dirty 0x40)
    long          m_roomLF;                                // +0x0c (dirty 0x80)
    void SetRoom(long v) { m_room = v; DirtyFlags |= 0x20; }
    void SetRoomHF(long v) { m_roomHF = v; DirtyFlags |= 0x40; }
    void SetRoomLF(long v) { m_roomLF = v; DirtyFlags |= 0x80; }
};
// The landed definers of the clamped setters (descriptive spellings, one per address).
struct CAttribute_00c53440 { void SetClamped_CAttribute_00c53440(float value); };   // 0x00C53440 +0x10
struct CAttribute_00c53500 { void SetClamped_CAttribute_00c53500(float value); };   // 0x00C53500 +0x18
struct CAttribute_00c534a0 { void SetClamped_CAttribute_00c534a0(float value); };   // 0x00C534A0 +0x14
struct CAttribute_00c53560 { void SetClamped_CAttribute_00c53560(float value); };   // 0x00C53560 +0x24
class CUnprovenReverbParameters { public:
    void SetClamped28(float value);                        // 0x00C535C0 +0x28
    void SetClamped30(float value);                        // 0x00C536B0 +0x30
    void SetClamped64(float value);                        // 0x00C53A40 +0x64
};
struct CAttribute_00c53650 { void SetClamped_CAttribute_00c53650(float value); };   // 0x00C53650 +0x2c
struct CUnprovenPendingParams_00c53740 {
    void SetField1C(float value);                          // 0x00C53740 +0x1c
    void SetField20(float value);                          // 0x00C537A0 +0x20
    void SetField38(float value);                          // 0x00C53980 +0x38
    void SetField34(float value);                          // 0x00C539E0 +0x34
};
struct CAttribute_00c53800 { void SetClamped_CAttribute_00c53800(float value); };   // 0x00C53800 +0x3c
struct CAttribute_00c53860 { void SetClamped_CAttribute_00c53860(float value); };   // 0x00C53860 +0x40
struct CAttribute_00c538c0 { void SetClamped_CAttribute_00c538c0(float value); };   // 0x00C538C0 +0x44
struct CAttribute_00c53920 { void SetClamped_CAttribute_00c53920(float value); };   // 0x00C53920 +0x48
struct CVoiceEffect {                                      // +0x2F8
    void Configure(const SVoiceEffectParameters* parameters, long mode);   // 0x00C6AEF0
};

class CDecodedStream {                                     // vtable 0x012B89C4
public:
    virtual ~CDecodedStream();
    bool                   m_audible;                      // +0x04 (room above -10000 mB)
    unsigned char          _pad005[0x224 - 5];
    SSourceProperties      m_properties;                   // +0x224
    SVoiceEffectParameters m_pending;                      // +0x290
    unsigned char          _pad2a0[0x2f8 - 0x2a0];
    CVoiceEffect           m_effect;                       // +0x2f8
    void SetRoom(long v)
    {
        if (m_properties.Room != v) {
            m_properties.Room = v;
            m_pending.SetRoom(v);
        }
    }
    void SetRoomLF(long v)
    {
        if (m_properties.RoomLF != v) {
            m_properties.RoomLF = v;
            m_pending.SetRoomLF(v);
        }
    }
    void SetRoomHF(long v)
    {
        if (m_properties.RoomHF != v) {
            m_properties.RoomHF = v;
            m_pending.SetRoomHF(v);
        }
    }
    void SetDecayTime(float v)
    {
        if (m_properties.DecayTime != v) {
            m_properties.DecayTime = v;
            reinterpret_cast<CAttribute_00c53440&>(m_pending).SetClamped_CAttribute_00c53440(v);
        }
    }
    void SetDecayLFRatio(float v)
    {
        if (m_properties.DecayLFRatio != v) {
            m_properties.DecayLFRatio = v;
            reinterpret_cast<CAttribute_00c53500&>(m_pending).SetClamped_CAttribute_00c53500(v);
        }
    }
    void SetDecayHFRatio(float v)
    {
        if (m_properties.DecayHFRatio != v) {
            m_properties.DecayHFRatio = v;
            reinterpret_cast<CAttribute_00c534a0&>(m_pending).SetClamped_CAttribute_00c534a0(v);
        }
    }
    void SetReflections(long v)
    {
        if (m_properties.Reflections != v) {
            m_properties.Reflections = v;
            reinterpret_cast<CAttribute_00c53560&>(m_pending).SetClamped_CAttribute_00c53560(MillibelsToAmplitude_00c53b60(v));
        }
    }
    void SetReflectionsDelay(float v)
    {
        if (m_properties.ReflectionsDelay != v) {
            m_properties.ReflectionsDelay = v;
            reinterpret_cast<CUnprovenReverbParameters&>(m_pending).SetClamped28(v);
        }
    }
    void SetReverb(long v)
    {
        if (m_properties.Reverb != v) {
            m_properties.Reverb = v;
            reinterpret_cast<CAttribute_00c53650&>(m_pending).SetClamped_CAttribute_00c53650(MillibelsToAmplitude_00c53b60(v));
        }
    }
    void SetReverbDelay(float v)
    {
        if (m_properties.ReverbDelay != v) {
            m_properties.ReverbDelay = v;
            reinterpret_cast<CUnprovenReverbParameters&>(m_pending).SetClamped30(v);
        }
    }
    void SetEnvironmentSize(float v)
    {
        if (m_properties.EnvironmentSize != v) {
            m_properties.EnvironmentSize = v;
            reinterpret_cast<CUnprovenReverbParameters&>(m_pending).SetClamped64(v);
        }
    }
    void SetValue44(float v)
    {
        if (m_properties.m_44 != v) {
            m_properties.m_44 = v;
            reinterpret_cast<CUnprovenPendingParams_00c53740&>(m_pending).SetField34(v);
        }
    }
    void SetValue48(float v)
    {
        if (m_properties.m_48 != v) {
            m_properties.m_48 = v;
            reinterpret_cast<CUnprovenPendingParams_00c53740&>(m_pending).SetField38(v);
        }
    }
    void SetEchoTime(float v)
    {
        if (m_properties.EchoTime != v) {
            m_properties.EchoTime = v;
            reinterpret_cast<CAttribute_00c53800&>(m_pending).SetClamped_CAttribute_00c53800(v);
        }
    }
    void SetEchoDepth(float v)
    {
        if (m_properties.EchoDepth != v) {
            m_properties.EchoDepth = v;
            reinterpret_cast<CAttribute_00c53860&>(m_pending).SetClamped_CAttribute_00c53860(v);
        }
    }
    void SetModulationTime(float v)
    {
        if (m_properties.ModulationTime != v) {
            m_properties.ModulationTime = v;
            reinterpret_cast<CAttribute_00c538c0&>(m_pending).SetClamped_CAttribute_00c538c0(v);
        }
    }
    void SetModulationDepth(float v)
    {
        if (m_properties.ModulationDepth != v) {
            m_properties.ModulationDepth = v;
            reinterpret_cast<CAttribute_00c53920&>(m_pending).SetClamped_CAttribute_00c53920(v);
        }
    }
    void SetLFReference(unsigned long v)
    {
        if (m_properties.LFReference != v) {
            m_properties.LFReference = v;
            reinterpret_cast<CUnprovenPendingParams_00c53740&>(m_pending).SetField1C((float)v);
        }
    }
    virtual void SetProperties(const SSourceUpdate_00c54190* update);
};

void CDecodedStream::SetProperties(const SSourceUpdate_00c54190* update)
{
    const SSourceProperties& p = update->Properties;
    m_audible = p.Room > -10000;
    reinterpret_cast<C3DSoundSource*>(this)->SetPosition(p.Position);
    reinterpret_cast<C3DSoundSource*>(this)->SetVelocity(p.Velocity);
    SetRoom(p.Room);
    SetRoomLF(p.RoomLF);
    SetRoomHF(p.RoomHF);
    SetDecayTime(p.DecayTime);
    SetDecayLFRatio(p.DecayLFRatio);
    SetDecayHFRatio(p.DecayHFRatio);
    SetReflections(p.Reflections);
    SetReflectionsDelay(p.ReflectionsDelay);
    SetReverb(p.Reverb);
    SetReverbDelay(p.ReverbDelay);
    SetEnvironmentSize(p.EnvironmentSize);
    SetValue44(p.m_44);
    SetValue48(p.m_48);
    SetEchoTime(p.EchoTime);
    SetEchoDepth(p.EchoDepth);
    SetModulationTime(p.ModulationTime);
    SetModulationDepth(p.ModulationDepth);
    SetLFReference(p.LFReference);
    reinterpret_cast<CUnprovenPendingParams_00c53740&>(m_pending).SetField20((float)p.HFReference);
    m_effect.Configure(&m_pending, 0);
}
