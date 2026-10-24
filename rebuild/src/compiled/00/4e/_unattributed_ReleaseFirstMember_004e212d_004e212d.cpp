// _unattributed::ReleaseFirstMember_004e212d() -- call 0x00BFEA14 on the first member when it is set (a release/free helper; owner unknown)
// Leaf: ?ReleaseFirstMember_004e212d@CFirstMemberOwner_004e212d@
// Renamed (fistp pass, 2026-10-24) from CFileInstaller::CActiveFile::OnReadFinished @ 00/4e/CActiveFile_OnReadFinished_004e212d.cpp: 14-byte `if (first member) call 0x00BFEA14(member)` (size-optimised: pop ecx); neither the owner nor the callee's class is recoverable from the bytes; the propagated OnReadFinished label is wrong (no vtable, no CFileInstaller caller); descriptive placeholder
// Retail: Fable.exe (TLC, VC7.1) @ 0x004E212D, 14 bytes.
#pragma optimize("s", on)
void __cdecl Release_004e212d(void* p);                 // 0x00BFEA14
struct CFirstMemberOwner_004e212d {
    void* m_pFirst;                                  // +0x00
    void ReleaseFirstMember_004e212d();
};
void CFirstMemberOwner_004e212d::ReleaseFirstMember_004e212d()
{
    if (m_pFirst)
        Release_004e212d(m_pFirst);
}
