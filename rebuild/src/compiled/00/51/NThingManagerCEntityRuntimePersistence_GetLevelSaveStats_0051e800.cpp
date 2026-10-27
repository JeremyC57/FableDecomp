// NThingManager::CEntityRuntimePersistence::GetLevelSaveStats(unsigned long& noSaves, unsigned long& totalSize, unsigned long& totalUncompressedSize) const -- counts the valid persisted entity-script saves and sums their stored and uncompressed sizes
// Leaf: ?GetLevelSaveStats@CEntityRuntimePersistence@NThingManager@@QBEXAAK00@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0051E800, 114 bytes, thiscall `ret 0Ch`.
// Identity (proven): Ego_r.pdb names CEntityRuntimePersistence::GetLevelSaveStats(unsigned long&, unsigned long&,
//   unsigned long&) const, the class's only three-reference method. Its only caller 0x0051E900 is the
//   CThingManager::ShowLevelSaveStats console command (`<n> binary level saves take up <size> (<uncompressed>
//   uncompressed)`, strings 0x01244E4C / 0x01244E6C / 0x01244E70), which passes &thingManager->EntityRuntimePersistence
//   (CThingManager +0xA0, landed ctor model 0x00523540).
// Layout (retail STLport; Ego_r.pdb names): CEntityRuntimePersistence { CArray<CEntityScriptInfo>
//   LevelPersistentEntityScripts +0x0; CArray<CEntityScriptInfo> GamePersistentEntityScripts +0xC; ... }, and
//   CEntityScriptInfo (24 bytes, the 0x2AAAAAAB / sar 2 stride) { CCharString Script +0; CArray<unsigned char>
//   SaveData +4; long UncompressedSize +0x10; bool Valid +0x14 }. The walk is over GamePersistentEntityScripts (+0xC).
// Shape: the three results are zeroed first (in argument order), the element count is computed once (signed `jle`),
//   the array base is reloaded every pass (the stores through the references may alias it).
class CCharString {
public:
    void* PStringData;
};

namespace std {
template <class T> class allocator { };
template <class T, class A = allocator<T> > class vector {
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
};
}
template <class T> class CArray : public std::vector<T, std::allocator<T> > {
public:
    long Size() const { return this->_M_finish - this->_M_start; }
    const T& operator[](long i) const { return this->_M_start[i]; }
};

namespace NThingManager {
class CEntityRuntimePersistence {
public:
    class CEntityScriptInfo {                                   // 24 bytes (retail)
    public:
        CCharString            Script;                          // +0x00
        CArray<unsigned char>  SaveData;                        // +0x04
        long                   UncompressedSize;                // +0x10
        bool                   Valid;                           // +0x14
    };
    CArray<CEntityScriptInfo> LevelPersistentEntityScripts;     // +0x00
    CArray<CEntityScriptInfo> GamePersistentEntityScripts;      // +0x0C
    void GetLevelSaveStats(unsigned long& noSaves, unsigned long& totalSize, unsigned long& totalUncompressedSize) const;
};

void CEntityRuntimePersistence::GetLevelSaveStats(unsigned long& noSaves, unsigned long& totalSize,
                                                  unsigned long& totalUncompressedSize) const
{
    noSaves = 0;
    totalSize = 0;
    totalUncompressedSize = 0;
    long noScripts = GamePersistentEntityScripts.Size();
    for (long i = 0; i < noScripts; ++i)
    {
        const CEntityScriptInfo& info = GamePersistentEntityScripts[i];
        if (info.Valid)
        {
            ++noSaves;
            totalSize += info.SaveData.Size();
            totalUncompressedSize += info.UncompressedSize;
        }
    }
}
}