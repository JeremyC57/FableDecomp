// CWorld::GetFrameAsString() -- static: the current world frame number formatted as a CCharString
// Leaf: ?GetFrameAsString@CWorld@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0049D880, 18 bytes, static __fastcall with the hidden CCharString result in ecx, plain `ret`.
// Identity: Ego_r.pdb + FableWin.pdb list ?GetFrame@CWorld@@SIJXZ, ?GetFrameAsString@CWorld@@SI?AVCCharString@@XZ,
//   ?SetFrame@CWorld@@SIXJ@Z back to back; retail has the same run: GetFrame 0x0049D870 (landed) reads the frame counter
//   0x013B89BC, this row formats the same counter, SetFrame 0x0049D8A0 writes it. Only caller: CCombatManager::RegisterCombatant
//   0x006EDC50 (debug-name build). The catalog's "list<CMessageEvent const*>" constructor label is propagated.
// Callee: GFIntToCharString(long) @ 0x0099F830 (__fastcall: hidden result in ecx, the value in edx).
// Shape: the hidden result pointer is kept in esi across the call and returned in eax; the counter goes straight to edx.
class CCharString {
public:
    void* PData;                                        // +0x00
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
};

CCharString __fastcall GFIntToCharString(long value);  // 0x0099F830

class CWorld {
public:
    static CCharString __fastcall GetFrameAsString();
private:
    static long Frame;                                  // 0x013B89BC (descriptive: the static is not in the PDB name sets)
};

CCharString __fastcall CWorld::GetFrameAsString()
{
    return GFIntToCharString(Frame);
}