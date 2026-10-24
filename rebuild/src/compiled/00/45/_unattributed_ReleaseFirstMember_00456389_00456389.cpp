// _unattributed::ReleaseFirstMember_00456389() -- call 0x00BFEA14 on the first member when it is set (a release/free helper; owner unknown)
// Leaf: ?ReleaseFirstMember_00456389@CFirstMemberOwner_00456389@
// Renamed (fistp pass, 2026-10-24) from CFileInstaller::CActiveFile::OnReadFinished @ 00/45/CActiveFile_OnReadFinished_00456389.cpp: 14-byte `if (first member) call 0x00BFEA14(member)` (size-optimised: pop ecx); neither the owner nor the callee's class is recoverable from the bytes; the propagated OnReadFinished label is wrong (no vtable, no CFileInstaller caller); descriptive placeholder
// Retail: Fable.exe (TLC, VC7.1) @ 0x00456389, 14 bytes.
#pragma optimize("s", on)
void __cdecl Release_00456389(void* p);                 // 0x00BFEA14
struct CFirstMemberOwner_00456389 {
    void* m_pFirst;                                  // +0x00
    void ReleaseFirstMember_00456389();
};
void CFirstMemberOwner_00456389::ReleaseFirstMember_00456389()
{
    if (m_pFirst)
        Release_00456389(m_pFirst);
}
