// Behaviour test for 0x007D7950 CCardDef::~CCardDef: the implicit destructor frees the SubDefInfoMap vector storage
// (when allocated) and then runs the CDefPointeeBase / CResource destructors.
#include <cstdio>
#include <cstdlib>
static int g_freed = 0, g_resourceDtors = 0;
static void TrackedFree(void* p) { ++g_freed; std::free(p); }
class CResource { public: virtual ~CResource() { ++g_resourceDtors; } };
class CDefPointeeBase : public CResource { public: virtual ~CDefPointeeBase() { } };
class CDefClassBase : public CDefPointeeBase { public:
    class CSubDefInfo { public: long DefIndex; long OriginalParentDefIndex; };
};
struct SPair { unsigned long first; CDefClassBase::CSubDefInfo second; };
struct CSubDefInfoMap {
    SPair* _M_start; SPair* _M_finish; SPair* _M_end_of_storage; bool Dirty;
    CSubDefInfoMap() : _M_start(0), _M_finish(0), _M_end_of_storage(0), Dirty(false) { }
    ~CSubDefInfoMap() { if (_M_start != 0) TrackedFree(_M_start); }
};
class CParentDefClassBase : public CDefClassBase { public:
    long InstantiationName;
    CSubDefInfoMap SubDefInfoMap;
};
class CCardDef : public CParentDefClassBase { public:
    long CardName, CardVal;
};
int main()
{
    {
        CCardDef card;
        card.SubDefInfoMap._M_start = (SPair*)std::malloc(2 * sizeof(SPair));
        card.SubDefInfoMap._M_finish = card.SubDefInfoMap._M_start + 2;
        card.SubDefInfoMap._M_end_of_storage = card.SubDefInfoMap._M_finish;
    }
    if (g_freed != 1 || g_resourceDtors != 1) { std::printf("007d7950_TEST FAIL\n"); return 1; }
    {
        CCardDef card;                                     // empty map: nothing freed
    }
    if (g_freed != 1 || g_resourceDtors != 2) { std::printf("007d7950_TEST FAIL\n"); return 1; }
    std::printf("007d7950_TEST PASS\n");
    return 0;
}