// Behaviour test for the CV_TravellingHeroesScript ctor (0x00EDEBA0).
#include <cstdio>
#include <cstdlib>
static int g_baseObjects = 0, g_scriptBases = 0, g_strings = 0;
struct CGameScriptInterface {
    long NextTimer;
    long RegisterTimer() { return NextTimer++; }
};
struct CScriptDataBase { int Dummy; };
struct CCharString { void* PStringData; CCharString() : PStringData(0) { ++g_strings; } };
struct CCPPointerInfo { long RefCount; };
template <class T> struct CCountedPointer {
    T* Data; CCPPointerInfo* Info;
    CCountedPointer() : Data(0), Info(0) {}
};
struct CBaseObject {
    void* IntelligentPointer;
    CBaseObject() : IntelligentPointer(0) { ++g_baseObjects; }
};
struct CScriptGameResourceObjectBase : CBaseObject {};
struct CScriptGameResourceObjectScriptedThingBase : CScriptGameResourceObjectBase {
    CCountedPointer<CScriptGameResourceObjectScriptedThingBase> PImp;
};
struct CScriptBase { CScriptBase() { ++g_scriptBases; } };
struct CQuestScriptBase : CScriptBase {
    CGameScriptInterface* GSI;
    CScriptDataBase*      Data;
    CQuestScriptBase(CScriptDataBase* data, CGameScriptInterface* gsi) : CScriptBase(), GSI(gsi), Data(data) {}
};
static bool Empty(const CScriptGameResourceObjectScriptedThingBase& h) { return h.PImp.Data == 0 && h.PImp.Info == 0 && h.IntelligentPointer == 0; }
struct _Rb_tree_node_base { char _M_color; _Rb_tree_node_base* _M_parent; _Rb_tree_node_base* _M_left; _Rb_tree_node_base* _M_right; };
template <unsigned int NodeSize> struct _Rb_tree_base {
    _Rb_tree_node_base* _M_header;
    _Rb_tree_base() : _M_header(0) { _M_header = (_Rb_tree_node_base*)std::malloc(NodeSize); }
};
template <unsigned int NodeSize> struct map : _Rb_tree_base<NodeSize> {
    unsigned long _M_node_count;
    unsigned char _M_key_compare;
    map() : _M_node_count(0) { _M_empty_initialize(); }
    void _M_empty_initialize()
    {
        this->_M_header->_M_color = 0;
        this->_M_header->_M_parent = 0;
        this->_M_header->_M_left = this->_M_header;
        this->_M_header->_M_right = this->_M_header;
    }
};
struct CV_TravellingHeroesScript : CQuestScriptBase {
    CScriptGameResourceObjectScriptedThingBase seh_Rival, seh_Girl, seh_Boy;
    bool Flags[6];
    map<sizeof(_Rb_tree_node_base) + 2 * sizeof(void*)> csargs;   // 0x18-byte nodes in the 32-bit build
    CV_TravellingHeroesScript(CScriptDataBase* data, CGameScriptInterface& gsi);
};
CV_TravellingHeroesScript::CV_TravellingHeroesScript(CScriptDataBase* data, CGameScriptInterface& gsi)
    : CQuestScriptBase(data, &gsi)
{
}
int main()
{
    CGameScriptInterface gsi; CScriptDataBase db;
    CV_TravellingHeroesScript* q = new CV_TravellingHeroesScript(&db, gsi);
    _Rb_tree_node_base* h = q->csargs._M_header;
    int bad = q->GSI != &gsi || q->Data != &db || g_scriptBases != 1 || g_baseObjects != 3
        || !Empty(q->seh_Rival) || !Empty(q->seh_Girl) || !Empty(q->seh_Boy)
        || !h || h->_M_color != 0 || h->_M_parent != 0 || h->_M_left != h || h->_M_right != h || q->csargs._M_node_count != 0;
    if (bad) { std::printf("00edeba0_TEST FAIL\n"); return 1; }
    std::printf("00edeba0_TEST PASS\n");
    return 0;
}