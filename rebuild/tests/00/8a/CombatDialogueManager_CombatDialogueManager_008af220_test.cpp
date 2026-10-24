#include <cstdio>
#include <cstdlib>
#include <new>
// Model of CombatDialogueManager::CombatDialogueManager() @ 0x008AF220: an empty STLport map (malloc'd header, red,
// no root, self-linked), a null def pointer, a null current talker (assigned through a temporary), zeroed text-group
// index and frame counters.
struct _Rb_tree_node_base {
    bool                _M_color;
    _Rb_tree_node_base* _M_parent;
    _Rb_tree_node_base* _M_left;
    _Rb_tree_node_base* _M_right;
};
template <class V> struct _Rb_tree_node : public _Rb_tree_node_base { V _M_value_field; };
template <class V>
class _Rb_tree {
public:
    typedef _Rb_tree_node<V> _Node;
    _Node*       _M_header;
    unsigned int _M_node_count;
    _Rb_tree() : _M_node_count(0)
    {
        _M_header = (_Node*)malloc(sizeof(_Node));
        _M_empty_initialize();
    }
    void _M_empty_initialize()
    {
        _M_header->_M_color = false;
        _M_header->_M_parent = 0;
        _M_header->_M_left = _M_header;
        _M_header->_M_right = _M_header;
    }
};
template <class K, class T> struct pair { K first; T second; };
template <class K, class T> class map { public: _Rb_tree<pair<const K, T> > _M_t; };
struct CThingCreatureBase { int Id; };
static int g_setCalls = 0;
class CBaseIntelligentPointer {
public:
    CBaseIntelligentPointer() : PData(0) { }
    virtual ~CBaseIntelligentPointer() { }
    void* GetPItem() const { return PData; }
    void SetPItem(void* item) { ++g_setCalls; PData = item; }
    void* PData;
};
template <class T>
class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    CIntelligentPointer() { }
    CIntelligentPointer(T* item) { SetPItem(item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other)
    {
        SetPItem(other.GetPItem());
        return *this;
    }
};
struct CCombatDialogueDef { int Id; };
template <class T> class CDefPointer {
public:
    const void* Object;
    CDefPointer() : Object(0) { }
};
class CombatDialogueManager {
public:
    struct TSpeechGroupInfo { unsigned int Data; };
    map<unsigned int, TSpeechGroupInfo>      SpeechGroupInfoMap;
    CDefPointer<const CCombatDialogueDef>    PDialogueDef;
    CIntelligentPointer<CThingCreatureBase>  PCurrentTalker;
    unsigned int CurrentTalkerTextGroupIndex;
    int          LastSpeechFrame;
    int          LastBattleCryFrame;
    CombatDialogueManager();
};
CombatDialogueManager::CombatDialogueManager()
    : LastSpeechFrame(0), LastBattleCryFrame(0)
{
    PCurrentTalker = 0;
    CurrentTalkerTextGroupIndex = 0;
}
union Storage {                                         // raw, aligned storage pre-filled with a pattern
    double        align;
    void*         alignPtr;
    unsigned char bytes[sizeof(CombatDialogueManager)];
};
int main()
{
    bool ok = true;
    static Storage storage;
    for (unsigned int i = 0; i < sizeof(storage.bytes); ++i)
        storage.bytes[i] = 0xAB;
    CombatDialogueManager* m = new (storage.bytes) CombatDialogueManager;
    _Rb_tree_node_base* header = m->SpeechGroupInfoMap._M_t._M_header;
    if (header == 0 || header->_M_color || header->_M_parent != 0 || header->_M_left != header || header->_M_right != header)
        ok = false;
    if (m->SpeechGroupInfoMap._M_t._M_node_count != 0 || m->PDialogueDef.Object != 0) ok = false;
    if (m->PCurrentTalker.GetPItem() != 0 || g_setCalls != 2) ok = false;   // the temporary's ctor + operator=
    if (m->CurrentTalkerTextGroupIndex != 0 || m->LastSpeechFrame != 0 || m->LastBattleCryFrame != 0) ok = false;
    m->~CombatDialogueManager();
    free(header);
    if (!ok) { std::printf("008af220_TEST FAIL\n"); return 1; }
    std::printf("008af220_TEST PASS\n");
    return 0;
}