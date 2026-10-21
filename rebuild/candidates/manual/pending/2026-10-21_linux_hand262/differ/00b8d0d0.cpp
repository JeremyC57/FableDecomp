// Grade: DIFFER(414v429) -- VC7.1 unrolls the 2x2 vertex loop's inner j loop (retail keeps both loops, bound in edi = the shared constant 2, both counters spilled for fild); index table and /GS frame otherwise as retail. Tried named size local, index form, <=, float locals, pragma s.
// CEngineOcclusionManager::InitializeOcclusionBuffers() -- create the unit-box vertex buffer (2x2 grid of XYZRHW+UV vertices) and the 36-entry index buffer
// Retail: Fable.exe @ 0x00B8D0D0, 429 bytes, /GS. Callees: CVertexBufferWin32::Initialise 0x00A63150, VB Lock 0x00AC20A0 / Unlock 0x00AC20B0 (manifest "CThingFilter_IsValid::operator()" is propagated), CIndexBuffer::Initialise 0x00AC1F70 / Lock 0x00AC1FB0 / Unlock 0x00AC1F00.
// Flags: /GS
extern "C" void* __cdecl memcpy(void* dest, const void* source, unsigned int count);
#pragma intrinsic(memcpy)
struct CUnitBoxVertex {
    float X, Y, Z, RHW, U, V;
};
struct CVertexBufferWin32 {
    unsigned char _body[0x34];
    bool Initialise(long type, long count, long stride, unsigned long usage, long pool);   // 0x00A63150
    void* Lock(unsigned long offset, unsigned long size, unsigned long flags);          // 0x00AC20A0
    void Unlock();                                                                      // 0x00AC20B0
};
struct CIndexBuffer {
    unsigned char _body[0x14];
    void Initialise(unsigned long count, unsigned long usage, long format, long pool, long primitiveType);   // 0x00AC1F70
    unsigned char* Lock(unsigned long offset, unsigned long size, unsigned long flags);   // 0x00AC1FB0
    void Unlock();                                                                        // 0x00AC1F00
};
struct CEngineOcclusionManager {
    unsigned char      _pad[0x24068];
    CVertexBufferWin32 UnitBoxVB;                  // +0x24068
    CIndexBuffer       UnitBoxIB;                  // +0x2409c
    void InitializeOcclusionBuffers();
};

void CEngineOcclusionManager::InitializeOcclusionBuffers()
{
    UnitBoxVB.Initialise(2, 8, 12, 0, 1);
    CUnitBoxVertex* vertex = (CUnitBoxVertex*)UnitBoxVB.Lock(0, 0, 0);
    for (int i = 0; i < 2; ++i)
        for (int j = 0; j < 2; ++j) {
            vertex->X = 0.0f;
            vertex->Y = (float)j;
            vertex->Z = (float)i;
            vertex->RHW = 1.0f;
            vertex->U = (float)j;
            vertex->V = (float)i;
            ++vertex;
        }
    UnitBoxVB.Unlock();
    UnitBoxIB.Initialise(0x48, 0, 0x65, 1, 4);
    unsigned short indices[36] = {
        2, 0, 1,  2, 1, 3,  4, 6, 5,  7, 5, 6,  1, 0, 4,  1, 4, 5,
        2, 3, 6,  7, 6, 3,  0, 2, 6,  6, 4, 0,  3, 1, 7,  7, 1, 5 };
    memcpy(UnitBoxIB.Lock(0, 0, 0), indices, sizeof(indices));
    UnitBoxIB.Unlock();
}
