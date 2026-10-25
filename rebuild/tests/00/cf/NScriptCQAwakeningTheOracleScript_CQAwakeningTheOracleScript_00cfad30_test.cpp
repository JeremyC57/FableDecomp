// Behaviour test for NScript::CQ_AwakeningTheOracleScript::CQ_AwakeningTheOracleScript (0x00CFAD30): the level-script base stores the game interface and master
// data, and each STLport map member gets a malloc'd header node that is red, parentless and points at itself.
#include <cstdio>
#include <cstdlib>

class CGameScriptInterfaceBase { public: int id; };
class CScriptDataBase { public: int id; };
static int g_mallocs = 0;
static int g_scriptBaseCtors = 0;
struct CScriptBase {
    CScriptBase() { ++g_scriptBaseCtors; }
    virtual ~CScriptBase() { }
};
template <class TData> struct CLevelScriptBase : CScriptBase {
    CGameScriptInterfaceBase* Game;
    TData* MasterScriptData;
    CLevelScriptBase(CScriptDataBase* data, CGameScriptInterfaceBase& game) : CScriptBase(), Game(&game), MasterScriptData((TData*)data) { }
};
struct CNodeBase { bool color; CNodeBase* parent; CNodeBase* left; CNodeBase* right; };
template <class V> struct CNode : CNodeBase { V value; };
template <class V> struct CMap {
    CNode<V>* header;
    unsigned int node_count;
    CMap() : header(0)
    {
        header = (CNode<V>*)std::malloc(sizeof(CNode<V>));
        ++g_mallocs;
        node_count = 0;
        header->color = false;
        header->parent = 0;
        header->left = header;
        header->right = header;
    }
};
template <class T> struct CVector {
    T* _M_start; T* _M_finish; T* _M_end_of_storage;
    CVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
};
struct CThing { int id; };
struct CPairStrStr { void* first; void* second; };
struct CPairStrActor { void* first; void* second[4]; };
template <class V> bool Empty(const CMap<V>& m)
{
    return m.header != 0 && m.node_count == 0 && !m.header->color && m.header->parent == 0
        && m.header->left == m.header && m.header->right == m.header;
}
class CQ_AwakeningTheOracleScript : public CLevelScriptBase<CScriptDataBase> {
public:
    CVector<CThing> DiggingPoints;
    CMap<CPairStrStr> cs_args;
    CMap<CPairStrActor> cs_chars;
    CQ_AwakeningTheOracleScript(CScriptDataBase* data, CGameScriptInterfaceBase& game) : CLevelScriptBase<CScriptDataBase>(data, game) { }
};

int main()
{
    CGameScriptInterfaceBase game; game.id = 1;
    CScriptDataBase data; data.id = 2;
    CQ_AwakeningTheOracleScript* s = new CQ_AwakeningTheOracleScript(&data, game);
    bool ok = s->Game == &game && s->MasterScriptData == &data && g_scriptBaseCtors == 1 && Empty(s->cs_args) && Empty(s->cs_chars) && !s->DiggingPoints._M_start && !s->DiggingPoints._M_finish && !s->DiggingPoints._M_end_of_storage && g_mallocs == 2;
    std::free(s->cs_args.header); std::free(s->cs_chars.header);
    delete s;
    if (!ok) {
        std::printf("00cfad30_TEST FAIL\n");
        return 1;
    }
    std::printf("00cfad30_TEST PASS\n");
    return 0;
}