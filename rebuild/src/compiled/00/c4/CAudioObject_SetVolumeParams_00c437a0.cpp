// CAudioObject::SetVolumeParams(const SVolumeParams* value) -- slot-7 virtual of the libpk audio object: forward the optional volume-parameter record to the shared set-or-clear helper with the object's current-pointer member (+0x28) and its inline storage (+0x44)
// Leaf: ?SetVolumeParams@CAudioObject@@UAEXPBUSVolumeParams@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C437A0, 21 bytes (+ int3 padding), __fastcall `ret 4`.
// Library: libpk audio (no PDB/RTTI): descriptive names. Slot 7 of the audio-object vtable 0x012B5A88 (slot 0 the
//   destructor 0x00C3CFC0, slot 2 IsOpen 0x00C3CFB0, slot 3 Open 0x00C42DD0, slot 4 Close 0x00C44170, slot 6
//   SetLoopPoints 0x00C43780); the derived vtable 0x012B5AB0 reaches it through 0x00C418A0. The manifest's
//   "CMusicManager_SetVolume" is propagated (CMusicManager is not this class).
// Callee: the landed set-or-clear helper 0x00C434A0 (`ret 0Ch`: current-pointer slot, storage, value; it marks dirty
//   bit 0x100 of +0x14).
// Shape: three pushes right to left (value, &storage, &current) and `this` left in ecx; the helper takes more stack
//   arguments than the forwarder, so there is no sibling-call `jmp`.
struct SVolumeParams {
    long  m_mode;
    float m_volume;
    float m_fadeTime;
};

class CAudioObject {
public:
    virtual void SetVolumeParams(const SVolumeParams* value);
    void UpdateVolumeParams(SVolumeParams** current, SVolumeParams* storage, const SVolumeParams* value);   // 0x00C434A0
    unsigned char m_pad04[0x28 - 4];
    SVolumeParams*  m_pVolumeParams;                // +0x28 current record (0 = none)
    unsigned char m_pad2c[0x44 - 0x2c];
    SVolumeParams   m_volumeParamsStorage;          // +0x44
};

void CAudioObject::SetVolumeParams(const SVolumeParams* value)
{
    UpdateVolumeParams(&m_pVolumeParams, &m_volumeParamsStorage, value);
}