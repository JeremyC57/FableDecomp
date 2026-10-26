// CEngineSimulationBase::GetMaximumIterations() -- returns the dword at +0x20 of the object the global @ 0x01436E68 points to
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B806C0, oracle length 9 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  mov eax,[0x1436e68] ; mov eax,[eax+0x20] ; ret
// Static-style getter: reads the global object pointer at 0x01436E68 and returns its dword member
// at +0x20; `this` is not used (`ret` with no arguments). Names are descriptive beyond the
// manifest label. One of 6 bodies of this shape.
struct CGlobalObject {
    unsigned char _pad[0x20];
    void* m_field;                         // +0x20
};

extern CGlobalObject* g_object_01436e68;    // 0x01436E68

class CEngineSimulationBase { public:
    void* GetMaximumIterations();
};

void* CEngineSimulationBase::GetMaximumIterations()
{
    return g_object_01436e68->m_field;
}