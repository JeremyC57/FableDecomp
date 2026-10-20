// CPlayerManager::DestroyPlayers() -- uninitialise player 0..n-1 (each looked up by number, the neutral player standing in for a missing one), delete every player, empty the player vector and reset the neutral/main player numbers
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0044A070, length 202 bytes (+ int3 padding), plain `ret`.
// Identity: Ego_r.pdb `?DestroyPlayers@CPlayerManager@@AAEXXZ`.
// Callees: CPlayer::Uninitialise 0x00487AF0, the player's virtual scalar deleting destructor (slot 0, `push 1`),
// memmove 0x00BFEAE6 (STLport __copy_trivial inside vector::erase), the player-number global 0x013760B0.
// Layout: Players +0x0C (retail STLport vector<CPlayer*>, 12 bytes), PlayerNeutral +0x18, MainPlayer +0x1C
// (engine/CPlayerManager.h carries the PDB's 16-byte vector, so the class is modelled locally as the landed
// PrepareForLoad 0x00449C30 does); CPlayer::Number +0x28.
// Shape: GetPlayer(long) (0x004498C0 out of line) is inlined: a signed indexed search over a count taken once, walking
// a pointer beside the index, the hit re-indexed from the first element, the miss falling back to Players[PlayerNeutral]. Both outer loops
// use unsigned indices against a size() re-derived after every call. clear() is the STLport `erase(begin(), end())`
// whose trivial pointer copy is `last == first ? result : memmove(result, first, n) + n`. The two number resets
// re-read the global (it may alias the first store).
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