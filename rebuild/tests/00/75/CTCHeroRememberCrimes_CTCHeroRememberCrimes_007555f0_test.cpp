// Behaviour test for 0x007555F0 CTCHeroRememberCrimes::CTCHeroRememberCrimes(CThing*)
#include <cstdio>
#include <cstdlib>
#include <new>

struct CThing { int id; };
struct CRetailLess { };

struct CCrimeMap {
    struct node {
        char colour;
        node* parent;
        node* left;
        node* right;
        unsigned char value[0xC];
    };
    node*         m_header;
    unsigned long m_nodeCount;
    CRetailLess   m_compare;
    CCrimeMap() : m_header(0)
    {
        m_header = (node*)std::malloc(sizeof(node));
        m_nodeCount = 0;
        m_header->colour = 0;
        m_header->parent = 0;
        m_header->left = m_header;
        m_header->right = m_header;
    }
};

static int g_baseCalls = 0;
struct CTCBase_Calls {
    CThing* ParentThing;
    CTCBase_Calls(CThing* parent) : ParentThing(parent) { ++g_baseCalls; }
};

struct CTCHeroRememberCrimes : CTCBase_Calls {
    CCrimeMap RegionNamesToCrimeDurationAndFine;
    bool      HasPaidTheFine;
    CTCHeroRememberCrimes(CThing* parent);
};

CTCHeroRememberCrimes::CTCHeroRememberCrimes(CThing* parent)
    : CTCBase_Calls(parent)
    , HasPaidTheFine(false)
{
}

int main()
{
    CThing thing;
    thing.id = 3;
    CTCHeroRememberCrimes* tc = (CTCHeroRememberCrimes*)std::malloc(sizeof(CTCHeroRememberCrimes));
    *(unsigned char*)&tc->HasPaidTheFine = 0xAB;
    tc->RegionNamesToCrimeDurationAndFine.m_nodeCount = 99;
    new (tc) CTCHeroRememberCrimes(&thing);
    int ok = 1;
    CCrimeMap::node* h = tc->RegionNamesToCrimeDurationAndFine.m_header;
    if (g_baseCalls != 1 || tc->ParentThing != &thing) ok = 0;
    if (!h || h->colour != 0 || h->parent != 0 || h->left != h || h->right != h) ok = 0;
    if (tc->RegionNamesToCrimeDurationAndFine.m_nodeCount != 0) ok = 0;
    if (tc->HasPaidTheFine != false) ok = 0;
    std::free(h);
    std::free(tc);
    if (!ok) { std::printf("007555f0_TEST FAIL\n"); return 1; }
    std::printf("007555f0_TEST PASS\n");
    return 0;
}