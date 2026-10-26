// CEngineManager::~CEngineManager() -- free the changed-area vector, destroy the engine list, unwind the singleton base
// Leaf: ??1CEngineManager@@UAE@XZ
// Retail: Fable.exe @ 0x00437040, 35 bytes. Catalog label CTCInventoryItem::OnDie is wrong: the closing vptr store is
// 0x0123156C, the RTTI vtable of CTBaseSingleton<CEngineManager>, and the only caller 0x00437020 is the matching
// deleting destructor (this call, then operator delete 0x00BFE9BC when bit 0 is set). Body: `if (_M_start) free()` for
// ChangedAreas (+0x08, malloc allocator, free = 0x00BFEA14), the out-of-line STLport list<CIEngine*> dtor 0x00436900 on
// Engines (+0x04: frees each node, relinks the sentinel, frees it), then the base vptr store.
// Layout: 0x14 bytes = Ego_r.pdb CEngineManager (32) with STLport containers: the 4-byte list (node pointer) at +0x04
// moves ChangedAreas from +0x10 to +0x08. engine/ headers do not model the polymorphic singleton base, so it is local.
// Shape: CEngineManager's destructor is implicit (no derived vptr store); CTBaseSingleton's is user-declared and empty.
// The qualified call in DestroyEngineManager makes VC7.1 emit the implicit destructor out of line.
#include <stdlib.h>
class CIEngine;
template <class T> class CTBaseSingleton {
public:
    virtual ~CTBaseSingleton() {}
};
template <class T> class list {                          // STLport list<T>: one pointer to the sentinel node
public:
    ~list();                                             // 0x00436900
    void* _M_node;
};
template <class T> class CMallocVector {                 // STLport vector<T> over the malloc allocator
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    ~CMallocVector() { if (_M_start) free(_M_start); }
};
class C2DBoxI { public: long Left, Top, Right, Bottom; };
class CEngineManager : public CTBaseSingleton<CEngineManager> {
public:
    list<CIEngine*>        Engines;                      // +0x04
    CMallocVector<C2DBoxI> ChangedAreas;                 // +0x08
};
void DestroyEngineManager(CEngineManager* p)             // emission helper, not a retail function
{
    p->CEngineManager::~CEngineManager();
}