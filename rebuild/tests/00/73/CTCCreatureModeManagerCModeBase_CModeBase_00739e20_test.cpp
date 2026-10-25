// Behaviour test for 0x00739E20 CTCCreatureModeManager::CModeBase::~CModeBase() (members die: the filter's group string, then the filter base).
#include <cstdio>
#include <cstring>

static char g_order[8];
static int  g_n = 0;
class CCharString {
public:
    ~CCharString() { g_order[g_n++] = 'S'; }
};
class CAnimFilterBase {
public:
    virtual ~CAnimFilterBase() { g_order[g_n++] = 'F'; }
};
class CAnimFilterOfGroup : public CAnimFilterBase {
public:
    CCharString   Group;
    unsigned long GroupCRC;
};
class CThing;
class CModeBase {
public:
    CThing*            ParentThing;
    CAnimFilterOfGroup WeaponBasedFilter;
    long               FramesBeforeRemovingSelf;
    bool               Active;
    virtual ~CModeBase() { g_order[g_n++] = 'M'; }
};
class CModeDerived : public CModeBase {
public:
    virtual ~CModeDerived() { g_order[g_n++] = 'D'; }
};

int main()
{
    CModeBase* mode = new CModeDerived;
    delete mode;
    g_order[g_n] = 0;
    if (!(g_n == 4 && std::strcmp(g_order, "DMSF") == 0)) {
        std::printf("00739e20_TEST FAIL\n");
        return 1;
    }
    std::printf("00739e20_TEST PASS\n");
    return 0;
}