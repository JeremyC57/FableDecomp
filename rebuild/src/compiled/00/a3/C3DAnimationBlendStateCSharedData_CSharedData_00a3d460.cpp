// C3DAnimationBlendState::CSharedData::~CSharedData() -- release the counted data pointer (+0x18/+0x1c), destroy and free the blend-entry vector (+0x0c), release the intrusive owner pointer (+0x08), then the CIVCountedPointeeBase part
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A3D460, length 129 bytes. Called only by the deleting dtor 0x00A3D440
// (slot 0 of vtable 0x0129D1F8 = C3DAnimationBlendState::CSharedData per rtti-vtables.tsv).
// Callees: operator delete @ 0x00BFE9BC (counted block), free @ 0x00BFEA14 (vector storage), element virtual dtor (slot 0, flag 0),
// owner virtual slot 1 (release at zero count); base vtable 0x012354D0 restored at the end.
// Shape: no own vtable store at entry = `__declspec(novtable)` on the derived class (the base's store stays). The counted
// block holds {count, member-function deleter, object}: `mov ecx,[eax+8] ; call [eax+4]` is `(obj->*pfn)()` and the
// class must be complete or VC emits the 16-byte general member-pointer call. Every member dtor is `__forceinline`;
// the vector uses an STLport-style `_Destroy(first, last)` helper (end kept in ebx); the owner release is a local copy.
// Leaf: ??1CSharedData
extern "C" void __cdecl free(void* block);         // 0x00BFEA14
void __cdecl operator delete(void* block);         // 0x00BFE9BC

struct CIVCountedPointeeBase {                     // vtable 0x012354D0
    virtual ~CIVCountedPointeeBase() {}
    long m_references;                             // +4
};

struct CIVObject {
    virtual void v00();
    virtual void Release();                        // slot 1
    long m_references;                             // +4
};

struct CIVPointer {
    CIVObject* m_p;
    __forceinline ~CIVPointer()
    {
        CIVObject* p = m_p;
        if (p) {
            if (--p->m_references == 0)
                p->Release();
            m_p = 0;
        }
    }
};

struct CBlendEntry {                               // 0x14-byte polymorphic element
    virtual ~CBlendEntry();
    unsigned char _body[0x10];
};

struct CBlendEntryVector {                         // STLport vector<CBlendEntry>
    CBlendEntry* _M_start;
    CBlendEntry* _M_finish;
    CBlendEntry* _M_end_of_storage;
    static void _Destroy(CBlendEntry* first, CBlendEntry* last)
    {
        for (; first != last; ++first)
            first->~CBlendEntry();
    }
    __forceinline ~CBlendEntryVector()
    {
        _Destroy(_M_start, _M_finish);
        if (_M_start)
            free(_M_start);
    }
};

struct CDeletable { long m_unused; };
struct SCountedBlock {
    long        m_count;
    void (CDeletable::*m_pDestroy)();
    CDeletable* m_pObject;
};

struct CCountedPointerData {
    void*          Data;
    SCountedBlock* Info;
    __forceinline ~CCountedPointerData()
    {
        if (Info) {
            --Info->m_count;
            if (Info->m_count == 0) {
                (Info->m_pObject->*Info->m_pDestroy)();
                delete Info;
            }
        }
        Data = 0;
        Info = 0;
    }
};

namespace C3DAnimationBlendState {
struct __declspec(novtable) CSharedData : CIVCountedPointeeBase {       // vtable 0x0129D1F8
    CIVPointer          m_pOwner;                  // +0x08
    CBlendEntryVector   m_entries;                 // +0x0c
    CCountedPointerData m_pData;                   // +0x18
    virtual ~CSharedData();
};
}

C3DAnimationBlendState::CSharedData::~CSharedData()
{
}