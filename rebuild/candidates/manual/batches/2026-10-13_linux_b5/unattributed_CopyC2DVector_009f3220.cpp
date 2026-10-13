// __fastcall C2DVector-return helper: copies the 8-byte vector it is given
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009F3220, oracle length 15 bytes.
// Grade: landed -- byte-exact (MATCH/RELOCATION_MATCH) and behaviour PASS under the Linux lane
//        (Wine + DDK 13.10.4035 cl.exe, 2026-10-13); the lander's winning pragma, if any, heads the file.
// The manifest label for this body is propagated across byte-identical bodies and is NOT trusted;
// the class is modelled from the bytes only and the name below describes what the code does.
//
// Retail bytes:  mov eax,ecx ; mov ecx,[edx] ; mov edx,[edx+4] ; mov [eax],ecx ; mov [eax+4],edx ; ret 4
// __fastcall returning an 8-byte struct: ecx is the hidden result slot, edx the source vector, and
// one 4-byte stack argument is accepted and ignored (`ret 4`). The result is the source, copied.
struct C2DVector {
    float X;    // +0x00
    float Y;    // +0x04
    C2DVector() {}          // non-POD => returned through the hidden pointer (a POD comes back in edx:eax)
};

C2DVector __fastcall CopyC2DVector(const C2DVector& source, long unused)
{
    return source;
}
