// Behaviour test for CTCTavernGame::ClearBetAndMoney (0x005E9F30): every handle in the three display vectors is cleared;
// a primitive held by one handle is notified (slot 3) and destroyed (slot 1), a shared one is only decremented.
#include <cstdio>
struct CEnginePrimitiveHandle;
struct CEnginePrimitive {
    int notified, destroyed;
    CEnginePrimitiveHandle* lastHandle;
    virtual void v00() { }
    virtual void Destroy() { ++destroyed; }
    virtual void v02() { }
    virtual void OnLastHandleReleased(CEnginePrimitiveHandle* handle) { ++notified; lastHandle = handle; }
    long RefCount;
    void Release() { if (--RefCount == 0) Destroy(); }
};
struct CEnginePrimitiveHandle {
    virtual ~CEnginePrimitiveHandle() { }
    CEnginePrimitive* P;
    long GetRefCount() const { return P ? P->RefCount : 0; }
    void Reset()
    {
        if (GetRefCount() == 1)
            P->OnLastHandleReleased(this);
        if (P) {
            P->Release();
            P = 0;
        }
        P = 0;
    }
};
struct CHandleVector {
    CEnginePrimitiveHandle* _M_start;
    CEnginePrimitiveHandle* _M_finish;
    CEnginePrimitiveHandle* _M_end_of_storage;
    CEnginePrimitiveHandle* begin() { return _M_start; }
    CEnginePrimitiveHandle* end() { return _M_finish; }
};
struct CTCTavernGame {
    void* __vfptr;
    void* ParentThing;
    unsigned char _pad08[0x44 - 0x08];
    CHandleVector Handles;
    CHandleVector BoxHandles;
    CHandleVector BetHandles;
    CHandleVector MoneyHandles;
    CHandleVector WinningsHandles;
    void ClearBetAndMoney();
};

void CTCTavernGame::ClearBetAndMoney()
{
    CEnginePrimitiveHandle* h;
    for (h = BetHandles.begin(); h != BetHandles.end(); ++h)
        h->Reset();
    for (h = MoneyHandles.begin(); h != MoneyHandles.end(); ++h)
        h->Reset();
    for (h = WinningsHandles.begin(); h != WinningsHandles.end(); ++h)
        h->Reset();
}

static void Init(CEnginePrimitive& p, long count) { p.notified = 0; p.destroyed = 0; p.lastHandle = 0; p.RefCount = count; }
static void Set(CHandleVector& v, CEnginePrimitiveHandle* a, int n) { v._M_start = a; v._M_finish = a + n; v._M_end_of_storage = a + n; }

int main()
{
    int bad = 0;
    CEnginePrimitive solo, shared, money, untouched;
    Init(solo, 1); Init(shared, 3); Init(money, 1); Init(untouched, 1);
    CEnginePrimitiveHandle bet[3], cash[1], box[1];
    bet[0].P = &solo; bet[1].P = &shared; bet[2].P = 0;
    cash[0].P = &money; box[0].P = &untouched;
    CTCTavernGame g;
    Set(g.Handles, 0, 0); Set(g.BoxHandles, box, 1);
    Set(g.BetHandles, bet, 3); Set(g.MoneyHandles, cash, 1); Set(g.WinningsHandles, 0, 0);
    g.ClearBetAndMoney();
    if (bet[0].P || bet[1].P || bet[2].P || cash[0].P) bad |= 1;
    if (solo.notified != 1 || solo.lastHandle != &bet[0] || solo.destroyed != 1 || solo.RefCount != 0) bad |= 2;
    if (shared.notified != 0 || shared.destroyed != 0 || shared.RefCount != 2) bad |= 4;
    if (money.notified != 1 || money.destroyed != 1) bad |= 8;
    if (box[0].P != &untouched || untouched.RefCount != 1 || untouched.notified) bad |= 16;
    if (g.BetHandles._M_finish != bet + 3) bad |= 32;           // the vectors are not emptied
    if (bad) { std::printf("005e9f30_TEST FAIL\n"); return 1; }
    std::printf("005e9f30_TEST PASS\n");
    return 0;
}
