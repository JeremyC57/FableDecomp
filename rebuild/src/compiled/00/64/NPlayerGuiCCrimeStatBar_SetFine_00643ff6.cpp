// NPlayerGui::CCrimeStatBar::SetFine(long fine) -- store the fine the crime bar shows
// Leaf: ?SetFine@CCrimeStatBar@NPlayerGui@@QAEXJ@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00643FF6, 10 bytes: mov eax,[esp+4]; mov [ecx+68h],eax; ret 4 (thiscall).
// Naming wave 35c: Ego_r.pdb + FableWin.pdb ?SetFine@CCrimeStatBar@NPlayerGui@@QAEXJ@Z; called twice by the landed
//   CPlayerGui::UpdateCrime 0x004385F0, which binds this spelling. Was a placeholder member setter `T::Set(int)` (old file
//   CObservable_AddExclusiveObserver_00643ff6.cpp, Leaf AddExclusiveObserver; the manifest's NUISystem::CObservable label is propagated).
// Layout: as the landed UpdateCrime's model: Ego_r.pdb CCrimeStatBar CurrentFine at retail +0x68 (PDB +0x78; the retail CStatBase
//   is 0x10 shorter), the CStatBar / CStatBase bases flattened to padding here.
namespace NPlayerGui {
class CCrimeStatBar { public:
    unsigned char _pad00[0x68];                         // +0x00 CStatBar : CStatBase
    long          CurrentFine;                          // +0x68
    void SetFine(long fine);
};
}

void NPlayerGui::CCrimeStatBar::SetFine(long fine)
{
    CurrentFine = fine;
}
