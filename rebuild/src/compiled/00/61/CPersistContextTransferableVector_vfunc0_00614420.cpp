// CPersistContext_TransferableVector::vfunc_0() -- the element count (8-byte elements) of the range object at +0x4
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00614420, oracle length 12 bytes.
// Grade: candidate -- generated for a repeated body shape and graded in the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); the batch README records the lane verdict.
//
// Retail bytes:  mov ecx, dword ptr [ecx + 4] ; mov eax, dword ptr [ecx + 4] ; sub eax, dword ptr [ecx] ; sar eax, 3 ; ret 
// Tiny-body shape `GetCountThrough` synthesised from the instruction pattern (the class comes from the
// manifest label when it is a plain class name, else the method is landed under a descriptive
// placeholder); member names are descriptive. 
struct SElement { unsigned char _body[0x8]; };

struct CRange {
    SElement* m_first;
    SElement* m_last;
};

struct CPersistContext_TransferableVector {
    unsigned char _pad[0x4];
    CRange* m_pRange;                      // +0x4
    long vfunc_0();
};

long CPersistContext_TransferableVector::vfunc_0()
{
    return m_pRange->m_last - m_pRange->m_first;
}