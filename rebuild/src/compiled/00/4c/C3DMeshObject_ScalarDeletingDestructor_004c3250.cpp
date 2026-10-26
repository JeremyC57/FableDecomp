// C3DMeshObject::`scalar deleting destructor'(unsigned int) -- release the Mesh counted pointer, CBase::~CBase, free on bit 0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004C3250, oracle length 87 bytes.
// Callees: CBase::~CBase @ 0x0099A2E0, operator delete @ 0x00BFE9BC (both landed-trusted).
// Vtable 0x01239860 = C3DMeshObject, slot 0 of that vtable is this function (rtti-vtables.tsv).
//
// Retail bytes:  push esi ; mov esi,ecx ; mov [esi],offset C3DMeshObject::`vftable' ; mov eax,[esi+0Ch] ; test eax,eax ; je N ;
//                dec dword ptr [eax] ; mov eax,[esi+0Ch] ; cmp dword ptr [eax],0 ; jne N ; mov ecx,[eax+8] ; call [eax+4] ;
//                mov eax,[esi+0Ch] ; push eax ; call operator delete ; add esp,4 ; N: mov ecx,esi ; mov dword ptr [esi+8],0 ;
//                mov dword ptr [esi+0Ch],0 ; call CBase::~CBase ; test byte ptr [esp+8],1 ; je K ; push esi ; call operator delete ;
//                add esp,4 ; K: mov eax,esi ; pop esi ; ret 4
// PDB layout (struct_layouts_egor.tsv, C3DMeshObject = 120 bytes): +4 MeshInfo (raw pointer, untouched),
// +8 Mesh = CCountedPointer<C3DMeshStats const> {object +8, count block +0xC}. The empty virtual
// destructor restores the vtable, runs Mesh's inline ~CCountedPointer (decrement the shared block,
// destroy + delete at zero, null both fields), then CBase::~CBase; the deleting flavour frees `this`
// when bit 0 of the flag is set. Only the members the body touches are modelled.
struct SCountBlock {
    long m_references;                     // +0
    void (__fastcall* m_pfnDestroy)(void*);// +4
    void* m_pObject;                       // +8
};

struct CCountedPointer {
    void* m_pObject;                       // +0
    SCountBlock* m_pCount;                 // +4
    ~CCountedPointer()
    {
        if (m_pCount) {
            --m_pCount->m_references;
            if (m_pCount->m_references == 0) {
                m_pCount->m_pfnDestroy(m_pCount->m_pObject);
                delete m_pCount;
            }
        }
        m_pObject = 0;
        m_pCount = 0;
    }
};

struct CBase {
    CBase();                               // 0x0099A2D0 (only needed by the constructing helper below)
    virtual ~CBase();                      // 0x0099A2E0
};

class C3DMeshInfo;

class C3DMeshObject : public CBase { public:
    C3DMeshInfo*    MeshInfo;              // +0x4
    CCountedPointer Mesh;                  // +0x8  CCountedPointer<C3DMeshStats const>
    unsigned char   _rest[0x78 - 0x10];    // +0x10 PositionMatrix .. Scale (not touched here)
    virtual ~C3DMeshObject();
};

// The empty virtual destructor is defined out of line: VC7.1 then inlines it (and the member's
// ~CCountedPointer) into the deleting destructor; an in-class `{ }` body made the deleting destructor
// call ~CCountedPointer out of line instead (tried: DIFFER).
C3DMeshObject::~C3DMeshObject()
{
}

// VC7.1 emits a class's vtable (and the deleting destructor it references) in every translation
// unit that constructs the object; this helper is that construction, nothing more.
void* MakeOne()
{
    return new C3DMeshObject;
}