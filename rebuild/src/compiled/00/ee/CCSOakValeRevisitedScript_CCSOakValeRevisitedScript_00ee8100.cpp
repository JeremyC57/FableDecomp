// CCS_OakValeRevisitedScript::CCS_OakValeRevisitedScript(void* data, CGameScriptInterface* game) -- cut-scene script ctor: script base, interface/data pair, vtable 0x012F53C0, the empty FirePoint/Fires vectors and OakValeFlag map, and the seven v1..v7 scripted-thing handles
// Leaf: ??0CCS_OakValeRevisitedScript
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00EE8100, length 266 bytes (`ret 8`); the only caller is its factory
// thunk at 0x00EE8BFB.
// Callees: CScriptBase::CScriptBase @ 0x00CB8110, malloc @ 0x00BFEA0E (the map's 0x18-byte header),
// CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x0099A380 (x7).
// Layout (retail = PDB -0x14 for the base, 12-byte STLport containers): FirePoint +0x48, Fires +0x54
// (vector<CScriptThing>), OakValeFlag +0x60 (map<CCharString,bool>), v1..v7 +0x6c..+0xcc (vtable 0x0127094C,
// 0x10 bytes each); size 0xdc.
// Shape: the landed CV_ChickenKickingScript (0x00E62830) / CQ_HeroSoulsBriarScript (0x00D80C30) model; the two
// vectors are STLport `_Vector_base` + `_STLP_alloc_proxy` (six zero words from the one ebx), the map header is
// zeroed, malloc'd and self-linked by `_M_empty_initialize`.
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
struct CGameScriptInterface {                      // retail vtable @ 0x01260F0C
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009();
    virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019();
    virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029();
    virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047(); virtual void v048(); virtual void v049();
    virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055(); virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059();
    virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063(); virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069();
    virtual void v070(); virtual void v071(); virtual void v072(); virtual void v073(); virtual void v074(); virtual void v075(); virtual void v076(); virtual void v077(); virtual void v078(); virtual void v079();
    virtual void v080(); virtual void v081(); virtual void v082(); virtual void v083(); virtual void v084(); virtual void v085(); virtual void v086();
    virtual long RegisterTimer();   // slot 87 (+0x15c)
};
extern CGameScriptInterface* g_pGameScriptInterface;  // 0x0143E8F8
struct CTimer {                                    // a script timer: the interface's timer index
    long Index;
    CTimer() : Index(g_pGameScriptInterface->RegisterTimer()) { }
};
struct CScriptBase {                               // 0x40 bytes, the quest-script root (vtable 0x012C1648)
    virtual void v00();
    unsigned char _body[0x3c];                     // +0x04..+0x3f
    CScriptBase();                                 // 0x00CB8110
};
struct __declspec(novtable) CQuestScriptObject : CScriptBase {   // the argument-carrying intermediate base
    CGameScriptInterface* Game;                    // +0x40 (second argument)
    void*                 Data;                    // +0x44 (first argument, the script data)
    CQuestScriptObject(void* data, CGameScriptInterface* game) : CScriptBase(), Game(game), Data(data) { }
};
struct CCountedPointerWords {                     // {Data, Info}
    void* Data;
    void* Info;
    CCountedPointerWords() : Data(0), Info(0) { }
};
struct CBaseObject {                               // CBaseIntelligentPointer root
    virtual void v00();
    void* m_pIntelligentPointer;                   // +0x04
    CBaseObject();                                 // 0x0099A380
};
struct CScriptGameResourceObjectScriptedThingBase : CBaseObject {   // vtable 0x0127094C, 0x10 bytes
    CCountedPointerWords PImp;                     // +0x08 Data, +0x0c Info
    CScriptGameResourceObjectScriptedThingBase() : CBaseObject(), PImp() { }
    virtual void v00();
};
template <class T> struct allocator { allocator() { } };
template <class T, class A> struct _STLP_alloc_proxy : A {
    T _M_data;
    _STLP_alloc_proxy(const A& a, T p) : A(a), _M_data(p) { }
};
template <class T> struct _Vector_base {
    T* _M_start;
    T* _M_finish;
    _STLP_alloc_proxy<T*, allocator<T> > _M_end_of_storage;
    _Vector_base(const allocator<T>& a) : _M_start(0), _M_finish(0), _M_end_of_storage(a, (T*)0) { }
};
template <class T> struct CRetailVector : _Vector_base<T> {   // STLport vector<T>: three pointers (12 bytes in retail)
    CRetailVector() : _Vector_base<T>(allocator<T>()) { }
};
struct _Rb_tree_node_base {
    char                _M_color;                  // +0x00
    _Rb_tree_node_base* _M_parent;                 // +0x04
    _Rb_tree_node_base* _M_left;                   // +0x08
    _Rb_tree_node_base* _M_right;                  // +0x0c
};
template <unsigned int NodeSize> struct _Rb_tree_base {
    _Rb_tree_node_base* _M_header;
    _Rb_tree_base() : _M_header(0) { _M_header = (_Rb_tree_node_base*)malloc(NodeSize); }
};
template <unsigned int NodeSize> struct CRetailMap : _Rb_tree_base<NodeSize> {   // STLport map/_Rb_tree (12 bytes in retail)
    unsigned long _M_node_count;
    unsigned char _M_key_compare;
    CRetailMap() : _M_node_count(0) { _M_empty_initialize(); }
    void _M_empty_initialize()
    {
        this->_M_header->_M_color = 0;
        this->_M_header->_M_parent = 0;
        this->_M_header->_M_left = this->_M_header;
        this->_M_header->_M_right = this->_M_header;
    }
};
struct CScriptThing;
struct CMapNodeCharStringBool;
struct CCS_OakValeRevisitedScript : CQuestScriptObject {   // vtable 0x012F53C0
    CRetailVector<CScriptThing*> FirePoint;        // +0x48 (vector<CScriptThing>, 12 bytes)
    CRetailVector<CScriptThing*> Fires;            // +0x54
    CRetailMap<0x18> OakValeFlag;                  // +0x60 map<CCharString,bool>
    CScriptGameResourceObjectScriptedThingBase v1; // +0x6c
    CScriptGameResourceObjectScriptedThingBase v2; // +0x7c
    CScriptGameResourceObjectScriptedThingBase v3; // +0x8c
    CScriptGameResourceObjectScriptedThingBase v4; // +0x9c
    CScriptGameResourceObjectScriptedThingBase v5; // +0xac
    CScriptGameResourceObjectScriptedThingBase v6; // +0xbc
    CScriptGameResourceObjectScriptedThingBase v7; // +0xcc
    virtual void v00();
    CCS_OakValeRevisitedScript(void* data, CGameScriptInterface* game);
};
CCS_OakValeRevisitedScript::CCS_OakValeRevisitedScript(void* data, CGameScriptInterface* game)
    : CQuestScriptObject(data, game)
{
}