// CVertexShaderInput::~CVertexShaderInput() -- free the D3D declaration array, destroy and free the stream-info array, then the CBaseClass destructor
// Leaf: ??1CVertexShaderInput
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A5EB70, length 62 bytes.
// Evidence: the only caller is the scalar deleting destructor @ 0x00AB4690, slot 0 of CVertexShaderInput's vtable 0x0129EA08
// (rtti-vtables.tsv). The catalog label "CPatchTesselationEdgeStrip::~CPatchTesselationEdgeStrip" is propagated.
// Callees: free @ 0x00BFEA14 twice, the CStreamInfo range destroy @ 0x00A5E900 (`__fastcall` first/last, the iterator tag by
// reference homed in the high byte of the `push ecx` frame slot, `lea eax,[esp+7]`; labelled "_Destroy_range<allocator<CSpeechFrame>>",
// propagated), CBaseClass::~CBaseClass @ 0x0099A2E0 (stores the CBaseClass vtable 0x01231710; labelled "CBase").
// Layout: Ego_r.pdb CVertexShaderInput with the retail 12-byte CArray: DataStreams +0x04, D3DDeclaration +0x10 (retail
// size 0x1c). No engine header.
// Shape: members destroyed in reverse order (the declaration array freed, then the stream array destroyed and freed with
// `first` re-read for the null test), no vtable store of this class (`__declspec(novtable)`), then the out-of-line base
// destructor as a plain call (the frame slot is popped after it); `ret`.
extern "C" void __cdecl free(void* block);         // 0x00BFEA14

struct CBaseClass {                                // vtable 0x01231710
    virtual void v00();
    ~CBaseClass();                                 // 0x0099A2E0 (stores its vtable; labelled "CBase::CBase")
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
    virtual void v00();
    ~CVertexShaderInput();
};

CVertexShaderInput::~CVertexShaderInput()
{
}
