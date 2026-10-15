// CMarkedBuffer_00c44550::GetMark(SBufferMark& mark) -- fill the caller's 20-byte mark record (buffer, block, cursor, mark start/end) and return it
// Leaf: GetMark
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C442F0, length 35 bytes; callers 0x00C35AAC, 0x00C43308 (the landed
// CMarkedBufferOwner_00c43810::ReportMark 0x00C432F0, which names this callee GetMark on its +0x54 buffer), 0x00C437E7, 0x00C43837.
// The chunk label "CTCPhysicsRigidBody::GetFacingVector" is a propagated BSim name. Class named after the landed
// CMarkedBuffer_00c44550::Release (0x00C44550: m_pBlock +0, m_pCursor +4, m_pBuffer +8).
// Callees: none.
// Shape: five word copies through edx with the record pointer in eax, in source statement order (+8, +0x14, +0x10, +0, +4);
// eax still holds the record at `ret 4` = the reference is returned. A by-value return of a named local makes VC7.1 load all
// five words into callee-saved registers first (46 v 35).
struct SBufferMark {
    void* m_pBuffer;                               // +0x00
    void* m_pBlock;                                // +0x04
    void* m_pCursor;                               // +0x08
    long  m_markStart;                             // +0x0c
    long  m_markEnd;                               // +0x10
};

struct CMarkedBuffer_00c44550 {
    void* m_pBlock;                                // +0x00
    void* m_pCursor;                               // +0x04
    void* m_pBuffer;                               // +0x08
    long  m_reserved;                              // +0x0c (not read here)
    long  m_markStart;                             // +0x10
    long  m_markEnd;                               // +0x14
    SBufferMark& GetMark(SBufferMark& mark);
};

SBufferMark& CMarkedBuffer_00c44550::GetMark(SBufferMark& mark)
{
    mark.m_pBuffer = m_pBuffer;
    mark.m_markEnd = m_markEnd;
    mark.m_markStart = m_markStart;
    mark.m_pBlock = m_pBlock;
    mark.m_pCursor = m_pCursor;
    return mark;
}