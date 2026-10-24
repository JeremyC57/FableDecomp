// _unattributed::ReleaseFirstMember_0046089a() -- call 0x00BFEA14 on the first member when it is set (a release/free helper; owner unknown)
// Leaf: ?ReleaseFirstMember_0046089a@CFirstMemberOwner_0046089a@
// Renamed (fistp pass, 2026-10-24) from CFileInstaller::CActiveFile::OnReadFinished @ 00/46/CActiveFile_OnReadFinished_0046089a.cpp: 14-byte `if (first member) call 0x00BFEA14(member)` (size-optimised: pop ecx); neither the owner nor the callee's class is recoverable from the bytes; the propagated OnReadFinished label is wrong (no vtable, no CFileInstaller caller); descriptive placeholder
// Retail: Fable.exe (TLC, VC7.1) @ 0x0046089A, 14 bytes.
#pragma optimize("s", on)
void __cdecl Release_0046089a(void* p);                 // 0x00BFEA14
struct CFirstMemberOwner_0046089a {
    void* m_pFirst;                                  // +0x00
    void ReleaseFirstMember_0046089a();
};
void CFirstMemberOwner_0046089a::ReleaseFirstMember_0046089a()
{
    if (m_pFirst)
        Release_0046089a(m_pFirst);
}
