#pragma optimize("s", on)
// CFrontEndManager::~CFrontEndManager() -- front-end manager teardown: drop the virtual keyboard, delete every registered screen component, destroy the UI manager singleton and clear the front-end singleton pointer, then the members (+0xC4 down to +0x14) and the CObserver base
// Grade: DIFFER(213v213) -- scheduling only: in the inlined CObserver base destructor retail pops edi after the base vtable store and `lea ecx,[esi+4]`, ours before the store
// Leaf: ??1CFrontEndManager
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005954AD, length 213 bytes (called only by the deleting destructor 0x0059B641,
// vtable 0x012521A8 slot 0 = NUISystem::CFrontEndManager by RTTI). Size-optimised TU: `#pragma optimize("s", on)`.
// Label correction: the manifest's "CEngineScreenEffectDisplacementRenderer::~CEngineScreenEffectDisplacementRenderer" is a
// propagation; the vtable store and the PDB CFrontEndManager members name the class.
// Callees: CFrontEndManager::DestroyVirtualKeyboard(bool) 0x00594FA9 (deletes +0x60; manifest label is propagated),
// TreeNext_004292c0 (the STLport tree successor, __cdecl), NUISystem::CManager::DeleteInstance() 0x0042FB40 (deletes the UI
// manager singleton 0x013B8710), member destructors: map<CWideString,bool> 0x00485E45, ~CWideString 0x0099B510,
// vector<CCharString> 0x00414EA0, map<FRONTEND_SCREEN,CComponent*> 0x0059B098, the screen stack's deque 0x0059AC77,
// list<CActionParam*> 0x0042AB1C, the CObserver member 0x0052E050 (tail jump); free 0x00BFEA14 for the three plain vectors.
// Globals: the front-end manager singleton pointer 0x013B8B5C (cleared).
// Layout (retail = PDB less 4: the 0x14-byte CObserver base): MediaPlayerErrorString +0x14, UIActionParams +0x18,
// PGameComponent +0x1C, m_stackScreens +0x20 (STLport deque, 0x28), m_vComponents +0x48, m_mapScreens +0x54,
// m_pVirtualKeyboard +0x60, m_vFileTimes +0x68, FileRegionNames +0x74, FileRegionMiniMapNames +0x80, FilePlayTimes +0x8C,
// GameToLoad +0xAC, ProfileToDelete +0xB0, Signatures +0xC4.
// Shape: the screen map walk keeps the header pointer's address in ebx and the node in edi; `delete it->second` is the
// null test + virtual deleting destructor (slot 0, flag 1); the plain vectors' inline destructors free their storage.
#include <stdlib.h>

struct SNode {                                  // STLport _Rb_tree_node<pair<const FRONTEND_SCREEN, CComponent*> >
    long   Color;
    SNode* Parent;
    SNode* Left;
    SNode* Right;
    long   Key;                                 // +0x10
    struct CComponentBase* Value;               // +0x14
};
SNode* __cdecl TreeNext_004292c0(SNode* node);  // 0x004292C0

struct CComponentBase {                         // NUISystem::CComponent
    virtual ~CComponentBase();
};

struct CWideString {
    void* m_pData;
    ~CWideString();                             // 0x0099B510
};
struct CActionParamList {                       // list<NUISystem::CActionParam*>
    void* m_node;
    ~CActionParamList();                        // 0x0042AB1C
};
struct CComponentStack {                        // stack<CComponent*, deque<CComponent*> >
    unsigned char _body[0x28];
    ~CComponentStack();                         // 0x0059AC77
};
template <class T> struct CPlainVector {        // STLport vector over malloc'ed storage
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    ~CPlainVector() { if (_M_start) free(_M_start); }
};
struct CScreenMap {                             // map<FRONTEND_SCREEN, CComponent*>
    SNode* _M_header;                           // +0x00
    long   _M_node_count;                       // +0x04
    long   _M_key_compare;                      // +0x08 (the empty less<> padded to a dword)
    ~CScreenMap();                              // 0x0059B098
};
struct CCharStringVector {
    unsigned char _body[0xc];
    ~CCharStringVector();                       // 0x00414EA0
};
struct CSignatureMap {                          // map<CWideString, bool>
    unsigned char _body[8];
    ~CSignatureMap();                           // 0x00485E45
};
struct CDateAndTime { long Year; unsigned long Month, Day, Hour, Minute, Second, Millisecond; };

namespace NUISystem {
struct CObserverMember_0052e050 {
    unsigned char _body[0x10];
    ~CObserverMember_0052e050();                // 0x0052E050
};
struct __declspec(novtable) CObserverRoot {
    virtual void v00();
};
struct CObserver : CObserverRoot {              // vtable 0x01246020
    CObserverMember_0052e050 m_member4;         // +0x04
    __forceinline virtual ~CObserver() { }
};
struct CManager {
    static void DeleteInstance();               // 0x0042FB40
};
}

class CFrontEndManager : public NUISystem::CObserver {   // vtable 0x012521A8
public:
    CWideString                   MediaPlayerErrorString;   // +0x14
    CActionParamList              UIActionParams;           // +0x18
    void*                         PGameComponent;           // +0x1c
    CComponentStack               m_stackScreens;           // +0x20
    CPlainVector<CComponentBase*> m_vComponents;            // +0x48
    CScreenMap                    m_mapScreens;             // +0x54
    void*                         m_pVirtualKeyboard;       // +0x60
    bool                          m_bStartNewGame;          // +0x64
    CPlainVector<CDateAndTime>    m_vFileTimes;             // +0x68
    CCharStringVector             FileRegionNames;          // +0x74
    CCharStringVector             FileRegionMiniMapNames;   // +0x80
    CPlainVector<long>            FilePlayTimes;            // +0x8c
    unsigned char                 _pad98[0xac - 0x98];
    CWideString                   GameToLoad;               // +0xac
    CWideString                   ProfileToDelete;          // +0xb0
    unsigned char                 _padb4[0xc4 - 0xb4];
    CSignatureMap                 Signatures;               // +0xc4
    void DestroyVirtualKeyboard(bool reactivate);           // 0x00594FA9
    virtual ~CFrontEndManager();
};

extern CFrontEndManager* g_pFrontEndManager;    // 0x013B8B5C

CFrontEndManager::~CFrontEndManager()
{
    DestroyVirtualKeyboard(false);
    for (SNode* node = m_mapScreens._M_header->Left; node != m_mapScreens._M_header; node = TreeNext_004292c0(node))
        delete node->Value;
    NUISystem::CManager::DeleteInstance();
    g_pFrontEndManager = 0;
}
