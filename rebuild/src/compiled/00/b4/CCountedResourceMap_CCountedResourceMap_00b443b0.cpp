// CCountedResourceMap::~CCountedResourceMap() -- destructor of an engine member pairing a counted pointer with a map of counted pointers: clear and free the map's tree, then release the counted pointer
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B443B0, 106 bytes (+6 int3), __fastcall, no arguments.
// Leaf: ??1CCountedResourceMap@@
// The catalog label (CBankFileEntryUpdateData::~CBankFileEntryUpdateData) is propagated: the only caller is the
// destructor at 0x00B42A90, which runs it on its member at +0x44 (after that member's clear 0x00BE05D0); the
// class is named by role.
// Callees: the tree's recursive _Erase 0x00B441F0 (releases each node's counted pointer at node+0x1C/+0x20 and
// frees the node), free 0x00BFEA14 (the head node), the count block's destroy function (`call [blk+4]`, the
// object in ecx: __fastcall), operator delete 0x00BFE9BC (the count block).
// Layout (by use): a CCountedPointer {object, count block} at +0; a VC6-era map {head, size} at +8 (node:
// colour, parent +4, left +8, right +0x0C, then the value).
// Shape: members destroyed in reverse order, both destructors inline. The map destructor (a non-empty map is
// cleared: _Erase(root), then left/parent/right of the head reset and the size zeroed -- the head re-read
// for each store -- then the head freed when non-null); the counted pointer's release re-reads the block
// after the decrement and before the delete, and both of its words are cleared at the end.
#include <stdlib.h>

void operator delete(void* p);                      // 0x00BFE9BC

struct SCountBlock {
    long m_references;                              // +0
    void (__fastcall* m_pfnDestroy)(void* object);  // +4
    void* m_pObject;                                // +8
};

template <class T>
struct CCountedPointer {
    T* m_pObject;                                   // +0
    SCountBlock* m_pCount;                          // +4
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

struct SResourceNode {                              // VC6-era tree node
    unsigned char m_colour;                         // +0x00
    SResourceNode* m_pParent;                       // +0x04
    SResourceNode* m_pLeft;                         // +0x08
    SResourceNode* m_pRight;                        // +0x0C
    unsigned char m_key[0x0C];                      // +0x10
    CCountedPointer<void> m_value;                  // +0x1C
};

struct CResourceTree {                              // map<key, CCountedPointer<..>>: {head, size}
    SResourceNode* m_pHead;                         // +0
    unsigned long m_size;                           // +4
    void _Erase(SResourceNode* root);               // 0x00B441F0
    ~CResourceTree()
    {
        if (m_size) {
            _Erase(m_pHead->m_pParent);
            m_pHead->m_pLeft = m_pHead;
            m_pHead->m_pParent = 0;
            m_pHead->m_pRight = m_pHead;
            m_size = 0;
        }
        if (m_pHead)
            free(m_pHead);
    }
};

class CResource;

struct CCountedResourceMap {
    CCountedPointer<CResource> m_owner;             // +0x00
    CResourceTree m_entries;                        // +0x08
    ~CCountedResourceMap();
};

CCountedResourceMap::~CCountedResourceMap()
{
}