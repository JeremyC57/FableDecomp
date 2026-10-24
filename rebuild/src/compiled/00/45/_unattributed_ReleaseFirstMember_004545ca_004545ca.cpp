// _unattributed::ReleaseFirstMember_004545ca() -- call 0x00BFEA14 on the first member when it is set (a release/free helper; owner unknown)
// Leaf: ?ReleaseFirstMember_004545ca@CFirstMemberOwner_004545ca@
// Renamed (fistp pass, 2026-10-24) from CFileInstaller::CActiveFile::OnReadFinished @ 00/45/CActiveFile_OnReadFinished_004545ca.cpp: 14-byte `if (first member) call 0x00BFEA14(member)` (size-optimised: pop ecx); neither the owner nor the callee's class is recoverable from the bytes; the propagated OnReadFinished label is wrong (no vtable, no CFileInstaller caller); descriptive placeholder
// Retail: Fable.exe (TLC, VC7.1) @ 0x004545CA, 14 bytes.
#pragma optimize("s", on)
void __cdecl Release_004545ca(void* p);                 // 0x00BFEA14
struct CFirstMemberOwner_004545ca {
    void* m_pFirst;                                  // +0x00
    void ReleaseFirstMember_004545ca();
};
void CFirstMemberOwner_004545ca::ReleaseFirstMember_004545ca()
{
    if (m_pFirst)
        Release_004545ca(m_pFirst);
}
