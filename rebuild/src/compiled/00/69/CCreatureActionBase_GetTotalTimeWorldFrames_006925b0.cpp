// CCreatureActionBase::GetTotalTimeWorldFrames() const -- the action's total time in world frames (TotalTime, +0x10)
// Leaf: ?GetTotalTimeWorldFrames@CCreatureActionBase@@QBEJXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006925B0, 4 bytes: mov eax,[ecx+0x10] ; ret.
// Identity (proven, chunk c667): Ego_r.pdb donor run HasPerformedAction 0x00518EF0 / GetTotalTimeWorldFrames 0x00518F00 /
//   GetFramesRunning 0x00518F10 / GetFramesRemaining 0x00518F20 / GetPercentageComplete 0x00518F30 = retail 0x006925A0 /
//   0x006925B0 / 0x006925C0 / 0x006925D0 / 0x006925E0; no GetTotalTime in either PDB.
// Layout: Ego_r.pdb CCreatureActionBase TotalTime +0x10 (engine/CCreatureActionBase.h is the flat layout; the member view is
//   local, as the landed siblings 0x006925D0 / 0x00692620).
// Naming wave 16: landed as the free __fastcall DrawGetEnvironment(const unsigned char*); respelled on the PDB member
//   (name-only: the bytes are unchanged).
class CCreatureActionBase {
public:
    long GetTotalTimeWorldFrames() const;
    unsigned char _pad_0x00[0x10];
    long TotalTime;                                     // +0x10
};

long CCreatureActionBase::GetTotalTimeWorldFrames() const
{
    return TotalTime;
}
