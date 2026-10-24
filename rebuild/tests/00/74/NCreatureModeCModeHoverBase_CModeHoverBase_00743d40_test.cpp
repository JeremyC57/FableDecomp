// Behaviour test for 0x00743D40 NCreatureMode::CModeHoverBase::~CModeHoverBase()
#include <cstdio>
#include <cstdlib>
#include <new>

static int g_log[64];
static int g_n = 0;
static void Log(int v) { if (g_n < 64) g_log[g_n++] = v; }

class CBaseIntelligentPointer {
public:
    int tag;
    virtual ~CBaseIntelligentPointer() { Log(tag); }      // stand-in for 0x00A01C10
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { };

class CCharString {
public:
    int tag;
    ~CCharString() { Log(tag); }
};
class CAnimFilterBase { public: virtual void v00() {} ~CAnimFilterBase() {} };
class CAnimFilterOfGroup : public CAnimFilterBase { public: CCharString Group; unsigned int GroupCRC; };

class CThing;
class CModeBase {
public:
    virtual void v00() {}
    CThing*            ParentThing;
    CAnimFilterOfGroup WeaponBasedFilter;
    int                FramesBeforeRemovingSelf;
    bool               Active;
    ~CModeBase() {}
};

template <class T> inline void _Destroy(T* p) { p->~T(); }
template <class ForwardIter> inline void _Destroy(ForwardIter first, ForwardIter last)
{
    for (; first != last; ++first)
        _Destroy(&*first);
}
template <class T> class vector {
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    ~vector()
    {
        _Destroy(_M_start, _M_finish);
        if (_M_start)
            std::free(_M_start);
    }
};

struct _List_node_base { _List_node_base* _M_next; _List_node_base* _M_prev; };
static int g_clears = 0;
template <class T> class list {
public:
    _List_node_base* _M_node;
    void clear() { ++g_clears; Log(500); _M_node->_M_next = _M_node; _M_node->_M_prev = _M_node; }   // stand-in for 0x00742290
    ~list()
    {
        clear();
        if (_M_node)
            std::free(_M_node);
    }
};

class CModeHoverBase : public CModeBase {
public:
    struct CHoverState {
        CCharString AnimName;
        float       Area[3];
        int         MovementType;
        float       MovementSpeed;
    };
    vector<CHoverState>          HoverStates;
    CCharString                  CurrentName;
    CHoverState*                 PCurrentState;
    int                          CurrentMovementType;
    list<int>                    AnimQueue;
    CIntelligentPointer<CThing>  PParticleEffect;
    ~CModeHoverBase() {}
};

int main()
{
    int ok = 1;
    CModeHoverBase* m = (CModeHoverBase*)std::malloc(sizeof(CModeHoverBase));
    new (m) CModeHoverBase();
    m->WeaponBasedFilter.Group.tag = 5;
    m->HoverStates._M_start = (CModeHoverBase::CHoverState*)std::malloc(sizeof(CModeHoverBase::CHoverState) * 2);
    m->HoverStates._M_start[0].AnimName.tag = 30;
    m->HoverStates._M_start[1].AnimName.tag = 31;
    m->HoverStates._M_finish = m->HoverStates._M_start + 2;
    m->HoverStates._M_end_of_storage = m->HoverStates._M_finish;
    m->CurrentName.tag = 20;
    m->AnimQueue._M_node = (_List_node_base*)std::malloc(sizeof(_List_node_base));
    m->PParticleEffect.tag = 40;
    m->~CModeHoverBase();
    std::free(m);
    int want[] = { 40, 500, 20, 30, 31, 5 };
    if (g_n != (int)(sizeof(want) / sizeof(want[0]))) ok = 0;
    for (int i = 0; ok && i < g_n; ++i) if (g_log[i] != want[i]) ok = 0;
    if (g_clears != 1) ok = 0;
    if (!ok) { std::printf("00743d40_TEST FAIL\n"); return 1; }
    std::printf("00743d40_TEST PASS\n");
    return 0;
}