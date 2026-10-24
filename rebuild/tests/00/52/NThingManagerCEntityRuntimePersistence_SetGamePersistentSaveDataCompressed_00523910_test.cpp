// Behaviour test for 0x00523910 CEntityRuntimePersistence::SetGamePersistentSaveDataCompressed: grows the game table to
// cover index, records the uncompressed size, sizes the slot's SaveData to the block and copies it, marks the slot valid.
#include <cstdio>
#include <cstring>

static int g_assigns;
class CCharString {
public:
    int Id;
    CCharString& operator=(const CCharString& other) { Id = other.Id; ++g_assigns; return *this; }
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
    void resize(unsigned int new_size, const T& x)          // test body: grow inside a reserved buffer, filling with x
    {
        T* old_finish = this->_M_finish;
        this->_M_finish = this->_M_start + new_size;
        for (T* p = old_finish; p < this->_M_finish; ++p) *p = x;
    }
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
        unsigned int LastResize;
        void resize(unsigned int new_size)                  // test body: records live in a reserved buffer
        {
            LastResize = new_size;
            if (new_size > size()) { CEntityScriptInfo* p = this->_M_finish; this->_M_finish = this->_M_start + new_size;
                for (; p != this->_M_finish; ++p) { p->Script.Id = 0; p->Valid = false; p->UncompressedSize = 0; } }
            else this->_M_finish = this->_M_start + new_size;
        }
    };
    CEntityScriptArray LevelPersistentEntityScripts;        // +0x00
    CEntityScriptArray GamePersistentEntityScripts;
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
typedef NThingManager::CEntityRuntimePersistence P;
typedef P::CEntityScriptInfo R;
static R g_lvl[8], g_game[8];
static unsigned char g_bufs[8][32];
static void setup(P& p)
{
    p.LevelPersistentEntityScripts._M_start = p.LevelPersistentEntityScripts._M_finish = g_lvl;
    p.LevelPersistentEntityScripts._M_end_of_storage = g_lvl + 8;
    p.GamePersistentEntityScripts._M_start = p.GamePersistentEntityScripts._M_finish = g_game;
    p.GamePersistentEntityScripts._M_end_of_storage = g_game + 8;
    for (int i = 0; i < 8; ++i) {
        g_game[i].SaveData._M_start = g_game[i].SaveData._M_finish = g_bufs[i];
        g_game[i].SaveData._M_end_of_storage = g_bufs[i] + 32;
    }
}
int main()
{
    P p; setup(p);
    const unsigned char blk[7] = { 1, 2, 3, 4, 5, 6, 7 };
    p.SetGamePersistentSaveDataCompressed(1, blk, 7, 100);
    int ok = p.GamePersistentEntityScripts.LastResize == 2 && p.GamePersistentEntityScripts.size() == 2;
    ok &= g_game[1].UncompressedSize == 100 && g_game[1].Valid && g_game[1].SaveData.size() == 7;
    ok &= std::memcmp(g_bufs[1], blk, 7) == 0 && !g_game[0].Valid && g_assigns == 0;
    p.SetGamePersistentSaveDataCompressed(0, blk + 4, 3, 9);
    ok &= p.GamePersistentEntityScripts.LastResize == 2 && g_game[0].SaveData.size() == 3 && g_bufs[0][0] == 5;
    ok &= g_bufs[0][2] == 7 && g_game[0].UncompressedSize == 9 && g_game[0].Valid;
    if (!ok) { std::printf("00523910_TEST FAIL\n"); return 1; }
    std::printf("00523910_TEST PASS\n");
    return 0;
}