// CRenderManager2D::`scalar deleting destructor'(unsigned int) -- run the destructor, then free the object when flag bit 0 is set
// Leaf: ??_GCRenderManager2D@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x009E16E0, 30 bytes, thiscall, `ret 4`.
// Identity (proven, chunk c668): slot 0 of vtable 0x0129B9EC (RTTI CRenderManager2D), calls ??1CRenderManager2D@@UAE@XZ 0x009C2FD0.
// Callees: CRenderManager2D::~CRenderManager2D 0x009C2FD0 (??1CRenderManager2D@@UAE@XZ), operator delete 0x00BFE9BC.
// Naming wave 16: landed as the CRenderManager2D::ScalarDeletingDestructor (member stand-in)
//   calling CRenderManager2D::Dtor(); re-authored on the proven compiler-generated ??_G (name-only: bytes unchanged).
void __cdecl operator delete(void* p);                   // 0x00BFE9BC

class CDisplayManager;
class CWideString;

class CRenderManager2D {
public:
    CRenderManager2D(CDisplayManager&, const CWideString&, unsigned long);   // ??0CRenderManager2D@@QAE@AAVCDisplayManager@@ABVCWideString@@K@Z
    virtual ~CRenderManager2D();                        // 0x009C2FD0 (??1CRenderManager2D@@UAE@XZ)
};

// TU emitter: the PDB constructor (??0CRenderManager2D@@QAE@AAVCDisplayManager@@ABVCWideString@@K@Z) defined with an empty body emits the vtable and with it this
//   ??_G (the constructor body itself is its own row; not part of this graded body).
CRenderManager2D::CRenderManager2D(CDisplayManager&, const CWideString&, unsigned long) { }
