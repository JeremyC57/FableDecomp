// CLocalDetailCacheMap::Update(double time) -- with LockedFromMemoryRelocation (+0) raised, the quad tree at +0xc is updated (CQuadTreeElement::Update 0x00BDEBF0 with (FileBlockManager +4, CollectionTypePalette +0x64, time)); when the file-block manager exists its unused blocks are deleted (DeleteUnusedBlocks 0x00BE04F0); the lock is dropped and the update's result returned
// Leaf: ?Update@CLocalDetailCacheMap@@QAE_NN@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BDED90, length 64 bytes.
// Identity: Ego_r.pdb ?Update@CLocalDetailCacheMap@@QAE_NN@Z; members from struct_layouts_egor CLocalDetailCacheMap
//   (LockedFromMemoryRelocation +0, FileBlockManager +4, Map +8, QuadTree +0xC, AreaUpdateList +0x58, then
//   CollectionTypePalette at retail +0x64: Ego_r +0x68 less 4 for the 12-byte retail vector, so engine/CLocalDetailCacheMap.h
//   (Ego_r offsets) cannot be used here).
// Callees: CQuadTreeElement::Update @ 0x00BDEBF0 (landed; Ego_r.pdb ?Update@CQuadTreeElement@CLocalDetailCacheMap@@QAE_NPAV
//   CEngineStaticMapFileBlockManager@@ABVCObjectTypeCollectionPalette@2@N@Z, declared as the landed 0x00BDEBF0 declares it),
//   CEngineStaticMapFileBlockManager::DeleteUnusedBlocks @ 0x00BE04F0 (landed; Ego_r.pdb
//   ?DeleteUnusedBlocks@CEngineStaticMapFileBlockManager@@QAEXXZ). Shape: the double re-pushed through the FPU, the result
//   kept in bl across the second call, `ret 8`.
class CEngineMap;
class CEngineStaticMapFileBlockManager { public:
    void DeleteUnusedBlocks();                     // 0x00BE04F0
};
struct CObjectTypeCollectionPalette { unsigned char Entries[0xc]; };   // CArray<const CLocalDetailObjectCollectionType*>
struct CQuadTreeElement {
    unsigned char _body[0x4c];
    bool Update(CEngineStaticMapFileBlockManager* blocks, const CObjectTypeCollectionPalette& palette, double deadline);   // 0x00BDEBF0
};

class CLocalDetailCacheMap { public:
    bool                              LockedFromMemoryRelocation; // +0x00
    CEngineStaticMapFileBlockManager* FileBlockManager;       // +0x04
    CEngineMap*                       Map;                    // +0x08
    CQuadTreeElement                  QuadTree;               // +0x0c
    unsigned char                     AreaUpdateList[0xc];    // +0x58 vector<C2DBoxI> (12-byte retail STLport vector)
    CObjectTypeCollectionPalette      CollectionTypePalette;  // +0x64
    bool Update(double time);
};

bool CLocalDetailCacheMap::Update(double time)
{
    LockedFromMemoryRelocation = true;
    bool updated = QuadTree.Update(FileBlockManager, CollectionTypePalette, time);
    if (FileBlockManager)
        FileBlockManager->DeleteUnusedBlocks();
    LockedFromMemoryRelocation = false;
    return updated;
}
