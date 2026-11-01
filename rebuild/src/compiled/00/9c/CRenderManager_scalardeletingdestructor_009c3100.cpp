// CRenderManager::`scalar deleting destructor'(unsigned int) -- run the destructor, then free the object when flag bit 0 is set
// Leaf: ??_GCRenderManager@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x009C3100, 30 bytes, thiscall, `ret 4`.
// Identity (proven, chunk c668): slot 0 of vtable 0x0129B400 (RTTI CRenderManager); its implicit ~CRenderManager inlines to the call of ??1CRenderManager2D@@UAE@XZ 0x009C2FD0.
// Callees: CRenderManager2D::~CRenderManager2D 0x009C2FD0 (??1CRenderManager2D@@UAE@XZ; the implicit ~CRenderManager inlines to it), operator delete 0x00BFE9BC.
// Naming wave 16: landed as the CRenderManager::ScalarDeletingDestructor (member stand-in)
//   calling CRenderManager::Dtor(); re-authored on the proven compiler-generated ??_G (name-only: bytes unchanged).
void __cdecl operator delete(void* p);                   // 0x00BFE9BC

class CDisplayManager;
class CWideString;

class CRenderManager2D {
public:
    CRenderManager2D(CDisplayManager&, const CWideString&, unsigned long);   // ??0CRenderManager2D@@QAE@AAVCDisplayManager@@ABVCWideString@@K@Z
    virtual ~CRenderManager2D();                          // 0x009C2FD0 (??1CRenderManager2D@@UAE@XZ)
};

class CRenderManager : public CRenderManager2D {
public:
    CRenderManager(CDisplayManager&, const CWideString&, unsigned long);   // ??0CRenderManager@@QAE@AAVCDisplayManager@@ABVCWideString@@K@Z
};

// TU emitter: the PDB constructor (??0CRenderManager@@QAE@AAVCDisplayManager@@ABVCWideString@@K@Z) defined with an empty body emits the vtable and with it this
//   ??_G (the constructor body itself is its own row; not part of this graded body).
CRenderManager::CRenderManager(CDisplayManager& a0, const CWideString& a1, unsigned long a2) : CRenderManager2D(a0, a1, a2) { }
