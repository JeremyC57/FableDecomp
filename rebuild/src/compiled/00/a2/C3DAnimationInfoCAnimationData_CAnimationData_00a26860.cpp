// C3DAnimationInfo::CAnimationData::~CAnimationData() -- destructor: clear the owning info pointer, release the file data and shared animation, clear and free the per-ID animation map, then run the base destructor
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A26860, length 160 bytes.
// Callees: operator delete @ 0x00BFE9BC (count blocks), the STLport tree's recursive _M_erase(root) @ 0x00A2B9D0 (on the map), free @ 0x00BFEA14
// (the map's header node), the base destructor @ 0x009FC550 (tail jump). Vtable 0x0129CE50 (RTTI C3DAnimationInfo::CAnimationData; its slot 0
// is the deleting destructor 0x00A2DB10, which calls this body).
// Label correction: the manifest's "CEditWorld::~CEditWorld" is a propagated name. Layout = PDB CAnimationData with the 16-byte PDB map
// shrunk to the retail 12-byte STLport map: Animations (map<unsigned long, CCountedPointer<C3DAnimation2>>) +0x1c, SharedAnimation +0x28,
// FileData +0x30, AnimInfo +0x38; the base owns the vptr and +0x04..+0x1b.
// Shape: vtable store, then `AnimInfo = 0` (the body), then the members in reverse order with the ebx zero register: two counted pointers
// (`if (block) { --count; if (count == 0) { deleter(object); delete block; } } object = 0; block = 0;`), the inlined STLport map destructor
// (`clear()`: when the count is non-zero, `_M_erase(root)` then leftmost = header, root = 0, rightmost = header, count = 0; then the
// header node freed when non-null) and the base destructor as a tail jump. The deleting destructor is not emitted (non-virtual destructor
// in the model, the vtable comes from a virtual filler).
// Leaf: ??1CAnimationData
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8). Also: the local CCountedPointer members m_pObject->Data, m_pCount->Info are the header CCountedPointer's (same offsets); the local SCountBlock (= CCPPointerInfo) members m_references->RefCount, m_pfnDestroy->DeleteFunc, m_pObject->Data are the header CCPPointerInfo's (same offsets).
extern "C" void __cdecl free(void* block);   // 0x00BFEA14

#include "engine/CCountedPointer.h"



struct SRbTreeNode {                         // STLport _Rb_tree_node_base (+ value)
    unsigned char colour;
    SRbTreeNode*  parent;
    SRbTreeNode*  left;
    SRbTreeNode*  right;
};

struct CAnimationMap {                       // STLport map<unsigned long, CCountedPointer<C3DAnimation2>>: header, count, comparator
    SRbTreeNode*  m_header;                  // +0x00
    unsigned long m_nodeCount;               // +0x04
    unsigned char m_keyCompare;              // +0x08
    void Erase(SRbTreeNode* node);           // 0x00A2B9D0 (_M_erase)
    void clear()
    {
        if (m_nodeCount != 0) {
            Erase(m_header->parent);
            m_header->left = m_header;
            m_header->parent = 0;
            m_header->right = m_header;
            m_nodeCount = 0;
        }
    }
    ~CAnimationMap()
    {
        clear();
        if (m_header)
            free(m_header);
    }
};

class C3DAnimation2;
class CBankFileAsyncData;
class C3DAnimationInfo;

class CResource {                   // the base class: vptr + 0x18 bytes, out-of-line destructor
public:
    virtual void v00();
    virtual ~CResource();                   // 0x009FC550
    unsigned char _body[0x18];
};

class C3DAnimationInfo {
public:
    class CAnimationData : public CResource {   // vtable 0x0129CE50
    public:
        virtual void v00();
        CAnimationMap                       Animations;        // +0x1c
        CCountedPointer<C3DAnimation2>      SharedAnimation;   // +0x28
        CCountedPointer<CBankFileAsyncData> FileData;          // +0x30
        C3DAnimationInfo*                   AnimInfo;          // +0x38
        ~CAnimationData();
    };
};

C3DAnimationInfo::CAnimationData::~CAnimationData()
{
    AnimInfo = 0;
}