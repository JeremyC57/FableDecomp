// CEffectOwner_00c40750::CommitProperties() -- when the property block is dirty, clamp its two pan vectors to unit length, hand it to the effect and clear the dirty mask
// Grade: DIFFER(252v272) -- 37 masked lines: retail runs a 0x18 frame (y component of each pan in the second 12-byte slot, +0x14) and pre-stores the second pan's x/y/z into the result temporary before its length test; scalar locals give a 0x10 frame (y at +4), struct copies give memory loads (290 bytes)
// Leaf: ?CommitProperties@CEffectOwner_00c40750@@QAEXXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C40D10, length 272 bytes, thiscall, plain `ret`.
// Library: libpk / GameCODA audio (0x00C3xxxx-0x00C4xxxx, no PDB / RTTI): vendor naming rule, a descriptive name from the
//   body. The catalog autoname "NormalizeVectorPair" is descriptive too. Class: the landed CEffectOwner_00c40750 (Reset
//   0x00C40750 creates the effect at +0x7C and writes the 27-word property block at +0x0C whose first six words are the two
//   (0,0,1) vectors clamped here; the +0x78 dirty mask is the one SetProjection 0x00C40830 raises).
// Callers: CEAXManager::SetListenerProperty 0x00C52980 (after the block copy 0x00C40830), 0x00C4E717 (same pattern) and the
//   per-frame walk at 0x00C3D5C4.
// Behaviour: when the dirty mask (+0x78) is set, each pan vector (+0x0C, +0x18) longer than 1.0 (0x0122DED8) is scaled to unit
//   length, the effect (+0x7C) gets the owner through its vtable slot 5 (+0x14) and the mask is cleared.
#pragma intrinsic(sqrt)
extern "C" double __cdecl sqrt(double value);
struct CPKVector3_00c402e0 {
    float x, y, z;
    CPKVector3_00c402e0() { }
    CPKVector3_00c402e0(float ax, float ay, float az) : x(ax), y(ay), z(az) { }
};
class CEffectOwner_00c40750;
struct CEffect_00c40750 {
    virtual void v0(); virtual void v1(); virtual void v2(); virtual void v3(); virtual void v4();
    virtual long SetProperties(CEffectOwner_00c40750* owner);
};
class CEffectOwner_00c40750 { public:
    unsigned char _pad00[0x0c];
    CPKVector3_00c402e0 m_panA;            // +0x0c
    CPKVector3_00c402e0 m_panB;            // +0x18
    unsigned char _pad24[0x78 - 0x24];
    unsigned long m_dirtyFlags;            // +0x78
    CEffect_00c40750* m_pEffect;           // +0x7c
    void CommitProperties();
};
void CEffectOwner_00c40750::CommitProperties()
{
    if (m_dirtyFlags) {
        float x = m_panA.x, y = m_panA.y, z = m_panA.z;
        float length = (float)sqrt(z * z + y * y + x * x);
        if (length > 1.0f) {
            float scale = 1.0f / length;
            m_panA = CPKVector3_00c402e0(x * scale, y * scale, z * scale);
        }
        x = m_panB.x; y = m_panB.y; z = m_panB.z;
        length = (float)sqrt(z * z + y * y + x * x);
        if (length > 1.0f) {
            float scale = 1.0f / length;
            m_panB = CPKVector3_00c402e0(x * scale, y * scale, z * scale);
        }
        m_pEffect->SetProperties(this);
        m_dirtyFlags = 0;
    }
}
