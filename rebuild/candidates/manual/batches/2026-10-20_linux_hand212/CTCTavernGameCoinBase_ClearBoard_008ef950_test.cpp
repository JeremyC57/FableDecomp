// Behaviour test for 008ef950: a self-contained copy of the candidate's model and body, run over stub callees.
// CTCTavernGameCoinBase::ClearBoard() -- kills every owned coin thing that is still alive, then empties the coin list
#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __forceinline inline
#define __fastcall
#define __stdcall
#define __cdecl
#define __declspec(x)
#endif
struct CBaseObject;

struct CThing {
    unsigned char _pad_0x00[0x91];
    unsigned char m_flags;                      // +0x91 bit 0: dead
    bool IsDead() const { return (m_flags & 1) != 0; }
    void Kill(bool immediately);                // 0x004C9B80
};

struct CBaseIntelligentPointer {                // 8 bytes: vptr + item
    virtual void v00();
    CBaseObject* m_pItem;                       // +0x04
    CThing* GetPItem() const;                   // 0x00A01B50
};
struct CIntelligentPointerThing : CBaseIntelligentPointer {   // CIntelligentPointer<CThing>
    virtual void v00();
};

struct Coin {                                   // 0x3c bytes
    virtual ~Coin();
    unsigned char _pad_0x04[0x20];
    bool          Owned;                        // +0x24
    CIntelligentPointerThing Thing;             // +0x25
    float         Radius;                       // +0x30
    long          Owner;                        // +0x34
    unsigned long Sound;                        // +0x38
};

struct random_access_iterator_tag { random_access_iterator_tag() { } };
Coin* __fastcall __copy(Coin* first, Coin* last, Coin* result, const random_access_iterator_tag& tag, int* distance);   // 0x008F02E0

struct CCoinVector {                            // STLport vector<Coin> (12 bytes)
    Coin* _M_start;
    Coin* _M_finish;
    Coin* _M_end_of_storage;
    Coin* begin() { return _M_start; }
    Coin* end() { return _M_finish; }
    static void _Destroy(Coin* first, Coin* last)
    {
        for (; first != last; ++first)
            first->~Coin();
    }
    Coin* erase(Coin* first, Coin* last)
    {
        Coin* i = __copy(last, _M_finish, first, random_access_iterator_tag(), (int*)0);
        _Destroy(i, _M_finish);
        _M_finish = i;
        return first;
    }
    void clear() { erase(begin(), end()); }
};

struct CTCTavernGameCoinBase {
    virtual void v00();
    unsigned char _body04[0xfc - 0x04];         // CTCTavernGame (retail 0xf0) + Arrow, Arrow1, Arrow2
    CCoinVector   Coins;                        // +0xfc
    void ClearBoard();
};

void CTCTavernGameCoinBase::ClearBoard()
{
    for (Coin* coin = Coins.begin(); coin != Coins.end(); ++coin) {
        if (coin->Thing.GetPItem() && !coin->Thing.GetPItem()->IsDead() && coin->Owned)
            coin->Thing.GetPItem()->Kill(true);
    }
    Coins.clear();
}

// ---- stubs and driver ----
void CBaseIntelligentPointer::v00() {}
void CIntelligentPointerThing::v00() {}
void CTCTavernGameCoinBase::v00() {}
CThing* CBaseIntelligentPointer::GetPItem() const { return (CThing*)m_pItem; }
static CThing* g_killed[4]; static int g_kills; static bool g_imm;
void CThing::Kill(bool immediately) { if (g_kills < 4) g_killed[g_kills] = this; ++g_kills; g_imm = immediately; }
static int g_coinDtors;
Coin::~Coin() { ++g_coinDtors; }
Coin* __fastcall __copy(Coin* first, Coin* last, Coin* result, const random_access_iterator_tag&, int* distance)
{ if (distance) return 0; while (first != last) { *result = *first; ++first; ++result; } return result; }

static Coin g_coins[4];
static CThing g_things[4];

int main()
{
    static CTCTavernGameCoinBase game;
    g_coins[0].Owned = true;  g_coins[0].Thing.m_pItem = (CBaseObject*)&g_things[0];                          // alive, owned: killed
    g_coins[1].Owned = false; g_coins[1].Thing.m_pItem = (CBaseObject*)&g_things[1];                          // not owned
    g_coins[2].Owned = true;  g_coins[2].Thing.m_pItem = (CBaseObject*)&g_things[2]; g_things[2].m_flags = 1; // dead
    g_coins[3].Owned = true;  g_coins[3].Thing.m_pItem = 0;                                                   // no thing
    game.Coins._M_start = g_coins; game.Coins._M_finish = g_coins + 4; game.Coins._M_end_of_storage = g_coins + 4;
    game.ClearBoard();
    if (g_kills != 1 || g_killed[0] != &g_things[0] || !g_imm) { std::printf("008ef950_TEST FAIL (kills)\n"); return 1; }
    if (game.Coins._M_finish != g_coins || g_coinDtors != 4) { std::printf("008ef950_TEST FAIL (clear)\n"); return 1; }
    std::printf("008ef950_TEST PASS\n");
    return 0;
}
