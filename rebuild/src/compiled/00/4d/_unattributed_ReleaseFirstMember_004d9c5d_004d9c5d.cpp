// _unattributed::ReleaseFirstMember_004d9c5d() -- call 0x00BFEA14 on the first member when it is set (a release/free helper; owner unknown)
// Leaf: ?ReleaseFirstMember_004d9c5d@CFirstMemberOwner_004d9c5d@
// Renamed (fistp pass, 2026-10-24) from CFileInstaller::CActiveFile::OnReadFinished @ 00/4d/CActiveFile_OnReadFinished_004d9c5d.cpp: 14-byte `if (first member) call 0x00BFEA14(member)` (size-optimised: pop ecx); neither the owner nor the callee's class is recoverable from the bytes; the propagated OnReadFinished label is wrong (no vtable, no CFileInstaller caller); descriptive placeholder
// Retail: Fable.exe (TLC, VC7.1) @ 0x004D9C5D, 14 bytes.
#pragma optimize("s", on)
extern "C" void __cdecl free(void* block);   // 0x00BFEA14 (MSVCR71)
struct CFirstMemberOwner_004d9c5d {
    void* m_pFirst;                                  // +0x00
    void ReleaseFirstMember_004d9c5d();
};
void CFirstMemberOwner_004d9c5d::ReleaseFirstMember_004d9c5d()
{
    if (m_pFirst)
        free(m_pFirst);
}
