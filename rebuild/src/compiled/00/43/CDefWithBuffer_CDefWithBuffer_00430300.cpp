// CDefWithBuffer::~CDefWithBuffer() -- frees the definition's malloc-backed array, then unwinds CDefPointeeBase/CResource
// Leaf: ??1CDefWithBuffer@@UAE@XZ
// Retail: Fable.exe @ 0x00430300, 33 bytes. Catalog label CTCInventoryItem::OnDie is wrong: every caller (30+, e.g.
// 0x00431DD3, 0x00455899) is a definition class's deleting destructor, so this is a destructor shared (identical-code
// folded) by several CDefClassBase-derived definitions. Body: `if (data) free(data)` for the STLport vector at +0x29
// (malloc allocator; free = 0x00BFEA14), then the inline ~CDefPointeeBase stores its vptr (0x01230BA0) and tail-jumps to
// the out-of-line ~CResource (0x009FC550). The derived and CDefClassBase levels store no vptr (implicit destructors).
// Layout: Ego_r.pdb CDefClassBase is 37 bytes, packed (CResource 28: vptr, IVRefCount, ResourceList, Prev, Next, Size,
// LastUsedFrame; then PDefManager +0x1c, GlobalIndex +0x20, flag bits +0x24); the folded definitions continue with a dword
// at +0x25 and the vector at +0x29. The class name CDefWithBuffer is behavioural (several real classes share the body).
// Shape: the destructor is the implicit one (a user-declared ~CDefWithBuffer stores the derived vptr first); the qualified
// call in DestroyDefWithBuffer makes VC7.1 emit it out of line, as the retail deleting destructors call it.
#include <stdlib.h>
#pragma pack(push, 1)
class CResourceList;
class CResource {
public:
    unsigned long  IVRefCount;                           // +0x04
    CResourceList* ResourceList;                         // +0x08
    CResource*     PrevResource;                         // +0x0c
    CResource*     NextResource;                         // +0x10
    unsigned long  ResourceSize;                         // +0x14
    unsigned long  LastUsedFrame;                        // +0x18
    virtual ~CResource();                                // 0x009FC550
};
class CDefPointeeBase : public CResource {
public:
    virtual ~CDefPointeeBase() {}
};
class CDefinitionManager;
class CDefClassBase : public CDefPointeeBase {
public:
    CDefinitionManager* PDefManager;                     // +0x1c
    unsigned long       GlobalIndex;                     // +0x20
    unsigned char       Setup : 1;                       // +0x24
    unsigned char       Template : 1;
    unsigned char       DefaultValsApplied : 1;
};
template <class T> class CMallocVector {                 // STLport vector<T> over the malloc allocator
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    ~CMallocVector() { if (_M_start) free(_M_start); }
};
class CDefWithBuffer : public CDefClassBase {
public:
    long                Value;                           // +0x25
    CMallocVector<long> Data;                            // +0x29
};
#pragma pack(pop)
void DestroyDefWithBuffer(CDefWithBuffer* p)                 // emission helper, not a retail function
{
    p->CDefWithBuffer::~CDefWithBuffer();
}