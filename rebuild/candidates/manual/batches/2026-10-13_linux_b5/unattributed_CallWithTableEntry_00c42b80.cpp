// <unattributed>::CallWithTableEntry(long) -- this->Method(this->Table[index]), tail call
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C42B80, oracle length 18 bytes.
// Grade: candidate -- still DIFFER under the Linux lane (Wine + DDK 13.10.4035 cl.exe), see batch README.
// The manifest label for this body is propagated across byte-identical bodies and is NOT trusted;
// the class is modelled from the bytes only and the name below describes what the code does.
//
// Retail bytes:  mov edx,[esp+4] ; mov eax,[ecx] ; mov eax,[eax+edx*4] ; mov [esp+4],eax ; jmp Method_00c1e900
// The index argument is replaced in its own stack slot by the table entry it selects (table pointer
// at +0x00) and the one-argument __thiscall method @ 0x00C1E900 is tail-called on the same object.
struct CTableOwner_Retail {
    void** Table;                           // +0x00
    void* Method_00c1e900(void* entry);     // 0x00C1E900
    void* CallWithTableEntry(long index);
};

void* CTableOwner_Retail::CallWithTableEntry(long index)
{
    return Method_00c1e900(this->Table[index]);
}
