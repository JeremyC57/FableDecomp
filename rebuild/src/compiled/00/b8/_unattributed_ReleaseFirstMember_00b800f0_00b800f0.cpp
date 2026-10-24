// _unattributed::ReleaseFirstMember_00b800f0() -- call 0x00BFEA14 on the first member when it is set (a release/free helper; owner unknown)
// Leaf: ?ReleaseFirstMember_00b800f0@CFirstMemberOwner_00b800f0@
// Renamed (fistp pass, 2026-10-24) from CFileInstaller::CActiveFile::OnReadFinished @ 00/b8/CActiveFile_OnReadFinished_00b800f0.cpp: 14-byte `if (first member) call 0x00BFEA14(member)` (size-optimised: pop ecx); neither the owner nor the callee's class is recoverable from the bytes; the propagated OnReadFinished label is wrong (no vtable, no CFileInstaller caller); descriptive placeholder
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B800F0, 14 bytes.
#pragma optimize("s", on)
void __cdecl Release_00b800f0(void* p);                 // 0x00BFEA14
struct CFirstMemberOwner_00b800f0 {
    void* m_pFirst;                                  // +0x00
    void ReleaseFirstMember_00b800f0();
};
void CFirstMemberOwner_00b800f0::ReleaseFirstMember_00b800f0()
{
    if (m_pFirst)
        Release_00b800f0(m_pFirst);
}
