#include <cstdio>
// CAIStateGroupBase::~CAIStateGroupBase(): Name's destructor (this + 0x0C), then ~CBaseClass; the vptr untouched
static int g_inner = 0, g_outer = 0, g_order = 0;
static void* g_name_at = 0;
class __declspec(novtable) CBaseClass {
public:
    virtual ~CBaseClass();
};
class CCharString {
public:
    ~CCharString();
    void* PString;
};
class CAIBrain;
class __declspec(novtable) CAIStateGroupBase : public CBaseClass {
public:
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;
    long ScriptAIPriority;
    CCharString Name;
};
CCharString::~CCharString() { g_inner++; g_name_at = this; }
CBaseClass::~CBaseClass() { g_outer++; if (g_inner == 1) g_order = 1; }
int main()
{
    void* buf[8] = {(void*)0x1234, 0, 0, 0, 0, 0, 0, 0};
    ((CAIStateGroupBase*)buf)->CAIStateGroupBase::~CAIStateGroupBase();
    if (g_inner != 1 || g_name_at != (void*)((char*)buf + 0xc)) { std::printf("BAD_INNER\n"); return 1; }
    if (g_outer != 1 || !g_order) { std::printf("BAD_OUTER\n"); return 1; }
    if (buf[0] != (void*)0x1234) { std::printf("BAD_VPTR\n"); return 1; }
    std::printf("DESTVAL_008fd288_OK\n"); return 0;
}
