// CAudioObject::SetLoopPoints(const SLoopPoints* value) -- slot-6 virtual of the libpk audio object: forward the optional loop-point record to the shared set-or-clear helper with the object's current-pointer member (+0x24) and its inline storage (+0x3c)
// Leaf: ?SetLoopPoints@CAudioObject@@UAEXPBUSLoopPoints@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C43780, 21 bytes (+ int3 padding), __fastcall `ret 4`.
// Library: libpk audio (no PDB/RTTI): descriptive names. Slot 6 of the audio-object vtable 0x012B5A88 (slot 0 the
//   destructor 0x00C3CFC0, slot 2 IsOpen 0x00C3CFB0, slot 3 Open 0x00C42DD0, slot 4 Close 0x00C44170, slot 7
//   SetVolumeParams 0x00C437A0); the derived vtable 0x012B5AB0 reaches it through 0x00C41880. The manifest's
//   "CMusicManager_SetLoop" is propagated (CMusicManager is not this class).
// Callee: the landed set-or-clear helper 0x00C43420 (`ret 0Ch`: current-pointer slot, storage, value; it marks dirty
//   bit 0x80 of +0x14).
// Shape: three pushes right to left (value, &storage, &current) and `this` left in ecx; the helper takes more stack
//   arguments than the forwarder, so there is no sibling-call `jmp`.
struct SLoopPoints {
    long  m_start;
    float m_length;
};

class CAudioObject {
public:
    virtual void SetLoopPoints(const SLoopPoints* value);
    void UpdateLoopPoints(SLoopPoints** current, SLoopPoints* storage, const SLoopPoints* value);   // 0x00C43420
    unsigned char m_pad04[0x24 - 4];
    SLoopPoints*  m_pLoopPoints;                // +0x24 current record (0 = none)
    unsigned char m_pad28[0x3c - 0x28];
    SLoopPoints   m_loopPointsStorage;          // +0x3c
};

void CAudioObject::SetLoopPoints(const SLoopPoints* value)
{
    UpdateLoopPoints(&m_pLoopPoints, &m_loopPointsStorage, value);
}