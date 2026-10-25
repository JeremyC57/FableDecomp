// CSoundEmitter_00c37ad0::SyncVoice3D() -- when any parameter flag is set and the audio device exists: copy the flagged cached parameters (bit 1: the scalar at +0x10, bit 2: the 3-vector at +0x14) into locals, hand their addresses to the device's slot-0 update for the voice handle at +0xc, and copy the (possibly adjusted) values back
// Leaf: ?SyncVoice3D@CSoundEmitter_00c37ad0@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C37AD0, 192 bytes, __fastcall, no arguments.
// Library: libpk audio (no PDB/RTTI); descriptive names. The emitter is the class constructed at 0x00C37C90 (vtable
//   0x012B5370): owner +4, voice handle +0xc, scalar +0x10, vector +0x14, parameter flags +0x5c (bit 0 from the
//   constructor's first bool, bit 1 from its second). Called from that constructor (0x00C37D77) and from the
//   destructor-side 0x00C37C09. The device is reached owner->+0->+4->+0x38->+0x7cc. The manifest's
//   "Fastcall_CreatureOpinionApplyModifier" is propagated.
// Shape: the locals are a zero-constructed 3-vector and an uninitialised scalar; the optional pointers are
//   `flag ? &local : 0` (neg/sbb/and). The vector needs a memberwise inline operator=: the implicit struct copy goes
//   through lea + ebp (200 v 192).
struct CVector3_00c37ad0 {
    float x, y, z;
    CVector3_00c37ad0() : x(0), y(0), z(0) { }
    CVector3_00c37ad0& operator=(const CVector3_00c37ad0& v) { x = v.x; y = v.y; z = v.z; return *this; }
};

struct CVoiceDevice_00c37ad0 {
    virtual void Update(long handle, float* pScalar, void* p2, CVector3_00c37ad0* pPosition, void* p4);   // slot 0
};

struct CMixer_00c37ad0 {
    unsigned char _pad[0x7cc];
    CVoiceDevice_00c37ad0* m_pDevice;                   // +0x7cc
};

struct CAudio_00c37ad0 {
    unsigned char _pad[0x38];
    CMixer_00c37ad0* m_pMixer;                          // +0x38
};

struct CSystem_00c37ad0 {
    unsigned char _pad[4];
    CAudio_00c37ad0* m_pAudio;                          // +4
};

struct COwner_00c37ad0 {
    CSystem_00c37ad0* m_pSystem;                        // +0
    CVoiceDevice_00c37ad0* GetDevice() { return m_pSystem->m_pAudio->m_pMixer->m_pDevice; }
};

struct CSoundEmitter_00c37ad0 {
    void* _vtbl;                                        // +0 (vtable 0x012B5370)
    COwner_00c37ad0* m_pOwner;                          // +4
    long m_8;                                           // +8
    long m_handle;                                      // +0xc
    float m_scalar;                                     // +0x10
    CVector3_00c37ad0 m_position;                       // +0x14
    unsigned char _pad20[0x5c - 0x20];
    long m_flags;                                       // +0x5c
    void SyncVoice3D();
};

void CSoundEmitter_00c37ad0::SyncVoice3D()
{
    if (m_flags == 0)
        return;
    if (!m_pOwner->GetDevice())
        return;
    CVector3_00c37ad0 position;
    float scalar;
    CVector3_00c37ad0* pPosition = (m_flags & 2) ? &position : 0;
    float* pScalar = (m_flags & 1) ? &scalar : 0;
    if (pPosition)
        *pPosition = m_position;
    if (pScalar)
        *pScalar = m_scalar;
    m_pOwner->GetDevice()->Update(m_handle, pScalar, 0, pPosition, 0);
    if (pPosition)
        m_position = *pPosition;
    if (pScalar)
        m_scalar = *pScalar;
}