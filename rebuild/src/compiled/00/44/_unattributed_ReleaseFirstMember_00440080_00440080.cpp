// _unattributed::ReleaseFirstMember_00440080() -- call 0x00BFEA14 on the first member when it is set (a release/free helper; owner unknown)
// Leaf: ?ReleaseFirstMember_00440080@CFirstMemberOwner_00440080@
// Renamed (fistp pass, 2026-10-24) from CFileInstaller::CActiveFile::OnReadFinished @ 00/44/CActiveFile_OnReadFinished_00440080.cpp: 14-byte `if (first member) call 0x00BFEA14(member)` (size-optimised: pop ecx); neither the owner nor the callee's class is recoverable from the bytes; the propagated OnReadFinished label is wrong (no vtable, no CFileInstaller caller); descriptive placeholder
// Retail: Fable.exe (TLC, VC7.1) @ 0x00440080, 14 bytes.
#pragma optimize("s", on)
void __cdecl Release_00440080(void* p);                 // 0x00BFEA14
struct CFirstMemberOwner_00440080 {
    void* m_pFirst;                                  // +0x00
    void ReleaseFirstMember_00440080();
};
void CFirstMemberOwner_00440080::ReleaseFirstMember_00440080()
{
    if (m_pFirst)
        Release_00440080(m_pFirst);
}
