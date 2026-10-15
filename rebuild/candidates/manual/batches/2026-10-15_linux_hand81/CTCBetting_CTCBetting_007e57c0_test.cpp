// Behaviour test for 0x007E57C0 CTCBetting::CTCBetting: both handle vectors empty then resized to 5,
// Active false, the two frame counters and PreviousDonation zero, Mult and FrameToStart untouched.
#include <cstdio>
#include <cstring>
#include <new>
struct CThing { long m_id; };

static int g_baseCtor;
struct CTCBase_Calls {
    virtual void v00() { }
    CThing* ParentThing;
    bool    ToBeRemoved;
    bool    IsInGlobalUpdateTCs;
    CTCBase_Calls(CThing* thing) : ParentThing(thing), ToBeRemoved(false), IsInGlobalUpdateTCs(false) { ++g_baseCtor; }
};

static double g_storage[64];
static void* Fresh() { std::memset(g_storage, 0xAB, sizeof g_storage); return g_storage; }
static long g_resized[4];
static int g_nr;
struct CRetailHandleVector {
    void* m_pFirst; void* m_pLast; void* m_pEnd;
    CRetailHandleVector() : m_pFirst(0), m_pLast(0), m_pEnd(0) { }
    void Resize(long count) { if (m_pFirst == 0 && m_pLast == 0 && m_pEnd == 0) g_resized[g_nr++] = count; }
};
struct CTCBetting : CTCBase_Calls {
    bool                Active;
    CRetailHandleVector BetHandles;
    CRetailHandleVector MoneyHandles;
    long FrameBetWasLastChanged, FrameBetChangeStarted, Mult, FrameToStart, PreviousDonation;
    CTCBetting(CThing* thing) : CTCBase_Calls(thing)
    {
        Active = false;
        BetHandles.Resize(5);
        MoneyHandles.Resize(5);
        FrameBetWasLastChanged = 0;
        FrameBetChangeStarted = 0;
        PreviousDonation = 0;
    }
};

static bool Untouched(const void* p, unsigned n)
{
    const unsigned char* b = (const unsigned char*)p;
    for (unsigned i = 0; i < n; ++i) if (b[i] != 0xAB) return false;
    return true;
}

int main()
{
    CThing thing;
    CTCBetting* p = new (Fresh()) CTCBetting(&thing);
    if (g_baseCtor != 1 || p->ParentThing != &thing || p->Active || g_nr != 2 || g_resized[0] != 5 || g_resized[1] != 5) { std::printf("007e57c0_TEST FAIL\n"); return 1; }
    if (p->FrameBetWasLastChanged != 0 || p->FrameBetChangeStarted != 0 || p->PreviousDonation != 0 || !Untouched(&p->Mult, sizeof p->Mult) || !Untouched(&p->FrameToStart, sizeof p->FrameToStart)) { std::printf("007e57c0_TEST FAIL\n"); return 1; }
    std::printf("007e57c0_TEST PASS\n");
    return 0;
}
