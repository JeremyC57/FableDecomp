// CViewBase::GetDisplayEngine() -- returns the dword at +0x2c of the object the global @ 0x013B8A1C points to
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00686DA0, oracle length 9 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  mov eax,[0x13b8a1c] ; mov eax,[eax+0x2c] ; ret
// Static-style getter: reads the global object pointer at 0x013B8A1C and returns its dword member
// at +0x2c; `this` is not used (`ret` with no arguments). Names are descriptive beyond the
// manifest label. One of 6 bodies of this shape.
struct CGlobalObject {
    unsigned char _pad[0x2c];
    void* m_field;                         // +0x2c
};

extern CGlobalObject* g_object_013b8a1c;    // 0x013B8A1C

struct CViewBase {
    void* GetDisplayEngine();
};

void* CViewBase::GetDisplayEngine()
{
    return g_object_013b8a1c->m_field;
}
