// CLibpkObject1D8::GetEffectiveGain() -- slot 5 of the 3D audio object: the product of the two gain factors (+0x134, +0x130) and the object's amplitude (+0x18), times MillibelsToAmplitude(+0x13c) when that attenuation is set
// Leaf: ?GetEffectiveGain@CLibpkObject1D8@@UAEMXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C412D0, 49 bytes, thiscall, plain `ret`, float in st0.
// Identity: slot 5 of the 3D audio-object vtable 0x012B5AB0, overriding the pure slot 5 of CAudioObject 0x012B5A88; the
//   amplitude at +0x18 is the one CAudioObject::SetMillibels 0x00C42FB0 caches. The converter 0x00C40EE0 is the landed
//   MillibelsToAmplitude_00c40ee0. Descriptive (libpk, no PDB/RTTI): the factors' roles are not proven.
// Shape: the product is spilled to the `push ecx` slot; the attenuated arm multiplies the call result by it
//   (`fmul [esp+4]`), the other reloads it.
float __cdecl MillibelsToAmplitude_00c40ee0(long millibels);   // 0x00C40EE0

class CAudioObject {                            // vtable 0x012B5A88
public:
    virtual ~CAudioObject();
    virtual bool Is3D();
    virtual bool IsOpen();
    virtual long Open();
    virtual void Close();
    virtual float GetEffectiveGain() = 0;       // slot 5
    unsigned char m_pad04[0x18 - 4];
    float         m_amplitude;                  // +0x18
};

class CLibpkObject1D8 : public CAudioObject {   // vtable 0x012B5AB0
public:
    virtual float GetEffectiveGain();
    unsigned char m_pad1c[0x130 - 0x1c];
    float         m_gain130;                    // +0x130
    float         m_gain134;                    // +0x134
    unsigned char m_pad138[4];
    long          m_attenuation;                // +0x13c (millibels, 0 = none)
};

float CLibpkObject1D8::GetEffectiveGain()
{
    float gain = m_gain134 * m_gain130 * m_amplitude;
    if (m_attenuation)
        return MillibelsToAmplitude_00c40ee0(m_attenuation) * gain;
    return gain;
}