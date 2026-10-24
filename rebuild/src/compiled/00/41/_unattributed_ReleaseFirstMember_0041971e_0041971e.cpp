// _unattributed::ReleaseFirstMember_0041971e() -- call 0x00BFEA14 on the first member when it is set (a release/free helper; owner unknown)
// Leaf: ?ReleaseFirstMember_0041971e@CFirstMemberOwner_0041971e@
// Renamed (fistp pass, 2026-10-24) from CFileInstaller::CActiveFile::OnReadFinished @ 00/41/CActiveFile_OnReadFinished_0041971e.cpp: 14-byte `if (first member) call 0x00BFEA14(member)` (size-optimised: pop ecx); neither the owner nor the callee's class is recoverable from the bytes; the propagated OnReadFinished label is wrong (no vtable, no CFileInstaller caller); descriptive placeholder
// Retail: Fable.exe (TLC, VC7.1) @ 0x0041971E, 14 bytes.
#pragma optimize("s", on)
void __cdecl Release_0041971e(void* p);                 // 0x00BFEA14
struct CFirstMemberOwner_0041971e {
    void* m_pFirst;                                  // +0x00
    void ReleaseFirstMember_0041971e();
};
void CFirstMemberOwner_0041971e::ReleaseFirstMember_0041971e()
{
    if (m_pFirst)
        Release_0041971e(m_pFirst);
}
