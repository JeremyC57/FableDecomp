// _unattributed::ReleaseFirstMember_004e3088() -- call 0x00BFEA14 on the first member when it is set (a release/free helper; owner unknown)
// Leaf: ?ReleaseFirstMember_004e3088@CFirstMemberOwner_004e3088@
// Renamed (fistp pass, 2026-10-24) from CFileInstaller::CActiveFile::OnReadFinished @ 00/4e/CActiveFile_OnReadFinished_004e3088.cpp: 14-byte `if (first member) call 0x00BFEA14(member)` (size-optimised: pop ecx); neither the owner nor the callee's class is recoverable from the bytes; the propagated OnReadFinished label is wrong (no vtable, no CFileInstaller caller); descriptive placeholder
// Retail: Fable.exe (TLC, VC7.1) @ 0x004E3088, 14 bytes.
#pragma optimize("s", on)
extern "C" void __cdecl free(void* block);   // 0x00BFEA14 (MSVCR71)
struct CFirstMemberOwner_004e3088 {
    void* m_pFirst;                                  // +0x00
    void ReleaseFirstMember_004e3088();
};
void CFirstMemberOwner_004e3088::ReleaseFirstMember_004e3088()
{
    if (m_pFirst)
        free(m_pFirst);
}
