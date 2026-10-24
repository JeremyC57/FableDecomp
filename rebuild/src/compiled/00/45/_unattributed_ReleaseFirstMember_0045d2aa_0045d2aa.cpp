// _unattributed::ReleaseFirstMember_0045d2aa() -- call 0x00BFEA14 on the first member when it is set (a release/free helper; owner unknown)
// Leaf: ?ReleaseFirstMember_0045d2aa@CFirstMemberOwner_0045d2aa@
// Renamed (fistp pass, 2026-10-24) from CFileInstaller::CActiveFile::OnReadFinished @ 00/45/CActiveFile_OnReadFinished_0045d2aa.cpp: 14-byte `if (first member) call 0x00BFEA14(member)` (size-optimised: pop ecx); neither the owner nor the callee's class is recoverable from the bytes; the propagated OnReadFinished label is wrong (no vtable, no CFileInstaller caller); descriptive placeholder
// Retail: Fable.exe (TLC, VC7.1) @ 0x0045D2AA, 14 bytes.
#pragma optimize("s", on)
void __cdecl Release_0045d2aa(void* p);                 // 0x00BFEA14
struct CFirstMemberOwner_0045d2aa {
    void* m_pFirst;                                  // +0x00
    void ReleaseFirstMember_0045d2aa();
};
void CFirstMemberOwner_0045d2aa::ReleaseFirstMember_0045d2aa()
{
    if (m_pFirst)
        Release_0045d2aa(m_pFirst);
}
