// CEnginePrimitiveRenderer::ResetVisibilityRatios() -- run a CEngineSceneEnumeratorResetVisibilityRatio (built on the stack with a null argument) over every scene grid in the vector at +0x338
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B320A0, length 88 bytes.
// Callees: CBase::CBase @ 0x0099A2F0 and CBase::~CBase @ 0x0099A300 (the enumerator's base, out of line; the enumerator's own constructor is inlined: its vtable
// 0x012A12F4 and the argument at +4), the grid's Enumerate @ 0x00B93290 (on each grid; the enumerator by reference; the manifest label is propagated). Shape:
// the two-word enumerator lives in the frame; the vector's end is re-read each iteration; the loop head is aligned with `mov edi,edi`; the enumerator has no
// destructor of its own (only the base destructor runs: no vtable store).
namespace N_0099a300 { struct CBase {
    virtual void v00();
    CBase();                                       // 0x0099A2F0
    ~CBase();                                      // 0x0099A300
}; } using N_0099a300::CBase;

struct CEngineSceneEnumeratorResetVisibilityRatio : CBase {   // vtable 0x012A12F4
    void* m_argument;                              // +0x04
    virtual void v00();
    CEngineSceneEnumeratorResetVisibilityRatio(void* argument) : CBase(), m_argument(argument) { }
};

struct CEngineSceneGrid_00b320a0 {
    void Enumerate(CEngineSceneEnumeratorResetVisibilityRatio& enumerator);   // 0x00B93290
};

struct CEnginePrimitiveRenderer {
    unsigned char              _pad[0x338];
    CEngineSceneGrid_00b320a0** m_pFirstGrid;      // +0x338
    CEngineSceneGrid_00b320a0** m_pLastGrid;       // +0x33c
    void ResetVisibilityRatios();
};

void CEnginePrimitiveRenderer::ResetVisibilityRatios()
{
    CEngineSceneEnumeratorResetVisibilityRatio enumerator(0);
    for (CEngineSceneGrid_00b320a0** grid = m_pFirstGrid; grid != m_pLastGrid; ++grid)
        (*grid)->Enumerate(enumerator);
}