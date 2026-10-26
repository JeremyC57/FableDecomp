// Behaviour test for NScript::CGameflowScript::Alloc (0x00CEF950): a new Gameflow script with the game interface
// and master data stored by the level-script base and both saved-name vectors empty.
#include <cstdio>

class CGameScriptInterfaceBase { public: int id; };
class CScriptDataBase { public: int id; };
static int g_scriptBaseCtors = 0;
class CScriptBase { public:
    CScriptBase() { ++g_scriptBaseCtors; }
    virtual ~CScriptBase() { }
};
template <class TData> struct CLevelScriptBase : CScriptBase {
    CGameScriptInterfaceBase* Game;
    TData* MasterScriptData;
    CLevelScriptBase(CScriptDataBase* data, CGameScriptInterfaceBase& game) : CScriptBase(), Game(&game), MasterScriptData((TData*)data) { }
};
template <class T> struct CVector {
    T* _M_start; T* _M_finish; T* _M_end_of_storage;
    CVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
};
struct CName { int id; };
class CGameflowScript : public CLevelScriptBase<CScriptDataBase> {
public:
    unsigned long CoreQuestWaiting;
    CVector<CName> SavedScriptNames;
    CVector<CName> SavedCardDefNames;
    CGameflowScript(CScriptDataBase* data, CGameScriptInterfaceBase& game) : CLevelScriptBase<CScriptDataBase>(data, game) { }
    static CScriptBase* Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game) { return new CGameflowScript(data, game); }
};

int main()
{
    CGameScriptInterfaceBase game; game.id = 1;
    CScriptDataBase data; data.id = 2;
    CGameflowScript* s = (CGameflowScript*)CGameflowScript::Alloc(&data, game);
    if (s == 0 || s->Game != &game || s->MasterScriptData != &data || g_scriptBaseCtors != 1
        || s->SavedScriptNames._M_start || s->SavedScriptNames._M_finish || s->SavedScriptNames._M_end_of_storage
        || s->SavedCardDefNames._M_start || s->SavedCardDefNames._M_finish || s->SavedCardDefNames._M_end_of_storage) {
        std::printf("00cef950_TEST FAIL\n");
        return 1;
    }
    delete s;
    std::printf("00cef950_TEST PASS\n");
    return 0;
}