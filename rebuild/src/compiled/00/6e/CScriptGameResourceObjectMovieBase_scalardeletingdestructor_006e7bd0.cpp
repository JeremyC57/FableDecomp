// CScriptGameResourceObjectMovieBase::`scalar deleting destructor'(unsigned int flags) -- the inlined destructor (release PImp, the base vtable, the root destructor) then operator delete when bit 0 is set
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006E7BD0, length 87 bytes.
// Leaf: ??_GCScriptGameResourceObjectMovieBase
// Retail bytes: push esi ; mov esi,ecx ; mov eax,[esi+0Ch] ; test eax,eax ; je Z ; dec dword ptr [eax] ; mov eax,[esi+0Ch] ; cmp dword ptr [eax],0 ; jne Z ;
//               mov ecx,[eax+8] ; call [eax+4] ; mov eax,[esi+0Ch] ; push eax ; call operator delete ; add esp,4 ; Z: mov dword ptr [esi+8],0 ;
//               mov dword ptr [esi+0Ch],0 ; mov ecx,esi ; mov dword ptr [esi],offset CScriptGameResourceObjectBase::`vftable' ; call ~root ;
//               test byte ptr [esp+8],1 ; je K ; push esi ; call operator delete ; add esp,4 ; K: mov eax,esi ; pop esi ; ret 4
// Slot 0 of CScriptGameResourceObjectMovieBase (vtable 0x01260EF4, rtti-vtables.tsv); relatives: the out-of-line destructor 0x006E7B80 and the constructor 0x006E7B60.
// Callees: CBaseIntelligentPointer::~CBaseIntelligentPointer @ 0x0099A430 (the root destructor; the manifest's "~CPhysicsMeshInfo" is
// propagated), operator delete @ 0x00BFE9BC. Layout (PDB): the 8-byte root, then PImp (CCountedPointer, +8 object / +0xc count block).
// Shape: the destructor stores no vtable of its own -- in retail the class declares no destructor and its implicit one is
// inlined into the deleting destructor (the out-of-line copy has the same body). The lane emits the implicit destructor out of
// line and a declared one stores the class's vptr first, so this file uses the GOTCHAS Wave 24m model: the polymorphic base as
// the FIRST MEMBER of a non-polymorphic class with a `__forceinline` destructor and a `delete` helper (VC7.1 emits `??_G` for it);
// the members are destroyed in reverse order -- PImp (the `__forceinline` counted-pointer release), then the base's inline
// virtual destructor (its vtable store and the out-of-line root destructor).
void __cdecl operator delete(void* block);     // 0x00BFE9BC

struct SCountBlock {                           // the shared count block of a CCountedPointer
    long  m_references;                        // +0x00
    void (__fastcall* m_pfnDestroy)(void*);    // +0x04 destroys the object (in ecx)
    void* m_pObject;                           // +0x08
};

template<class T> class CCountedPointer { public:     // {object, count block}
    T*           m_pObject;                    // +0x00
    SCountBlock* m_pCount;                     // +0x04
    __forceinline ~CCountedPointer()
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

namespace N_0099a430 { struct CBaseIntelligentPointer {               // the intelligent-pointer target root (8 bytes)
    virtual void v00();
    void* m_pReferences;                       // +0x04
    ~CBaseIntelligentPointer();                // 0x0099A430
}; } using N_0099a430::CBaseIntelligentPointer;

class CScriptGameResourceObjectBase : public CBaseIntelligentPointer { public:   // vtable 0x0126008C
    __forceinline virtual ~CScriptGameResourceObjectBase() {}
};

class CScriptGameResourceObjectMovieBase { public:   // retail: derives from CScriptGameResourceObjectBase (vtable 0x01260EF4)
    CScriptGameResourceObjectBase  m_base;     // +0x00 vptr + root
    CCountedPointer<CScriptGameResourceObjectMovieBase> PImp;   // +0x08
    __forceinline ~CScriptGameResourceObjectMovieBase() {}
};

void DeleteMovieBase(CScriptGameResourceObjectMovieBase* object)
{
    delete object;
}