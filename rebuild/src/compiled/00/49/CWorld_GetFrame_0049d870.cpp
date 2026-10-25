// CWorld::GetFrame() -- static: the current world frame number (the file-static counter CWorld::SetFrame writes)
// Retail: Fable.exe (TLC, VC7.1) @ 0x0049D870, 6 bytes: `mov eax,[0x013B89BC] ; ret` (static __fastcall, no arguments).
// Identity: Ego_r.pdb + FableWin.pdb list ?GetFrame@CWorld@@SIJXZ, ?GetFrameAsString@CWorld@@SI?AVCCharString@@XZ,
//   ?SetFrame@CWorld@@SIXJ@Z, ?StartCountdownTimer@CWorld@@QAEXJ@Z back to back; retail has the same run: 0x0049D870 reads
//   0x013B89BC, 0x0049D880 formats it through GFIntToCharString 0x0099F830 into the hidden CCharString return, 0x0049D8A0
//   stores ecx into it, 0x0049D8B0 stores the argument at CWorld +0xA0 (Ego_r.pdb CWorld::CountdownTimer). The counter is
//   otherwise touched only by CWorld code (UpdateFrame, Save/LoadGameStateInternal, ActivateWorld, GetChecksum); 806 call
//   sites read it. Not GFGetPreMainMemoryUsed (Ego_r.pdb puts that at the image start, 0x00401960, in the startup TU).
// Leaf: ?GetFrame@CWorld@@SIJXZ
class CWorld {
public:
    static long __fastcall GetFrame();
private:
    static long Frame;                          // 0x013B89BC (descriptive name: the static is not in the PDB name sets)
};

long __fastcall CWorld::GetFrame()
{
    return Frame;
}
