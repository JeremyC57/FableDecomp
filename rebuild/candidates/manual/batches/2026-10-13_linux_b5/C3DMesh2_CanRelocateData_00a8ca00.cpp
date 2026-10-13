// C3DMesh2::CanRelocateData(long)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A8CA00, oracle length 16 bytes.
// Grade: candidate -- authored and graded in the Linux lane (Wine + DDK 13.10.4035 cl.exe); the
//        batch README records the lane verdict.
//
// Retail bytes:  mov dl,[ecx+0E5h] ; xor eax,eax ; test dl,dl ; sete al ; ret 4
// The mesh's data may be relocated unless the lock flag at +0xE5 is set; the long argument is
// unused. `xor eax,eax ; sete al` is the plain `return !flag;` form.
struct C3DMesh2_Retail {
    unsigned char _pad_0x00[0xE5];
    bool          DataLocked;               // +0xE5 (name from use)
    bool CanRelocateData(long unused);
};

bool C3DMesh2_Retail::CanRelocateData(long unused)
{
    return !this->DataLocked;
}
