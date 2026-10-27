// C3DAnimationSequenceData::GetDuration(bool looping) -- length of the bone track: FrameCount frames when looping, one fewer otherwise, scaled by the per-sample factor at +4
// Leaf: ?GetDuration@C3DAnimationSequenceData@@QBEM_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A4BE80, 45 bytes, `ret 4`, float in st0.
// Identity: Ego_r.pdb `?GetDuration@C3DAnimationSequenceData@@QBEM_N@Z` (float (bool) const); in the donor it sits
//   directly before WriteCompressedToFile (0x00BA19E0 / 0x00BA1A20), as retail 0x00A4BE80 sits before the catalogued
//   C3DAnimationSequenceData::WriteCompressedToFile 0x00A4BEB0. The only absolute "refs" (0x004A2F61, 0x004A5E64) are
//   bytes inside other instructions; the function is called through inlined-away paths elsewhere.
// Layout: Ego_r.pdb C3DAnimationSequenceData (44 bytes): BoneName +0, SamplesPerSecond +4 (float), FrameCount +8
//   (unsigned short), bit fields +0xA. The unsigned short count widens to int (`movzx`) and is converted through the
//   dead argument slot (`fild dword ptr [esp+4]`) before `fmul dword ptr [ecx+4]`.
class C3DAnimationSequenceData {
public:
    char*          BoneName;                               // +0x00
    float          SamplesPerSecond;                       // +0x04
    unsigned short FrameCount;                             // +0x08
    float GetDuration(bool looping) const;
};

float C3DAnimationSequenceData::GetDuration(bool looping) const
{
    if (looping)
        return FrameCount * SamplesPerSecond;
    return (FrameCount - 1) * SamplesPerSecond;
}