// Behaviour test for 0x00523790 CEntityRuntimePersistence::SetLevelPersistentScript: the level table grows to
// index + 1 when short (never shrinks), the script lands in that slot and the slot is marked valid.
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
void NThingManager::CEntityRuntimePersistence::SetLevelPersistentScript(long index, const CCharString& script)
{
    const unsigned int count = LevelPersistentEntityScripts.size();
    LevelPersistentEntityScripts.resize((unsigned int)(index + 1) > count ? index + 1 : count);
    LevelPersistentEntityScripts[index].Script = script;
    LevelPersistentEntityScripts[index].Valid = true;
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
    CCharString s; s.Id = 7;
    p.SetLevelPersistentScript(2, s);
    int ok = p.LevelPersistentEntityScripts.LastResize == 3 && p.LevelPersistentEntityScripts.size() == 3;
    ok &= g_lvl[2].Script.Id == 7 && g_lvl[2].Valid && !g_lvl[0].Valid && !g_lvl[1].Valid && g_assigns == 1;
    s.Id = 9;
    p.SetLevelPersistentScript(0, s);
    ok &= p.LevelPersistentEntityScripts.LastResize == 3 && p.LevelPersistentEntityScripts.size() == 3;
    ok &= g_lvl[0].Script.Id == 9 && g_lvl[0].Valid && g_lvl[2].Script.Id == 7 && p.GamePersistentEntityScripts.size() == 0;
    if (!ok) { std::printf("00523790_TEST FAIL\n"); return 1; }
    std::printf("00523790_TEST PASS\n");
    return 0;
}