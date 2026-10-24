// Grade: DIFFER(62v80) -- 2026-10-24 diag: an explicit inline destructor plus __declspec(novtable) on the derived class makes the ??_G inline the whole chain (two frees, the range destroy, the CBaseClass call, operator delete) as retail; the residue is the empty iterator tag, which retail homes on the LIVE flags slot (`lea eax,[esp+8]`) and ours gives its own byte (`push ecx ; lea eax,[esp+7]`): forwarding, side-effect and real-body DestroyRange stand-ins, with/without auto_inline(off), are inert (GOTCHAS 24l live-slot gap inside an inlined destructor chain). Previous: DIFFER(30v80).
// N3DPrimitive::CVertexShaderInputStatic::`scalar deleting destructor'(unsigned int flags) -- vtable slot 0: the inlined destructor chain (free the D3D declaration array, destroy and free the stream-info array, CBaseClass destructor), then operator delete when bit 0 of flags is set
// Leaf: ??_GCVertexShaderInputStatic
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A98760, 80 bytes (`ret 4`).
// Identity: slot 0 of vtable 0x0129DFB8 = N3DPrimitive::CVertexShaderInputStatic (rtti-vtables.tsv); the twin
// 0x00A987B0 is the same body for the sibling class. The catalog label "Image8::ReleaseAll" is propagated.
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

struct CVertexFormat : CBaseClass {              // 0x10 bytes: vptr, the element table, two words
    void* m_pElements;                             // +0x04 (freed by the destructor)
    unsigned long m_08;
    unsigned long m_0c;
    ~CVertexFormat() { if (m_pElements != 0) free(m_pElements); }
};
struct CStreamInfo {                               // 0x14 bytes
    unsigned long m_00;
    CVertexFormat Format;                          // +0x04
};
// The range destroy 0x00A5E900 with its REAL body in the TU (GOTCHAS 24l recipe: auto_inline(off) + noinline): VC7.1 then
// homes the empty tag temporary in the live `flags` argument slot as retail does. Not landed at 0x00A5E900.
#pragma auto_inline(off)
__declspec(noinline) void __fastcall DestroyRange(CStreamInfo* first, CStreamInfo* last, const SNonscalarPtrIteratorTag& tag)
{
    for (; first != last; ++first)
        first->~CStreamInfo();
}
#pragma auto_inline(on)

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
struct __declspec(novtable) CVertexShaderInputStatic : CVertexShaderInput {   // vtable 0x0129DFB8 (emitted here, with the ??_G)
    CVertexShaderInputStatic();
    ~CVertexShaderInputStatic() { }                // explicit inline: the ??_G inlines the whole chain; novtable: no vtable store
};
}

// The real constructor is 0x00AB3B10 (base) + Initialise; defining one here makes VC emit the vtable and with it
// the `??_G` under test.
N3DPrimitive::CVertexShaderInputStatic::CVertexShaderInputStatic()
{
}
