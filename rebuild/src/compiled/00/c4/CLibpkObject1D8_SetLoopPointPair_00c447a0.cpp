// CLibpkObject1D8::SetLoopPointPair(const SLoopPoints* first, const SLoopPoints* second) -- set or clear both loop-point records of the 3D audio object (+0x24/+0x3c and +0x110/+0x1a8); when that marked the loop points dirty (bit 0x80 of +0x14), drop the volume records instead (pointers +0x28/+0x114, dirty 0x100 -> 0x1000) and raise bit 0x10000 of +0x10
// Leaf: ?SetLoopPointPair@CLibpkObject1D8@@QAEXPBUSLoopPoints@@0@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C447A0, 91 bytes, thiscall `ret 8`.
// Identity: called from the libpk API wrapper at 0x00C35FD0; the helper 0x00C43420 is CAudioObject::UpdateLoopPoints (bound
//   name, landed SetLoopPoints 0x00C43780). The 3D object (vtable 0x012B5AB0, base CAudioObject 0x012B5A88) keeps a second
//   record set at +0x110.. and its dirty word at +0x1D8 (the landed CLibpkObject1D8 setters). Descriptive (libpk, no PDB).
// Shape: two helper calls (`this` reloaded from esi), then one read-modify-write of +0x14 and the zero stores.
struct SLoopPoints {
    long  m_start;
    float m_length;
};
struct SVolumeParams;

class CAudioObject {
public:
    void UpdateLoopPoints(SLoopPoints** current, SLoopPoints* storage, const SLoopPoints* value);   // 0x00C43420
    void*          m_vptr;                      // +0x00 (vtable 0x012B5A88)
    unsigned char  m_pad04[0x10 - 4];
    unsigned long  m_flags;                     // +0x10
    unsigned long  m_dirty;                     // +0x14
    unsigned char  m_pad18[0x24 - 0x18];
    SLoopPoints*   m_pLoopPoints;               // +0x24 current record (0 = none)
    SVolumeParams* m_pVolumeParams;             // +0x28
    unsigned char  m_pad2c[0x3c - 0x2c];
    SLoopPoints    m_loopPointsStorage;         // +0x3c
};

class CLibpkObject1D8 : public CAudioObject {   // vtable 0x012B5AB0
public:
    unsigned char  m_pad44[0x110 - 0x44];
    SLoopPoints*   m_pLoopPoints2;              // +0x110
    SVolumeParams* m_pVolumeParams2;            // +0x114
    unsigned char  m_pad118[0x1a8 - 0x118];
    SLoopPoints    m_loopPointsStorage2;        // +0x1a8
    void SetLoopPointPair(const SLoopPoints* first, const SLoopPoints* second);
};

void CLibpkObject1D8::SetLoopPointPair(const SLoopPoints* first, const SLoopPoints* second)
{
    UpdateLoopPoints(&m_pLoopPoints, &m_loopPointsStorage, first);
    UpdateLoopPoints(&m_pLoopPoints2, &m_loopPointsStorage2, second);
    if (m_dirty & 0x80) {
        m_dirty = (m_dirty & ~0x100) | 0x1000;
        m_pVolumeParams = 0;
        m_pVolumeParams2 = 0;
        m_flags |= 0x10000;
    }
}