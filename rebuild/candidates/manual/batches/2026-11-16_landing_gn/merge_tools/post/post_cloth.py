PAIRS = [('''class C3DClothPrimitive {
public:
''', '''class C3DMeshFileClothChunk;
class C3DClothPrimitive {
public:
#ifdef FABLE_RETAIL_C3DCLOTHPRIMITIVE_CTOR
    // Opt-in (acctC wfE8, row 0x00A91640 C3DPrimitive2::InitialiseFromMeshFileData; moved here from the row's local model at landing gn):
    // the loader builds each cloth primitive with `new C3DClothPrimitive(chunk)` (operator new(0x14) at 0x00A92260, ctor 0x00AC3E00).
    // Declarations only (both out of line in retail); the destructor is declared non-virtual as the row's matched model had it.
    C3DClothPrimitive(const C3DMeshFileClothChunk* chunk);                          // 0x00AC3E00
    ~C3DClothPrimitive();
#endif
''')]
