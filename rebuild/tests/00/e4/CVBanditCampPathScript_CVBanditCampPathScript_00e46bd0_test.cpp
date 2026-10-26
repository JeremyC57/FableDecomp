// Behaviour test for CV_BanditCampPathScript::CV_BanditCampPathScript (0x00E46BD0).
#include <cstdio>

class CGameScriptInterface { public: int dummy; };

static int g_baseBuilt = 0, g_cbaseBuilt = 0;
class CScriptBase { public:
    virtual void v00() { }
    unsigned char _body[0x3c];
    CScriptBase() { ++g_baseBuilt; }
};

struct CScriptObject : CScriptBase {
    CGameScriptInterface* m_pGame;
    void* m_pOwner;
    CScriptObject(void* owner, CGameScriptInterface* game) : CScriptBase(), m_pGame(game), m_pOwner(owner) { }
};

namespace N_0099a2d0 { struct CBase {
    CBase() { ++g_cbaseBuilt; }
    virtual ~CBase() { }
}; } using N_0099a2d0::CBase;

class CScriptThing;
class CCPPointerInfo;
struct CCountedPointer_CScriptThing {
    CScriptThing* Data;
    CCPPointerInfo* Info;
    CCountedPointer_CScriptThing() : Data(0), Info(0) { }
};

class CScriptThing : public CBase { public:
    CCountedPointer_CScriptThing PImp;
    CScriptThing() { }
    virtual ~CScriptThing() { }
};

template <class T>
struct allocator { allocator() { } };

template <class T, class A>
struct _STLP_alloc_proxy : A {
    T _M_data;
    _STLP_alloc_proxy(const A& a, T p) : A(a), _M_data(p) { }
};

template <class T>
struct _Vector_base {
    T* _M_start;
    T* _M_finish;
    _STLP_alloc_proxy<T*, allocator<T> > _M_end_of_storage;
    _Vector_base(const allocator<T>& a) : _M_start(0), _M_finish(0), _M_end_of_storage(a, (T*)0) { }
};

template <class T>
struct CRetailVector : _Vector_base<T> {
    CRetailVector() : _Vector_base<T>(allocator<T>()) { }
};

struct CV_BanditCampPathScript : CScriptObject {
    bool OverheardBanditConversation;
    CRetailVector<CScriptThing> Bandits;
    CScriptThing Bandit1;
    CScriptThing Bandit2;
    virtual void v00() { }
    CV_BanditCampPathScript(void* owner, CGameScriptInterface* game);
};

CV_BanditCampPathScript::CV_BanditCampPathScript(void* owner, CGameScriptInterface* game)
    : CScriptObject(owner, game)
{
}

int main()
{
    CGameScriptInterface gsi;
    int owner = 0;
    CV_BanditCampPathScript s(&owner, &gsi);
    if (g_baseBuilt != 1 || g_cbaseBuilt != 2 || s.m_pGame != &gsi || s.m_pOwner != &owner
        || s.Bandits._M_start != 0 || s.Bandits._M_finish != 0 || s.Bandits._M_end_of_storage._M_data != 0
        || s.Bandit1.PImp.Data != 0 || s.Bandit1.PImp.Info != 0 || s.Bandit2.PImp.Data != 0 || s.Bandit2.PImp.Info != 0) {
        std::printf("00e46bd0_TEST FAIL\n");
        return 1;
    }
    std::printf("00e46bd0_TEST PASS\n");
    return 0;
}