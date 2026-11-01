// CCreatureActionBase::GetFramesRunning() const -- the frames the action has been running (FramesRunning, +0x1C)
// Leaf: ?GetFramesRunning@CCreatureActionBase@@QBEJXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006925C0, 4 bytes: mov eax,[ecx+0x1c] ; ret.
// Identity (proven, chunk c667): Ego_r.pdb donor run (GetFramesRunning 0x00518F10 between GetTotalTimeWorldFrames and
//   GetFramesRemaining) = retail 0x006925C0; already bound so by landed rows.
// Layout: Ego_r.pdb CCreatureActionBase FramesRunning +0x1C (engine/CCreatureActionBase.h is the flat layout; the member view
//   is local, as the landed siblings 0x006925D0 / 0x00692620).
// Naming wave 16: landed as the free __fastcall DrawGetEnvironment(const unsigned char*) (catalog _global::GetMember_dword0x1c);
//   respelled on the PDB member (name-only: the bytes are unchanged).
class CCreatureActionBase {
public:
    long GetFramesRunning() const;
    unsigned char _pad_0x00[0x1c];
    long FramesRunning;                                 // +0x1C
};

long CCreatureActionBase::GetFramesRunning() const
{
    return FramesRunning;
}
