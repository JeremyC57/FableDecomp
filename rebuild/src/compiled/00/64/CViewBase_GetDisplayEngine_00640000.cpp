// CViewBase::GetDisplayEngine() -- returns the dword at +0x24 of the object the global @ 0x013B86A0 points to
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00640000, oracle length 9 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  mov eax,[0x13b86a0] ; mov eax,[eax+0x24] ; ret
// Static-style getter: reads the global object pointer at 0x013B86A0 and returns its dword member
// at +0x24; `this` is not used (`ret` with no arguments). Names are descriptive beyond the
// manifest label. One of 6 bodies of this shape.
struct CGlobalObject {
    unsigned char _pad[0x24];
    void* m_field;                         // +0x24
};

extern CGlobalObject* g_object_013b86a0;    // 0x013B86A0

struct CViewBase {
    void* GetDisplayEngine();
};

void* CViewBase::GetDisplayEngine()
{
    return g_object_013b86a0->m_field;
}