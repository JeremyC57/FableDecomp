// NThingMap::CThingMapManager::CThingMapManager() -- construct the thing-map manager: two empty arrays and an empty set of active game maps (SGI tree header allocated and self-linked)
// Leaf: ??0CThingMapManager
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005191E0, length 79 bytes.
// Callees: CBaseClassNonCopyable::CBaseClassNonCopyable @ 0x0099A2F0 (landed; vtable 0x0129A7C4), malloc
// @ 0x00BFEA0E. Vtable 0x01244678 = NThingMap::CThingMapManager (RTTI).
// Identity: the manifest label CPreallocTexturePool::CPreallocTexturePool is propagated; the vtable names the
// class and Ego_r.pdb has ??0CThingMapManager@NThingMap@@QAE@XZ.
// Layout (retail CArray 12 bytes): PWorld +4 (not touched), ThingMaps +8, ActiveThingMaps +0x14,
// ActiveGameMaps (std::set<long>: header pointer +0x20, node count +0x24).
// The engine header CThingMapManager.h carries the PDB widths (ActiveThingMaps +0x18, flattened vptr), so the
// class is modelled locally.
// Shape: the vtable store precedes the zero stores, so both arrays are member objects with inline
// constructors; the set is the SGI `_Rb_tree_base` (header = 0, then allocated) plus `_Rb_tree` (count = 0,
// header red, root 0, leftmost = rightmost = header, the header re-read after every store).
extern "C" void* __cdecl malloc(unsigned int size);         // 0x00BFEA0E

struct CBaseClassNonCopyable {                              // vtable 0x0129A7C4
    CBaseClassNonCopyable();                                // 0x0099A2F0
    virtual void v00();
};

template <class T> struct CArray {                          // retail CArray: three pointers (12 bytes)
    T* m_pFirst;
    T* m_pLast;
    T* m_pEnd;
    CArray() : m_pFirst(0), m_pLast(0), m_pEnd(0) { }
};

struct SLongSetNode {                                       // SGI _Rb_tree_node<long>, 0x14 bytes
    char          Color;                                    // +0x00 (0 = red)
    SLongSetNode* Parent;                                   // +0x04
    SLongSetNode* Left;                                     // +0x08
    SLongSetNode* Right;                                    // +0x0c
    long          Value;                                    // +0x10
};

struct CLongSetBase {                                       // SGI _Rb_tree_base: the header is allocated
    SLongSetNode* m_pHeader;
    CLongSetBase() : m_pHeader(0) { m_pHeader = (SLongSetNode*)malloc(sizeof(SLongSetNode)); }
};

struct CLongSet : CLongSetBase {                            // std::set<long>
    unsigned long m_nodeCount;
    CLongSet() : m_nodeCount(0)
    {
        m_pHeader->Color = 0;
        m_pHeader->Parent = 0;
        m_pHeader->Left = m_pHeader;
        m_pHeader->Right = m_pHeader;
    }
};

struct CWorld;

namespace NThingMap {

struct CThingMapManager : CBaseClassNonCopyable {           // vtable 0x01244678
    struct CThingMapInfo;
    CWorld*                 PWorld;                         // +0x04
    CArray<CThingMapInfo>   ThingMaps;                      // +0x08
    CArray<CThingMapInfo*>  ActiveThingMaps;                // +0x14
    CLongSet                ActiveGameMaps;                 // +0x20
    CThingMapManager();
    virtual void v00();
};

CThingMapManager::CThingMapManager()
{
}

}