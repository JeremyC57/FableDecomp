// CSmallVector<12-byte element>::operator[](unsigned long)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CD1FB1, oracle length 15 bytes.
// Grade: landed -- byte-exact (MATCH/RELOCATION_MATCH) and behaviour PASS under the Linux lane
//        (Wine + DDK 13.10.4035 cl.exe, 2026-10-13); the lander's winning pragma, if any, heads the file.
// The manifest label for this body is propagated across byte-identical bodies and is NOT trusted;
// the class is modelled from the bytes only and the name below describes what the code does.
//
// Retail bytes:  mov eax,[esp+4] ; mov ecx,[ecx] ; lea eax,[eax+eax*2] ; lea eax,[ecx+eax*4] ; ret 4
// The container keeps its element pointer at +0x00 and the stride is 12 bytes (index*3*4), so the
// manifest's `CANavQuadTreeNode const**` element type (4 bytes) cannot be right for this body.
struct CSmallVector12Element {
    long Values[3];                         // 12-byte element
};

struct CSmallVector12 {
    CSmallVector12Element* Data;            // +0x00
    CSmallVector12Element& operator[](unsigned long index);
};

CSmallVector12Element& CSmallVector12::operator[](unsigned long index)
{
    return this->Data[index];
}
