// CScriptThing::`scalar deleting destructor'(unsigned int) -- release the PImp counted pointer, CBaseClass::~CBaseClass, free on bit 0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004AB170, oracle length 87 bytes.
// Callees: CBaseClass::~CBaseClass @ 0x0099A2E0, operator delete @ 0x00BFE9BC (both landed-trusted).
// Vtable 0x01238C8C = CScriptThing (rtti-vtables.tsv); the manifest label "C3DMeshObject::`scalar_deleting_destructor'"
// is propagated from the byte-identical body at 0x004C3250 (which releases a pointer at +8/+0xC) and is rejected.
//
// Retail bytes:  push esi ; mov esi,ecx ; mov [esi],offset CScriptThing::`vftable' ; mov eax,[esi+8] ; test eax,eax ; je N ;
//                dec dword ptr [eax] ; mov eax,[esi+8] ; cmp dword ptr [eax],0 ; jne N ; mov ecx,[eax+8] ; call [eax+4] ;
//                mov eax,[esi+8] ; push eax ; call operator delete ; add esp,4 ; N: mov ecx,esi ; mov dword ptr [esi+4],0 ;
//                mov dword ptr [esi+8],0 ; call CBaseClass::~CBaseClass ; test byte ptr [esp+8],1 ; je K ; push esi ; call operator delete ;
//                add esp,4 ; K: mov eax,esi ; pop esi ; ret 4
// PDB layout (struct_layouts_egor.tsv): CScriptThing = 12 bytes, +4 CCountedPointer<CScriptThing> PImp
// {object +4, count block +8}. The empty virtual destructor restores the vtable, runs the member's
// inline ~CCountedPointer (decrement the shared block, destroy + delete at zero, null both fields), then
// the base destructor; the deleting flavour frees `this` when bit 0 of the flag is set.
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

class CBaseClass {
public:
    CBaseClass();                               // 0x0099A2D0 (only needed by the constructing helper below)
    virtual ~CBaseClass();                      // 0x0099A2E0
};

class CScriptThing : public CBaseClass { public:
    CCountedPointer PImp;                  // +0x4
    virtual ~CScriptThing();
};

// The empty virtual destructor is defined out of line: VC7.1 then inlines it (and the member's
// ~CCountedPointer) into the deleting destructor; an in-class `{ }` body made the deleting destructor
// call ~CCountedPointer out of line instead (tried: DIFFER).
CScriptThing::~CScriptThing()
{
}

// VC7.1 emits a class's vtable (and the deleting destructor it references) in every translation
// unit that constructs the object; this helper is that construction, nothing more.
void* MakeOne()
{
    return new CScriptThing;
}