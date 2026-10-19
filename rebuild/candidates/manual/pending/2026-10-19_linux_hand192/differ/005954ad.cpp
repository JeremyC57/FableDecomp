// Grade: DIFFER(213v213) -- scheduling only: retail pops edi after the CObserver base vtable store and the `lea ecx,[esi+4]` (right before the tail jump), ours pops it before the store; every other instruction matches (tried a map pointer local and direct member access)
// NUISystem::CFrontEndManager::~CFrontEndManager() -- destructor: shut the screens down (without going back), delete every registered screen object, destroy the global helper object and clear the front-end manager singleton, then the members and the CObserver base
// Leaf: ??1CFrontEndManager
// Retail: Fable.exe (TLC, VC7.1) @ 0x005954AD, 213 bytes (size-optimised module); called from the deleting destructor 0x0059B644.
// Label correction: the catalog's "CEngineScreenEffectDisplacementRenderer::~CEngineScreenEffectDisplacementRenderer" is
//   propagated; the vtable stored first is 0x012521A8 = NUISystem::CFrontEndManager and the base vtable stored last 0x01246020
//   = NUISystem::CObserver (rtti-vtables.tsv); Shutdown 0x00594FA9 is the landed CFrontEndManager member (keyboard at +0x60).
// Callees: CFrontEndManager::Shutdown(bool) 0x00594FA9, STLport _Rb_global<bool>::_M_increment 0x004292C0 (`__cdecl`), the
//   screens' virtual scalar deleting dtors (slot 0 with 1), the global-object teardown 0x0042FB40 (deletes the object at
//   0x013B8710), members: 0x00485E45 (+0xc4), ~CWideString 0x0099B510 (+0xb0, +0xac, +0x14), free 0x00BFEA14 (POD vectors
//   +0x8c, +0x68, +0x48), vector<CCharString>::~vector 0x00414EA0 (+0x80, +0x74), the screen map's dtor 0x0059B098 (+0x54),
//   the pointer array's FreeBuffer 0x0059AC77 (+0x20), list dtor 0x0042AB1C (+0x18), and the CObserver base's set dtor
//   0x0052E050 (+4, tail jump). Global: the front-end manager singleton 0x013B8B5C (cleared with `and [g],0`).
// Layout: retail offsets above (descriptive member names; Ego_r.pdb's CFrontEndManager containers are wider); the screen
//   map's nodes hold {key +0x10, screen +0x14}.
// Shape: `#pragma optimize("s", on)` (pop ecx, `and [g],0`); the map walk keeps a pointer to the map and re-reads its header
//   as end() each pass; `delete node->value` (null test + virtual deleting dtor); member dtors in reverse order; the
//   CObserver base's inline dtor stores its vtable and tail-jumps its member's dtor.
#pragma optimize("s", on)
extern "C" void __cdecl free(void* block);             // 0x00BFEA14

struct CWideString {
    void* PStringData;
    ~CWideString();                                    // 0x0099B510
};
struct CCharStringVector {                             // vector<CCharString>
    void* _M_words[3];
    ~CCharStringVector();                              // 0x00414EA0
};
template <class T> struct SPodVector {                 // STLport vector of a POD element (12 bytes)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    ~SPodVector()
    {
        if (_M_start)
            free(_M_start);
    }
};
struct CScreenHistoryList {                            // list (out-of-line dtor)
    void* _M_node;
    ~CScreenHistoryList();                             // 0x0042AB1C
};
struct CPointerArray_0059ac77 {
    void* _M_words[3];
    void FreeBuffer();                                 // 0x0059AC77
    ~CPointerArray_0059ac77() { FreeBuffer(); }
};
struct CMemberAtC4 {
    void* _M_node;
    ~CMemberAtC4();                                    // 0x00485E45
};

namespace NUISystem {

struct CScreen {
    virtual ~CScreen();
};

struct SScreenNode {                                   // _Rb_tree_node<pair<const long, CScreen*>>
    long         _M_color;
    SScreenNode* _M_parent;
    SScreenNode* _M_left;
    SScreenNode* _M_right;
    long         first;                                // +0x10
    CScreen*     second;                               // +0x14
};
struct CScreenMap {                                    // map<long, CScreen*> (retail 12 bytes)
    SScreenNode*  _M_header;
    unsigned long _M_node_count;
    unsigned long _M_key_compare;
    ~CScreenMap();                                     // 0x0059B098
};

struct CObservableSet {                                // set<CObservable*> (12 bytes)
    void* _M_words[3];
    ~CObservableSet();                                 // 0x0052E050
};

struct CObserver {                                     // vtable 0x01246020
    CObservableSet Observables;                        // +0x04
    virtual ~CObserver() { }
};

}

struct _Rb_global {
    static NUISystem::SScreenNode* __cdecl _M_increment(NUISystem::SScreenNode* node);   // 0x004292C0
};
void __cdecl DestroyGlobalObject_013b8710();           // 0x0042FB40

namespace NUISystem {

struct CFrontEndManager;
extern CFrontEndManager* g_pFrontEndManager;           // 0x013B8B5C

struct CFrontEndManager : CObserver {                  // vtable 0x012521A8
    unsigned char          _pad10[4];                  // +0x10
    CWideString            Title;                      // +0x14
    CScreenHistoryList     ScreenHistory;              // +0x18
    unsigned char          _pad1c[4];
    CPointerArray_0059ac77 OwnedObjects;               // +0x20
    unsigned char          _pad2c[0x48 - 0x2c];
    SPodVector<long>       Vector48;                   // +0x48
    CScreenMap             Screens;                    // +0x54
    void*                  Keyboard;                   // +0x60
    unsigned char          _pad64[4];
    SPodVector<long>       Vector68;                   // +0x68
    CCharStringVector      Strings74;                  // +0x74
    CCharStringVector      Strings80;                  // +0x80
    SPodVector<long>       Vector8c;                   // +0x8c
    unsigned char          _pad98[0xac - 0x98];
    CWideString            TextAC;                     // +0xac
    CWideString            TextB0;                     // +0xb0
    unsigned char          _padb4[0xc4 - 0xb4];
    CMemberAtC4            MemberC4;                   // +0xc4
    void Shutdown(bool goBack);                        // 0x00594FA9
    virtual ~CFrontEndManager();
};

CFrontEndManager::~CFrontEndManager()
{
    Shutdown(false);
    CScreenMap* screens = &Screens;
    for (SScreenNode* node = screens->_M_header->_M_left; node != screens->_M_header; node = _Rb_global::_M_increment(node))
        delete node->second;
    DestroyGlobalObject_013b8710();
    g_pFrontEndManager = 0;
}

}
