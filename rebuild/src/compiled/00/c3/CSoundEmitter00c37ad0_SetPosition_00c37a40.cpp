// CSoundEmitter_00c37ad0::SetPosition(long handle, const SEmitterPosition_00c37a40& position) -- slot 5 of the emitter: cache the 16-byte position block (+0x14) and mark it dirty (+0x24)
// Leaf: ?SetPosition@CSoundEmitter_00c37ad0@@UAEXJABUSEmitterPosition_00c37a40@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C37A40, 38 bytes, thiscall `ret 8` (the first argument is not read).
// Identity: slot 5 of the emitter vtable 0x012B5370 (the class of the landed constructor 0x00C37C90 and SyncVoice3D
//   0x00C37AD0, which keeps the cached position at +0x14); its sibling slot 2 0x00C37A10 stores the scalar at +0x10 the same
//   way (unused first argument). Descriptive (libpk, no PDB/RTTI): method and block names are inferred from SyncVoice3D.
// Shape: an implicit 16-byte struct copy (`lea eax,[ecx+14h]` then four dword moves through esi), then the byte flag.
struct SEmitterPosition_00c37a40 {
    float x, y, z;
    float w;
};

class CSoundEmitter_00c37ad0 {              // vtable 0x012B5370
public:
    virtual void v00();
    virtual void v01();
    virtual void SetScalar(long handle, float value);                         // slot 2 (0x00C37A10)
    virtual void v03(long handle, long value);
    virtual void v04(long handle, long value);
    virtual void SetPosition(long handle, const SEmitterPosition_00c37a40& position);   // slot 5
    void* m_pOwner;                         // +0x04
    long m_8;                               // +0x08
    long m_handle;                          // +0x0c
    float m_scalar;                         // +0x10
    SEmitterPosition_00c37a40 m_position;   // +0x14
    bool m_positionDirty;                   // +0x24
};

void CSoundEmitter_00c37ad0::SetPosition(long handle, const SEmitterPosition_00c37a40& position)
{
    m_position = position;
    m_positionDirty = true;
}