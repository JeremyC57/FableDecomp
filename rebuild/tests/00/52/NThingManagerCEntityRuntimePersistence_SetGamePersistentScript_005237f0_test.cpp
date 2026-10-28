// Behaviour test for 0x005237F0 CEntityRuntimePersistence::SetGamePersistentScript: the game table grows to index + 1
// when short (never shrinks), the script lands in that slot and the slot is marked valid; the level table is untouched.
#include <cstdio>
#include <cstring>

static int g_assigns;
class CCharString {
public:
    int Id;
    const CCharString& operator=(const CCharString& other) { Id = other.Id; ++g_assigns; return *this; }
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
void NThingManager::CEntityRuntimePersistence::SetGamePersistentScript(long index, const CCharString& script)
{
    GamePersistentEntityScripts.resize(MAX((unsigned int)(index + 1), GamePersistentEntityScripts.size()));
    GamePersistentEntityScripts[index].Script = script;
    GamePersistentEntityScripts[index].Valid = true;
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
    CCharString s; s.Id = 5;
    p.SetGamePersistentScript(4, s);
    int ok = p.GamePersistentEntityScripts.LastResize == 5 && p.GamePersistentEntityScripts.size() == 5;
    ok &= g_game[4].Script.Id == 5 && g_game[4].Valid && !g_game[3].Valid;
    s.Id = 6;
    p.SetGamePersistentScript(1, s);
    ok &= p.GamePersistentEntityScripts.LastResize == 5 && g_game[1].Script.Id == 6 && g_game[1].Valid;
    ok &= p.LevelPersistentEntityScripts.size() == 0 && g_assigns == 2;
    if (!ok) { std::printf("005237f0_TEST FAIL\n"); return 1; }
    std::printf("005237f0_TEST PASS\n");
    return 0;
}