// CEngineStaticMapFileBlockManager::~CEngineStaticMapFileBlockManager() -- destructor of the static-map file-block manager: clear and free the Blocks map's tree (releasing each block's LoadingData), then release AsyncBankEntry
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B443B0, 106 bytes (+6 int3), __fastcall, no arguments.
// Leaf: ??1CEngineStaticMapFileBlockManager@@QAE@XZ
// Identity: Ego_r.pdb / FableWin.pdb name sets ??1CEngineStaticMapFileBlockManager@@QAE@XZ; struct_layouts_egor
//   CEngineStaticMapFileBlockManager {CCountedPointer<CBankFileAsyncEntry> AsyncBankEntry +0, map<long, CFileBlock> Blocks
//   +8} and CEngineMap::FileBlockManager at +0x44 (68): the only caller is the CEngineMap destructor 0x00B42A90, which runs
//   CEngineStaticMapFileBlockManager::Reset 0x00BE05D0 and then this on its member at +0x44. The ctor is the landed
//   CEngineStaticMapFileBlockManager::CEngineStaticMapFileBlockManager 0x00B44370. Map value = pair<const long, CFileBlock>
//   with CFileBlock {RefCount +0, LowPriority +4, CCountedPointer<CBankFileAsyncData> LoadingData +8} (Ego_r), i.e. the
//   counted pointer at node+0x1C. The catalog names CCountedResourceMap / CBankFileEntryUpdateData were descriptive /
//   propagated.
// Callees: the tree's recursive _M_erase 0x00B441F0 (releases each node's LoadingData at node+0x1C/+0x20 and frees the
// node), free 0x00BFEA14 (the head node), the count block's destroy function (`call [blk+4]`, the object in ecx:
// __fastcall), operator delete 0x00BFE9BC (the count block).
// Layout (by use): a CCountedPointer {Data, Info} at +0; the STLport map {header, node count} at +8 (node: colour, parent
// +4, left +8, right +0x0C, then the value).
// Shape: members destroyed in reverse order, both destructors inline. The map destructor (a non-empty map is
// cleared: _M_erase(root), then left/parent/right of the head reset and the size zeroed -- the head re-read
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

struct SResourceNode {                              // STLport _Rb_tree_node<pair<const long, CFileBlock> >
    unsigned char m_colour;                         // +0x00
    SResourceNode* m_pParent;                       // +0x04
    SResourceNode* m_pLeft;                         // +0x08
    SResourceNode* m_pRight;                        // +0x0C
    unsigned char m_key[0x0C];                      // +0x10 key (long), CFileBlock RefCount / LowPriority
    CCountedPointer<void> m_value;                  // +0x1C CFileBlock::LoadingData
};

struct CResourceTree {                              // map<long, CFileBlock>: {header, node count}
    SResourceNode* m_pHead;                         // +0
    unsigned long m_size;                           // +4
    void _Erase(SResourceNode* root);               // 0x00B441F0 (_M_erase)
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

class CBankFileAsyncEntry;

class CEngineStaticMapFileBlockManager {
public:
    CCountedPointer<CBankFileAsyncEntry> AsyncBankEntry;   // +0x00
    CResourceTree Blocks;                           // +0x08
    ~CEngineStaticMapFileBlockManager();
};

CEngineStaticMapFileBlockManager::~CEngineStaticMapFileBlockManager()
{
}