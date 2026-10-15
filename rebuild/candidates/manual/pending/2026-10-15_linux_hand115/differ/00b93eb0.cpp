// CEngineSceneGrid::CEngineSceneGrid(long shift) -- CBase, the grid shift at +4, own vtable, cleared bounds, the side length 1 << shift and a cell vector resized to side * side zeros
// Grade: DIFFER(ours v retail) -- 80v80: register colouring only -- the side `1 << shift` lands in eax (retail edx) and the cell vector's `lea` in edx (retail eax); every instruction and its order otherwise agrees; tried: init-list `m_side(1 << shift)` with `resize(m_side * m_side, 0)` in the body (re-reads the member), a `long`/`unsigned long` local side with the store in the body (this form), an init-list vector(n, value) constructor (76 bytes, hoists)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B93EB0, length 80 bytes.
// Callees: CBase::CBase @ 0x0099A2F0, vector<unsigned int>::resize(unsigned int n, const unsigned int& value)
// @ 0x00B97A80. Vtable 0x012A3E58 = CEngineSceneGrid (rtti-vtables.tsv); the chunk label
// "CV_SingingStonesScript::CV_SingingStonesScript" is propagated.
// Shape: the shift stored at +4 before the vtable store (`__declspec(novtable)` argument base), `shl edx,cl`
// for the side, the value temporary 0 for resize homed in the dead `shift` argument slot (`lea ecx,[esp+0Ch]`
// pushed, `mov [esp+14h],edi` after the other push), the cell vector's three words zeroed through `lea eax,
// [esi+14h]` by its inline constructor, `imul` of the side kept in a LOCAL (edx) for the cell count, the side stored at +0x10 in the body; `ret 4`.
struct CBase {
    virtual void v00();
    CBase();                                       // 0x0099A2F0
};

struct __declspec(novtable) CEngineSceneGridBase : CBase {
    long m_shift;                                  // +0x04
    CEngineSceneGridBase(long shift) : CBase(), m_shift(shift) { }
};

struct CCellVector {                               // 12-byte retail vector<unsigned int>
    unsigned int* m_pFirst;
    unsigned int* m_pLast;
    unsigned int* m_pEnd;
    CCellVector() : m_pFirst(0), m_pLast(0), m_pEnd(0) { }
    void resize(unsigned int n, const unsigned int& value);   // 0x00B97A80
};

struct CEngineSceneGrid : CEngineSceneGridBase {   // vtable 0x012A3E58
    long        m_originX;                         // +0x08
    long        m_originY;                         // +0x0c
    long        m_side;                            // +0x10
    CCellVector m_cells;                           // +0x14
    virtual void v00();
    CEngineSceneGrid(long shift);
};

CEngineSceneGrid::CEngineSceneGrid(long shift)
    : CEngineSceneGridBase(shift), m_originX(0), m_originY(0), m_cells()
{
    long side = 1 << shift;                        // a local: `m_side * m_side` would re-read the member after its store
    m_side = side;
    m_cells.resize(side * side, 0);
}
