// CObservable::AddExclusiveObserver(long value, long unused1) -- stores argument 1 into the dword member at +0xc
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C59880, oracle length 10 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  mov eax, dword ptr [esp + 4] ; mov dword ptr [ecx + 0xc], eax ; ret 8
// Tiny-body shape `SetDword` synthesised from the instruction pattern (the class comes from the
// vtable-slot/manifest label when it is a plain class name, else the method is landed under a
// descriptive placeholder); member names are descriptive. 
struct CObservable {
    unsigned char _pad[0xc];
    long m_value;                          // +0xc
    void AddExclusiveObserver(long value, long unused1);
};

void CObservable::AddExclusiveObserver(long value, long unused1)
{
    m_value = value;
}
