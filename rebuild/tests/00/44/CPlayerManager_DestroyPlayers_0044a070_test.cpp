// Behaviour test for 0044a070: the model below is a copy of src/0044a070.cpp (compiled separately), driven by test doubles.
#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __fastcall
#define __stdcall
#define __cdecl
#define __fastcall
#define __forceinline inline
#define __int64 long long
#endif
template <class T> T* AddressOfTemporary(const T& t) { return const_cast<T*>(&t); }
#include <stddef.h>
extern "C" void* __cdecl memmove(void* dest, const void* src, size_t count);   // 0x00BFEAE6

extern long g_NoPlayerNumber_013760b0;                  // 0x013760B0

struct CPlayer {
    virtual ~CPlayer();                                 // slot 0 (scalar deleting destructor)
    unsigned char _pad04[0x28 - 0x04];
    long          Number;                               // +0x28
    void Uninitialise();                                // 0x00487AF0
};

inline void* __copy_trivial(const void* first, const void* last, void* result)
{
    return (last == first) ? result
        : ((char*)memmove(result, first, (const char*)last - (const char*)first)) + ((const char*)last - (const char*)first);
}

struct CPlayerVector {                                  // retail STLport vector<CPlayer*>, 12 bytes
    CPlayer** _M_start;
    CPlayer** _M_finish;
    CPlayer** _M_end_of_storage;
    CPlayer** begin() { return _M_start; }
    CPlayer** end() { return _M_finish; }
    unsigned int size() const { return _M_finish - _M_start; }
    CPlayer*& operator[](unsigned int i) { return _M_start[i]; }
    CPlayer** erase(CPlayer** first, CPlayer** last)
    {
        CPlayer** i = (CPlayer**)__copy_trivial(last, _M_finish, first);
        _M_finish = i;
        return first;
    }
    void clear() { erase(begin(), end()); }
};

struct CPlayerManager {
    void*         __vftable;
    void*         Component;                            // +0x04
    void*         DefinitionManager;                    // +0x08
    CPlayerVector Players;                              // +0x0c
    long          PlayerNeutral;                        // +0x18
    long          MainPlayer;                           // +0x1c
    __forceinline CPlayer* GetPlayer(long number)   // 0x004498C0 (inlined here)
    {
        long count = Players.size();
        for (long i = 0; i < count; ++i)
            if (Players[i]->Number == number)
                return Players[i];
        return Players[PlayerNeutral];
    }
    void DestroyPlayers();
};

void CPlayerManager::DestroyPlayers()
{
    for (unsigned int i = 0; i < Players.size(); ++i)
        GetPlayer(i)->Uninitialise();
    for (unsigned int j = 0; j < Players.size(); ++j)
        delete Players[j];
    Players.clear();
    PlayerNeutral = g_NoPlayerNumber_013760b0;
    MainPlayer = g_NoPlayerNumber_013760b0;
}

// ---- test doubles and driver ----
static int g_uninit[8]; static int g_nu = 0; static int g_deleted = 0;
long g_NoPlayerNumber_013760b0 = -1;
struct CTestPlayer : CPlayer { int id; ~CTestPlayer() { ++g_deleted; } };
CPlayer::~CPlayer() {}
void CPlayer::Uninitialise() { if (g_nu < 8) g_uninit[g_nu++] = static_cast<CTestPlayer*>(this)->id; }
int main()
{
    CPlayer* storage[4];
    CTestPlayer* a = new CTestPlayer; a->id = 10; a->Number = 1;     // player numbers 1, 0, 5 (5 has no slot match)
    CTestPlayer* b = new CTestPlayer; b->id = 20; b->Number = 0;
    CTestPlayer* c = new CTestPlayer; c->id = 30; c->Number = 5;
    storage[0] = a; storage[1] = b; storage[2] = c;
    static CPlayerManager pm;
    pm.Players._M_start = storage; pm.Players._M_finish = storage + 3; pm.Players._M_end_of_storage = storage + 4;
    pm.PlayerNeutral = 2; pm.MainPlayer = 1;
    pm.DestroyPlayers();
    bool ok = true;
    // i = 0 -> player numbered 0 (b), i = 1 -> a, i = 2 -> no number 2: the neutral player Players[2] (c)
    if (g_nu != 3 || g_uninit[0] != 20 || g_uninit[1] != 10 || g_uninit[2] != 30) ok = false;
    if (g_deleted != 3 || pm.Players._M_finish != storage || pm.Players._M_start != storage) ok = false;
    if (pm.PlayerNeutral != -1 || pm.MainPlayer != -1) ok = false;
    if (!ok) { std::printf("0044a070_TEST FAIL\n"); return 1; }
    std::printf("0044a070_TEST PASS\n");
    return 0;
}