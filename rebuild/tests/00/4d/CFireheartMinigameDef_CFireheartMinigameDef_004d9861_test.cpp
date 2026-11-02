// Test: CFireheartMinigameDef::~CFireheartMinigameDef() destroys SunFX, then MoonFX, then the def base chain down to ~CResource.
#include <cstdio>
static int g_order[8];
static int g_count = 0;
class CCharString { public:
    int Id;
    ~CCharString() { g_order[g_count++] = Id; }
};
class CResource { public:
    virtual ~CResource() { g_order[g_count++] = 100; }
};
class CDefPointeeBase : public CResource { public: virtual ~CDefPointeeBase() { } };
class CDefClassBase : public CDefPointeeBase { public: virtual ~CDefClassBase() { } };
class CSubDefClassBase : public CDefClassBase { public: virtual ~CSubDefClassBase() { } };
class CFireheartMinigameDef : public CSubDefClassBase { public:
    CCharString   MoonFX;
    CCharString   SunFX;
    unsigned long QuitText;
    unsigned long YesText;
    unsigned long NoText;
};
int main()
{
    {
        CFireheartMinigameDef def;
        def.MoonFX.Id = 1;
        def.SunFX.Id = 2;
    }
    if (g_count != 3 || g_order[0] != 2 || g_order[1] != 1 || g_order[2] != 100) { std::printf("004d9861_TEST FAIL\n"); return 1; }
    CResource* p = new CFireheartMinigameDef;
    static_cast<CFireheartMinigameDef*>(p)->MoonFX.Id = 3;
    static_cast<CFireheartMinigameDef*>(p)->SunFX.Id = 4;
    g_count = 0;
    delete p;                                     // through the virtual destructor (slot 0, the ??_G)
    if (g_count != 3 || g_order[0] != 4 || g_order[1] != 3 || g_order[2] != 100) { std::printf("004d9861_TEST FAIL\n"); return 1; }
    std::printf("004d9861_TEST PASS\n");
    return 0;
}