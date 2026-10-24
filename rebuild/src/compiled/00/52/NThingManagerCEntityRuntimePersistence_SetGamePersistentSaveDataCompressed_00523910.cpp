// NThingManager::CEntityRuntimePersistence::SetGamePersistentSaveDataCompressed(long index, const void* data, unsigned long size, unsigned long uncompressed_size) -- grow the game-persistent table to cover index and copy an already-compressed save block into that slot
// Leaf: ?SetGamePersistentSaveDataCompressed@CEntityRuntimePersistence@NThingManager@@
//
// Retail 0x00523910, 163 bytes (then int3 padding), `ret 10h` (thiscall).
// Callees: 0x0052C6A0 vector<CEntityScriptInfo>::resize(size_type), 0x00486189 vector<unsigned char>::resize(n, const
//   unsigned char&); memcpy is the /Oi intrinsic (rep movsd + rep movsb).
// Callers: 0x00524EC0 (catalog "CDrawQuestInfo_LoadDeadCreatures": the save loader handing back stored blocks).
// Identity: Ego_r.pdb names NThingManager::CEntityRuntimePersistence::{SetLevelPersistentScript, SetGamePersistentScript,
//   SetGamePersistentSaveDataCompressed} (name set only); the class is CThingManager::EntityRuntimePersistence. Its
//   CArray<CEntityScriptInfo> LevelPersistentEntityScripts (+0) / GamePersistentEntityScripts (+0xC in retail, +0x10 in
//   the Dinkumware PDB) hold 24-byte records {CCharString Script; CArray<uchar> SaveData; long UncompressedSize;
//   bool Valid} (the 0x2AAAAAAB / `sar edx,2` divide by 24). The landed 0x00523860 SetGamePersistentSaveData uses the
//   same record model.
// Shape: `resize(MAX(index + 1, size()))` (two divides, as 0x005237F0), a reference to the record (ebx), the
//   uncompressed size stored first, then SaveData.resize(size) -- STLport's inline resize(n) passing a zero
//   unsigned-char temporary that VC homes in the dead `index` argument slot -- then memcpy into SaveData.data()
//   (the engine's `size() ? first : (T*)this` fallback, as in the landed 0x00523860), then Valid = true.
// Label: catalog "CThingDeadCreature_SetAnimationData" is wrong.
extern "C" void* __cdecl memcpy(void* dst, const void* src, unsigned int size);

class CCharString {
public:
    const char* PString;
    CCharString& operator=(const CCharString& other);       // 0x0099EFB0
};

// STLport vector view (start, finish, end of storage); CArray<T> derives from it.
template <class T> class CArray {
public:
    T* _M_start;                                            // +0x0
    T* _M_finish;                                           // +0x4
    T* _M_end_of_storage;                                   // +0x8
    unsigned int size() const { return (unsigned int)(this->_M_finish - this->_M_start); }
    T& operator[](unsigned int n) { return *(this->_M_start + n); }
    T* data() { return size() ? this->_M_start : (T*)this; }   // the engine's empty-array buffer fallback
    void resize(unsigned int new_size, const T& x);         // vector<unsigned char>: 0x00486189
    void resize(unsigned int new_size) { resize(new_size, T()); }
};

namespace NThingManager {
class CEntityRuntimePersistence {
public:
    class CEntityScriptInfo {                               // 0x18 bytes in retail (STLport's 12-byte vector)
    public:
        CCharString           Script;                       // +0x00
        CArray<unsigned char> SaveData;                     // +0x04
        long                  UncompressedSize;             // +0x10
        bool                  Valid;                        // +0x14
    };
    class CEntityScriptArray : public CArray<CEntityScriptInfo> {
    public:
        void resize(unsigned int new_size);                 // 0x0052C6A0 (STLport resize(n) -> resize(n, T()))
    };
    CEntityScriptArray LevelPersistentEntityScripts;        // +0x00
    CEntityScriptArray GamePersistentEntityScripts;         // +0x0C
    // PersistedDeadEntities map +0x18, SaveFormatVersion +0x28 (unused here)

    void SetLevelPersistentScript(long index, const CCharString& script);
    void SetGamePersistentScript(long index, const CCharString& script);
    void SetGamePersistentSaveDataCompressed(long index, const void* data, unsigned long size,
                                             unsigned long uncompressed_size);
};
}
#define MAX(a, b) ((a) > (b) ? (a) : (b))
void NThingManager::CEntityRuntimePersistence::SetGamePersistentSaveDataCompressed(long index, const void* data,
                                                                                  unsigned long size,
                                                                                  unsigned long uncompressed_size)
{
    GamePersistentEntityScripts.resize(MAX((unsigned int)(index + 1), GamePersistentEntityScripts.size()));
    CEntityScriptInfo& info = GamePersistentEntityScripts[index];
    info.UncompressedSize = uncompressed_size;
    info.SaveData.resize(size);
    memcpy(info.SaveData.data(), data, size);
    info.Valid = true;
}