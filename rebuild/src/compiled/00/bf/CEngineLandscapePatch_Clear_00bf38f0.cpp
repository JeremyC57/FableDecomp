// CEngineLandscapePatch::Clear() -- drops the patch's texture-owner chain blocks, deletes its child object, frees its vertex block through the landscape memory manager, releases its counted resource and deletes its second owned object
// Retry 10b: the +0x14 object is deleted through a by-reference helper (`SafeDeleteRef(m_pResource)` = `delete p; p = 0;` on a T*&) inside the `if`: the reference keeps delete's own null test (`mov ecx,eax ; test ecx,ecx`) that a direct `delete m_pResource` folds; a reference local does the same.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BF38F0, length 132 bytes (144 with padding).
// Callees: CTextureOwner::ClearBlock @ 0x00BFDD40 (landed; manifest "CLandscapeBackgroundPatch::ClearTextures"),
// the owned objects' virtual scalar deleting destructors (vtable slot 0, `push 1`),
// CEngineLandscapeMemoryManager::FreeVirtual @ 0x00BD9B90 (the manager at engine 0x01436EA8 -> +0x6B0),
// the counted-resource member's DiscardAsyncData-shaped release @ 0x009D68A0 on +0x40 (`lea ecx,[esi+40h]`).
// Shape: a 3-bit state field in the byte at +0x3C (bit 1 = "textures attached"): when set, the chain of
// texture owners hung at +8 (linked through their +0x3C word) is cleared and the state reset; the state is
// reset again after the owner at +8 is deleted. `delete` on a known non-null member drops VC's own null
// test; the second owned pointer (+0x14) is deleted through the reference helper, which keeps it (`mov ecx,eax ;
// test ecx,ecx`). The patch has no PDB header; it is modelled locally (the landed destructor @ 0x00BF3A40
// shows the CMovableResource vptr at +0 and the counted pointer at +0x40).
class CEngineLandscapeMemoryManager { public:
    void FreeVirtual(void* block);                 // 0x00BD9B90
};
struct SEngine_01436ea8 {
    unsigned char                  _pad[0x6b0];
    CEngineLandscapeMemoryManager* m_pLandscapeMemory;   // +0x6b0
};
extern SEngine_01436ea8* g_pEngine_01436ea8;      // 0x01436EA8

struct CTextureOwner {                             // the chain element (0x40+ bytes)
    virtual ~CTextureOwner();                      // vtable slot 0
    unsigned char  _pad04[0x38];
    CTextureOwner* m_pNext;                        // +0x3C
    void ClearBlock();                             // 0x00BFDD40
};

struct CCountedResourcePointer {
    void* m_pObject;                               // +0x00
    void* m_pBlock;                                // +0x04
    void Discard();                                // 0x009D68A0
};

struct COwnedResource {
    virtual ~COwnedResource();                     // vtable slot 0
};

template <class T> inline void SafeDeleteRef(T*& p) { delete p; p = 0; }
class CEngineLandscapePatch { public:
    virtual ~CEngineLandscapePatch();              // +0x00 vptr (CMovableResource)
    unsigned char           _pad04[0x4];
    CTextureOwner*          m_pTextureOwners;      // +0x08
    void*                   m_pVertexBlock;        // +0x0C
    unsigned char           _pad10[0x4];
    COwnedResource*         m_pResource;           // +0x14
    unsigned char           _pad18[0x24];
    unsigned char           m_state : 3;           // +0x3C
    CCountedResourcePointer m_counted;             // +0x40
    void Clear();
};

void CEngineLandscapePatch::Clear()
{
    if (m_state & 2) {
        for (CTextureOwner* owner = m_pTextureOwners; owner; owner = owner->m_pNext)
            owner->ClearBlock();
        m_state = 0;
    }
    if (m_pTextureOwners) {
        delete m_pTextureOwners;
        m_pTextureOwners = 0;
        m_state = 0;
    }
    if (m_pVertexBlock) {
        g_pEngine_01436ea8->m_pLandscapeMemory->FreeVirtual(m_pVertexBlock);
        m_pVertexBlock = 0;
    }
    m_counted.Discard();
    if (m_pResource)
        SafeDeleteRef(m_pResource);
}