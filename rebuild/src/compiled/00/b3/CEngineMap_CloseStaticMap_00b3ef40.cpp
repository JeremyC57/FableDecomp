// CEngineMap::CloseStaticMap() -- when a static map is open (+0x20): clear the scene at +0 and the detail cache at +4 (each when present), release the counted static-map data at +0x40, then zero +0x40, +0x3c and +0x20
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B3EF40, length 91 bytes.
// Callees: the scene's Clear @ 0x00BDC4F0 and the detail cache's CloseStaticMap @ 0x00BDDD50 (non-virtual, each behind a null test), the counted block's destroy
// function at +4 (a pointer to member) with the item at +8 as `this`, operator delete @ 0x00BFE9BC. Shape: the counted-pointer release is the inline CCountedPointer idiom
// (`dec [eax] ; cmp [eax],0 ; jne ; call [eax+4] ; push eax ; call delete`), each read of the block re-loads +0x40; the three zero stores are immediates.
void __cdecl operator delete(void* block);         // 0x00BFE9BC

struct CEngineScene_00bdc4f0 { void Clear(); };                   // 0x00BDC4F0
class CLocalDetailCacheMap { public: void CloseStaticMap(); };          // 0x00BDDD50

struct CCountedItem_00b3ef40 { };                  // the item: destroyed through the block's member-function pointer (`mov ecx,[eax+8] ; call [eax+4]`)

struct SCountedBlock_00b3ef40 {                    // the counted-pointer control block: count, destroy function, item
    long  m_count;                                 // +0x00
    void (CCountedItem_00b3ef40::*m_pfnDestroy)(); // +0x04 (a 4-byte pointer to member: called with the item as `this`)
    CCountedItem_00b3ef40* m_pItem;                // +0x08
};

class CEngineMap { public:
    CEngineScene_00bdc4f0* m_pScene;               // +0x00
    CLocalDetailCacheMap*  m_pDetailCache;         // +0x04
    unsigned char          _pad08[0x18];
    void*                  m_pStaticMap;           // +0x20
    unsigned char          _pad24[0x18];
    void*                  m_pStaticMapItem;       // +0x3c
    SCountedBlock_00b3ef40* m_pStaticMapBlock;     // +0x40
    void CloseStaticMap();
};

void CEngineMap::CloseStaticMap()
{
    if (m_pStaticMap) {
        if (m_pScene)
            m_pScene->Clear();
        if (m_pDetailCache)
            m_pDetailCache->CloseStaticMap();
        if (m_pStaticMapBlock) {
            --m_pStaticMapBlock->m_count;
            if (m_pStaticMapBlock->m_count == 0) {
                (m_pStaticMapBlock->m_pItem->*m_pStaticMapBlock->m_pfnDestroy)();
                operator delete(m_pStaticMapBlock);
            }
        }
        m_pStaticMapBlock = 0;
        m_pStaticMapItem = 0;
        m_pStaticMap = 0;
    }
}