// CCodaPlayback::CCodaPlayback() -- install the playback vtable and clear the two leading words and the CODA source pointer
// Leaf: ??0CCodaPlayback@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C27AF0, 20 bytes (`ret`, returns this). Descriptive names (no PDB or RTTI covers
//   the libpk audio library): the manifest's "SoundObject_Construct_c27af0" is propagated. The vtable 0x012B4034 holds
//   SetSourceProperty20 0x00C275E0, SetVolume 0x00C27620, ..., SetSourcePropertyFC 0x00C27670 and SetPosition 0x00C27550;
//   the constructor's address is taken at 0x00C23E63.
// Shape: `mov eax,ecx ; xor ecx,ecx` then the vptr and the three zero stores in member order; the remaining members
//   are left to the owner.
struct CPanel;
struct C3DVector;
struct CODASource;

struct CCodaPlayback {                          // vtable 0x012B4034
    virtual void SetSourceProperty20(float value);                        // slot 0 (0x00C275E0, landed)
    virtual void SetVolume(CPanel* panel, float volume);                   // slot 1 (0x00C27620)
    virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void SetSourcePropertyFC(float value);                        // slot 6 (0x00C27670, landed)
    virtual void SetPosition(const C3DVector& position);                   // slot 7 (0x00C27550)
    long          m_04;                         // +0x04
    long          m_08;                         // +0x08
    CODASource*   m_pSource;                    // +0x0C
    long          m_loopsLeft;                  // +0x10
    unsigned long m_loopPoint;                  // +0x14
    long          m_bArmed;                     // +0x18
    unsigned char _pad1c[4];
    unsigned long m_length;                     // +0x20
    bool          m_bLooping;                   // +0x24
    bool          m_b3D;                        // +0x25
    bool          m_bStarted;                   // +0x26
    CCodaPlayback();
};

CCodaPlayback::CCodaPlayback()
    : m_04(0), m_08(0), m_pSource(0)
{
}