// _unattributed::ReleaseFirstMember_0045019a() -- call 0x00BFEA14 on the first member when it is set (a release/free helper; owner unknown)
// Leaf: ?ReleaseFirstMember_0045019a@CFirstMemberOwner_0045019a@
// Renamed (fistp pass, 2026-10-24) from CFileInstaller::CActiveFile::OnReadFinished @ 00/45/CActiveFile_OnReadFinished_0045019a.cpp: 14-byte `if (first member) call 0x00BFEA14(member)` (size-optimised: pop ecx); neither the owner nor the callee's class is recoverable from the bytes; the propagated OnReadFinished label is wrong (no vtable, no CFileInstaller caller); descriptive placeholder
// Retail: Fable.exe (TLC, VC7.1) @ 0x0045019A, 14 bytes.
#pragma optimize("s", on)
void __cdecl Release_0045019a(void* p);                 // 0x00BFEA14
struct CFirstMemberOwner_0045019a {
    void* m_pFirst;                                  // +0x00
    void ReleaseFirstMember_0045019a();
};
void CFirstMemberOwner_0045019a::ReleaseFirstMember_0045019a()
{
    if (m_pFirst)
        Release_0045019a(m_pFirst);
}
