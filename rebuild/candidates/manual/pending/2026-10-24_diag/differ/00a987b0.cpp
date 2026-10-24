// Grade: DIFFER(62v80) -- 2026-10-24 diag: as its twin 00a98760 (explicit inline destructor + __declspec(novtable) inlines the chain); the empty iterator tag's live-slot home (24l gap) is the residue. Previous: DIFFER(30v80).
// N3DPrimitive::CVertexShaderInputAnimated::`scalar deleting destructor'(unsigned int flags) -- vtable slot 0: the inlined destructor chain (free the D3D declaration array, destroy and free the stream-info array, CBaseClass destructor), then operator delete when bit 0 of flags is set
// Leaf: ??_GCVertexShaderInputAnimated
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A987B0, 80 bytes (`ret 4`).
// Identity: slot 0 of vtable 0x0129DFB0 = N3DPrimitive::CVertexShaderInputAnimated (rtti-vtables.tsv); the twin
// 0x00A98760 is the same body for the sibling class. The catalog label "Image8::ReleaseAll" is propagated.
// Callees: free 0x00BFEA14 twice, the CStreamInfo range destroy 0x00A5E900 (`__fastcall` first/last, the iterator tag by
// reference, homed in the flags argument slot: `lea eax,[esp+8]`), CBaseClass::~CBaseClass 0x0099A2E0 (stores the
// CBaseClass vtable 0x01231710), operator delete 0x00BFE9BC.
// Layout: Ego_r.pdb CVertexShaderInput with the retail 12-byte CArray: DataStreams +0x04, D3DDeclaration +0x10 (retail
// size 0x1c); no engine header. Model: the landed destructor 0x00A98640 (same members, same release order) with a
// virtual destructor so VC emits this `??_G`.
// Shape: members destroyed in reverse order, no vtable store of this class (it declares no destructor, and VC7.1's
// generated one stores none) or of CVertexShaderInput (`__declspec(novtable)`), the out-of-line base destructor as a plain call, `delete this` behind `test byte ptr [esp+8],1`.
extern "C" void __cdecl free(void* block);         // 0x00BFEA14
void __cdecl operator delete(void* block);         // 0x00BFE9BC

struct CBaseClass {                                // vtable 0x01231710
    virtual ~CBaseClass();                         // 0x0099A2E0 (stores its vtable; labelled "CBase::CBase")
};

struct SNonscalarPtrIteratorTag { SNonscalarPtrIteratorTag() { } };

struct CStreamInfo;
void __fastcall DestroyRange(CStreamInfo* first, CStreamInfo* last, const SNonscalarPtrIteratorTag& tag);   // 0x00A5E900

struct CStreamInfoArray {                          // retail CArray<CVertexShaderInput::CStreamInfo>: first / last / end
    CStreamInfo* m_pFirst;
    CStreamInfo* m_pLast;
    CStreamInfo* m_pEnd;
    ~CStreamInfoArray()
    {
        DestroyRange(m_pFirst, m_pLast, SNonscalarPtrIteratorTag());
        if (m_pFirst != 0)
            free(m_pFirst);
    }
};

struct D3DVERTEXELEMENT9;
struct CVertexElementArray {                       // retail CArray<_D3DVERTEXELEMENT9>: first / last / end
    D3DVERTEXELEMENT9* m_pFirst;
    D3DVERTEXELEMENT9* m_pLast;
    D3DVERTEXELEMENT9* m_pEnd;
    ~CVertexElementArray() { if (m_pFirst) free(m_pFirst); }
};

struct __declspec(novtable) CVertexShaderInput : CBaseClass {   // vtable 0x0129EA08
    CStreamInfoArray    DataStreams;               // +0x04
    CVertexElementArray D3DDeclaration;            // +0x10
    ~CVertexShaderInput() { }
};

namespace N3DPrimitive {
struct __declspec(novtable) CVertexShaderInputAnimated : CVertexShaderInput {
    CVertexShaderInputAnimated();
    ~CVertexShaderInputAnimated() { }                // explicit inline: the ??_G inlines the whole chain; novtable: no vtable store
};
}

// The real constructor is 0x00AB3B10 (base) + Initialise; defining one here makes VC emit the vtable and with it
// the `??_G` under test.
N3DPrimitive::CVertexShaderInputAnimated::CVertexShaderInputAnimated()
{
}
