// CEngineInternalPrimitiveMeshBase_00781ce0::~CEngineInternalPrimitiveMeshBase_00781ce0() -- destroy and free the element vector at +0x10, release the mesh resource's counted pointer at +0xc, then the base destructor
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00781CE0, length 70 bytes.
// Callees: _Destroy_range @ 0x007817B0 (`__fastcall`: first in ecx, last in edx, the allocator temporary's address on the stack -- the high byte of the 4-byte local
// opened by `push ecx`: `lea eax,[esp+7]`), free @ 0x00BFEA14, the resource's virtual slot 1 (Destroy) at zero count, CTCBase::~CTCBase @ 0x00686830 (a call:
// the frame slot keeps it from being a sibling jump). Shape: the members are destroyed in reverse declaration order -- the inlined vector destructor (destroy the range, free the block)
// at +0x10, then the counted release at +0xc with an immediate null store; the destructor body itself is empty. No vtable store (a non-polymorphic class); the chunk label names the class family.
struct CRetailAllocator { CRetailAllocator() { } };
struct SMeshElement { unsigned char _body[8]; };
void __fastcall DestroyRange_007817b0(SMeshElement* first, SMeshElement* last, const CRetailAllocator& allocator);   // 0x007817B0

extern "C" void __cdecl free(void* block);        // 0x00BFEA14

struct CMeshResource {                             // counted object: vptr, then the reference count
    virtual void v00();
    virtual void Destroy();                        // slot 1
    long m_refCount;                               // +0x04
    void Release()
    {
        if (--m_refCount == 0)
            Destroy();
    }
};

struct CIVCountedPointer_Mesh {
    CMeshResource* m_p;
    ~CIVCountedPointer_Mesh()                      // the inlined member destructor: the release
    {
        if (m_p) {
            m_p->Release();
            m_p = 0;
        }
    }
};

class CTCBase { public:
    unsigned char _pad[0x0c];                      // +0x00 .. +0x0c
    ~CTCBase();                                    // 0x00686830
};

struct CRetailElementVector {                      // VC6 Dinkumware vector<pair<CCharString, CPersistInfo>>
    SMeshElement* m_pFirst;
    SMeshElement* m_pLast;
    SMeshElement* m_pEnd;
    ~CRetailElementVector()
    {
        DestroyRange_007817b0(m_pFirst, m_pLast, CRetailAllocator());
        if (m_pFirst)
            free(m_pFirst);
    }
};

struct CEngineInternalPrimitiveMeshBase_00781ce0 : CTCBase {
    CIVCountedPointer_Mesh m_resource;             // +0x0c
    CRetailElementVector   m_elements;             // +0x10
    ~CEngineInternalPrimitiveMeshBase_00781ce0();
};

CEngineInternalPrimitiveMeshBase_00781ce0::~CEngineInternalPrimitiveMeshBase_00781ce0()
{
}