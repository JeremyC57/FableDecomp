// CLibpkObject1D8::SetVolumeParamPair(const SVolumeParams* first, const SVolumeParams* second) -- set or clear both volume-parameter records of the 3D audio object (+0x28/+0x44 and +0x114/+0x1b0); when that marked them dirty (bit 0x100 of +0x14), drop the loop-point records instead (pointers +0x24/+0x110, dirty 0x80 -> 0x1000) and raise bit 0x10000 of +0x10
// Leaf: ?SetVolumeParamPair@CLibpkObject1D8@@QAEXPBUSVolumeParams@@0@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C44800, 92 bytes, thiscall `ret 8`.
// Identity: called from the libpk API wrapper at 0x00C36000 (next to SetLoopPointPair's 0x00C35FD0); the helper 0x00C434A0
//   is CAudioObject::UpdateVolumeParams (bound name; landed SetVolumeParams 0x00C437A0). Descriptive (libpk, no PDB).
// Shape: two helper calls (`this` reloaded from esi), then one read-modify-write of +0x14 (`test ah,1`) and the zero stores.
struct SLoopPoints;
struct SVolumeParams {
    long m_0;
    long m_4;
};

class CAudioObject {
public:
    void UpdateVolumeParams(SVolumeParams** current, SVolumeParams* storage, const SVolumeParams* value);   // 0x00C434A0
    void*          m_vptr;                      // +0x00 (vtable 0x012B5A88)
    unsigned char  m_pad04[0x10 - 4];
    unsigned long  m_flags;                     // +0x10
    unsigned long  m_dirty;                     // +0x14
    unsigned char  m_pad18[0x24 - 0x18];
    SLoopPoints*   m_pLoopPoints;               // +0x24
    SVolumeParams* m_pVolumeParams;             // +0x28 current record (0 = none)
    unsigned char  m_pad2c[0x44 - 0x2c];
    SVolumeParams  m_volumeParamsStorage;       // +0x44
};

class CLibpkObject1D8 : public CAudioObject {   // vtable 0x012B5AB0
public:
    unsigned char  m_pad4c[0x110 - 0x4c];
    SLoopPoints*   m_pLoopPoints2;              // +0x110
    SVolumeParams* m_pVolumeParams2;            // +0x114
    unsigned char  m_pad118[0x1b0 - 0x118];
    SVolumeParams  m_volumeParamsStorage2;      // +0x1b0
    void SetVolumeParamPair(const SVolumeParams* first, const SVolumeParams* second);
};

void CLibpkObject1D8::SetVolumeParamPair(const SVolumeParams* first, const SVolumeParams* second)
{
    UpdateVolumeParams(&m_pVolumeParams, &m_volumeParamsStorage, first);
    UpdateVolumeParams(&m_pVolumeParams2, &m_volumeParamsStorage2, second);
    if (m_dirty & 0x100) {
        m_dirty = (m_dirty & ~0x80) | 0x1000;
        m_pLoopPoints = 0;
        m_pLoopPoints2 = 0;
        m_flags |= 0x10000;
    }
}