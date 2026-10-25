// Behaviour test for 0x00699BF0: CGameCameraManager::~CGameCameraManager() -- the camera manager's destructor: clear the three camera-list iterators and the default camera-mode def, then destroy the debug-point list, the registered-mode map, the def pointers, both camera lists, the observed thing, the quake list, the script-module / mode counted pointers, the camera-point list and the tools' scan list
// A copy of the source model and logic (compiled separately from src/00699bf0.cpp) with test stand-ins for the callees.
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>
#if !defined(_MSC_VER)
#define __fastcall
#define __forceinline inline
#define __declspec(x)
#define __cdecl
#define __fastcall
#define __int64 long long
#endif

class CWorld;
class CThing;
class CThingGameObject;

class CDefVirtual {
public:
    virtual void v00();
    virtual void Destroy() const;                      // slot 1: runs when the last reference goes
};
class CRefCounted {                                    // second base: the count sits at +4 behind the vptr
public:
    mutable long RefCount;                             // +0x04
    void DecRef() const { --RefCount; }
    long GetRefCount() const { return RefCount; }
};
class CDefPointeeBase : public CDefVirtual, public CRefCounted {
public:
    void ReleaseTemporary() const                      // inlined into CDefPointer::Clear
    {
        DecRef();
        if (GetRefCount() == 0)
            Destroy();
    }
};
class CCameraModeDef : public CDefPointeeBase { };
class CCameraManagerDef : public CDefPointeeBase { };
class CCameraManagerSetDef : public CDefPointeeBase { };

template <class T> class CDefPointer {
public:
    T* Object;
    ~CDefPointer()
    {
        T* old = Object;                               // a local copy: the decrement goes through the register
        if (old) {
            if (--old->RefCount == 0)
                old->Destroy();
            Object = 0;
        }
    }
    void Clear()
    {
        if (Object)
            Object->ReleaseTemporary();
        Object = 0;
    }
};

extern CDefPointer<const CCameraModeDef> g_DefaultCameraModeDef;   // 0x013BA8D8

struct CCPPointerInfo {
    unsigned long RefCount;                            // +0x00
    void (__fastcall* DeleteFunc)(void* data);         // +0x04
    void*         Data;                                // +0x08
};

template <class T> class CCountedPointer {
public:
    T*              Data;                              // +0x00
    CCPPointerInfo* Info;                              // +0x04
    ~CCountedPointer()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
        Data = 0;
        Info = 0;
    }
};

class CBaseObjectPointer;
class CBaseIntelligentPointer {
public:
    virtual ~CBaseIntelligentPointer();                // 0x00A01C10
    CBaseObjectPointer* PData;                         // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { };

class C3DVector {
public:
    float X, Y, Z;
};
class CRGBColour {
public:
    unsigned long Colour;
};
class C2DVector {
public:
    float X, Y;
};

namespace NCameraModes {
enum EMode { MODE_NONE };
class CBase;
class CBaseInit;
class CScriptModuleBase;
}

namespace std {
template <class _Tp> class allocator {
public:
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

template <class _Tp> struct less { };

template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};

struct __false_type { __false_type() { } };

template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }

template <class _ForwardIterator>
void __fastcall __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}
template <>
void __fastcall __destroy_aux<CDefPointer<const CCameraModeDef>*>(CDefPointer<const CCameraModeDef>* __first,
    CDefPointer<const CCameraModeDef>* __last, const __false_type&);   // 0x0069D2B0

template <class _ForwardIterator>
inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    __destroy_aux(__first, __last, __false_type());
}

// list
struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;
};
template <class _Tp, class _Alloc> class _List_base {
public:
    typedef _List_node<_Tp> _Node;
    ~_List_base()
    {
        clear();
        _M_node.deallocate(_M_node._M_data, 1);
    }
    void clear();
    _STLP_alloc_proxy<_Node*, _Node, allocator<_Node> > _M_node;
};
template <> _List_base<pair<C3DVector, CRGBColour>, allocator<pair<C3DVector, CRGBColour> > >::~_List_base();   // 0x0069CDB0
template <> void _List_base<CIntelligentPointer<CThing>, allocator<CIntelligentPointer<CThing> > >::clear();       // 0x004B8050

template <class _Tp, class _Alloc = allocator<_Tp> > class list : public _List_base<_Tp, _Alloc> { };

// map
struct _Rb_tree_node_base {
    bool                _M_color;
    _Rb_tree_node_base* _M_parent;
    _Rb_tree_node_base* _M_left;
    _Rb_tree_node_base* _M_right;
};
template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;
};
template <class _Tp, class _Alloc> struct _Rb_tree_base {
    typedef _Rb_tree_node<_Tp> _Node;
    ~_Rb_tree_base() { _M_header.deallocate(_M_header._M_data, 1); }
    _STLP_alloc_proxy<_Node*, _Node, allocator<_Node> > _M_header;
};
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
class _Rb_tree : public _Rb_tree_base<_Value, _Alloc> {
public:
    typedef _Rb_tree_node<_Value> _Node;
    typedef _Node* _Link_type;
    unsigned int _M_node_count;
    _Compare _M_key_compare;
    _Link_type& _M_root() const { return (_Link_type&)this->_M_header._M_data->_M_parent; }
    _Link_type& _M_leftmost() const { return (_Link_type&)this->_M_header._M_data->_M_left; }
    _Link_type& _M_rightmost() const { return (_Link_type&)this->_M_header._M_data->_M_right; }
    void _M_erase(_Link_type __x);                     // 0x0069D3D0
    void clear()
    {
        if (_M_node_count != 0) {
            _M_erase(_M_root());
            _M_leftmost() = this->_M_header._M_data;
            _M_root() = 0;
            _M_rightmost() = this->_M_header._M_data;
            _M_node_count = 0;
        }
    }
    ~_Rb_tree() { clear(); }
};
template <class _Pair> struct _Select1st { };
template <class _Key, class _Tp, class _Compare = less<_Key>, class _Alloc = allocator<pair<const _Key, _Tp> > >
class map {
public:
    typedef pair<const _Key, _Tp> value_type;
    _Rb_tree<_Key, value_type, _Select1st<value_type>, _Compare, _Alloc> _M_t;
};

// vector
template <class _Tp, class _Alloc> class _Vector_base {
public:
    ~_Vector_base()
    {
        if (_M_start != 0)
            _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start);
    }
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* iterator;
    ~vector() { _Destroy(this->_M_start, this->_M_finish); }
};
}

class CGameCameraManager;

class CTools {                                         // retail 0x20 bytes
public:
    CGameCameraManager* CameraManager;                 // +0x00
    std::vector<CIntelligentPointer<const CThing> > ScanThings;   // +0x04
    float CameraMeshOffsetDistance;                    // +0x10
    float CameraCreatureOffsetDistance;                // +0x14
    float CameraLandscapeOffsetDistance;               // +0x18
    float CameraThemeOffsetDistance;                   // +0x1c
};

class CAIGameCameraBase {                              // vtable 0x0125C1F4
public:
    virtual ~CAIGameCameraBase() { }
};

class CGameCameraManager : public CAIGameCameraBase {  // vtable 0x0125C754
public:
    class CQuake {
    public:
        unsigned char _body[0x1c];
    };
    unsigned char Camera[0x38];                        // +0x04 CCamera
    unsigned char OldCamera[0x38];                     // +0x3c CCamera
    CWorld* World;                                     // +0x74
    CTools CamTools;                                   // +0x78
    CCountedPointer<NCameraModes::CBase> CurrentMode;  // +0x98
    CCountedPointer<NCameraModes::CBase> PendingMode;  // +0xa0
    bool NewAutoMode;                                  // +0xa8
    bool ObservedThingInMelee;
    bool ObservedThingInBuilding;
    std::list<CIntelligentPointer<CThing> > AvailableCamPoints;   // +0xac
    bool SingleCut;                                    // +0xb0
    bool ChangeInControlDirection;
    long InsideHeroCount;                              // +0xb4
    float ControlAngleXY;                              // +0xb8
    bool AngleChange;                                  // +0xbc
    bool ControlNoControlMovement;
    long ControlNoControlMovementStartFrame;           // +0xc0
    C2DVector RightStickLookAroundAngles;              // +0xc4
    C2DVector RightStickTargetLookAroundAngles;        // +0xcc
    CCountedPointer<NCameraModes::CScriptModuleBase> PCurrentScriptModule;   // +0xd4
    CCountedPointer<NCameraModes::CScriptModuleBase> PPendingScriptModule;   // +0xdc
    long ScriptedCameraRemoveTimer;                    // +0xe4
    bool CameraPreload;                                // +0xe8
    bool CameraPreloadUseFastMode;
    bool ScriptHasPreloadedCamera;
    std::vector<CQuake> EarthQuakes;                   // +0xec
    float MinWorldHeight;                              // +0xf8
    float MaxWorldHeight;                              // +0xfc
    CIntelligentPointer<const CThingGameObject> PObservedThing;   // +0x100
    std::vector<CDefPointer<const CCameraModeDef> > CameraList;    // +0x108
    std::vector<CDefPointer<const CCameraModeDef> >::iterator CameraListIndex;              // +0x114
    std::vector<CDefPointer<const CCameraModeDef> >::iterator InitialModeCameraListIndex;   // +0x118
    std::vector<CDefPointer<const CCameraModeDef> > SpecialCameraList;                      // +0x11c
    std::vector<CDefPointer<const CCameraModeDef> >::iterator SpecialCameraListIndex;       // +0x128
    bool UsingCameraManually;                          // +0x12c
    bool BlockModeChange;                              // +0x12d
    CDefPointer<const CCameraManagerSetDef> PCameraManagerSet;   // +0x130
    CDefPointer<const CCameraManagerDef> PDef;         // +0x134
    std::map<NCameraModes::EMode, CCountedPointer<NCameraModes::CBaseInit> > RegisteredModes;   // +0x138
    bool ScriptedCameraModeFromGame;                   // +0x144
    bool ScriptedCameraPreloadFlag;
    float CamRSLookAroundDeltaTheta;                   // +0x148
    float CamZoomMinHeightFactor;                      // +0x14c
    bool AutoFOVZoomInCombat;                          // +0x150
    bool CamAutoGoBehind;
    bool LimitMinMaxWorldHeight;
    long DrawDebugGraphics;                            // +0x154
    unsigned long DebugPersistFrame;                   // +0x158
    std::list<std::pair<C3DVector, CRGBColour> > DebugPoints;     // +0x15c
    virtual ~CGameCameraManager();
};

CGameCameraManager::~CGameCameraManager()
{
    CameraListIndex = 0;
    InitialModeCameraListIndex = 0;
    SpecialCameraListIndex = 0;
    g_DefaultCameraModeDef.Clear();
}

// ---- test stand-ins ----
static int g_destroyed = 0, g_pointerDtors = 0, g_deleteFuncs = 0, g_listNodes = 0, g_mapNodes = 0;
static const CDefVirtual* g_lastDestroyed = 0;
void CDefVirtual::v00() { }
void CDefVirtual::Destroy() const { ++g_destroyed; g_lastDestroyed = this; }
CDefPointer<const CCameraModeDef> g_DefaultCameraModeDef;
CBaseIntelligentPointer::~CBaseIntelligentPointer() { ++g_pointerDtors; }
class CThing { };
class CThingGameObject { };
namespace std {
template <>
void __fastcall __destroy_aux<CDefPointer<const CCameraModeDef>*>(CDefPointer<const CCameraModeDef>* __first,
    CDefPointer<const CCameraModeDef>* __last, const __false_type&)
{
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}
template <> _List_base<pair<C3DVector, CRGBColour>, allocator<pair<C3DVector, CRGBColour> > >::~_List_base()
{
    _List_node_base* __cur = _M_node._M_data->_M_next;
    while (__cur != _M_node._M_data) { _List_node_base* __next = __cur->_M_next; ++g_listNodes; std::free(__cur); __cur = __next; }
    std::free(_M_node._M_data);
}
template <> void _List_base<CIntelligentPointer<CThing>, allocator<CIntelligentPointer<CThing> > >::clear()
{
    _List_node_base* __cur = _M_node._M_data->_M_next;
    while (__cur != _M_node._M_data) {
        _List_node_base* __next = __cur->_M_next;
        _Destroy(&((_Node*)__cur)->_M_data);
        ++g_listNodes;
        std::free(__cur);
        __cur = __next;
    }
    _M_node._M_data->_M_next = _M_node._M_data->_M_prev = _M_node._M_data;
}
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
void _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::_M_erase(_Link_type __x)
{
    while (__x != 0) {
        _M_erase((_Link_type)__x->_M_right);
        _Link_type __y = (_Link_type)__x->_M_left;
        _Destroy(&__x->_M_value_field);
        ++g_mapNodes;
        std::free(__x);
        __x = __y;
    }
}
}
static void __fastcall DeleteData(void*) { ++g_deleteFuncs; }
template <class T> void Counted(CCountedPointer<T>& p, unsigned long refs)
{
    p.Data = 0;
    p.Info = 0;
    if (refs) { p.Info = new CCPPointerInfo; p.Info->RefCount = refs; p.Info->DeleteFunc = DeleteData; p.Info->Data = 0; }
}
template <class T> void Empty(std::vector<T>& v) { v._M_start = v._M_finish = v._M_end_of_storage._M_data = 0; }
template <class T> void Header(std::list<T>& l)
{
    l._M_node._M_data = (std::_List_node<T>*)std::malloc(sizeof(std::_List_node<T>));
    l._M_node._M_data->_M_next = l._M_node._M_data->_M_prev = l._M_node._M_data;
}
template <class T> std::_List_node<T>* Append(std::list<T>& l)
{
    std::_List_node<T>* n = (std::_List_node<T>*)std::malloc(sizeof(std::_List_node<T>));
    n->_M_next = l._M_node._M_data; n->_M_prev = l._M_node._M_data->_M_prev;
    l._M_node._M_data->_M_prev->_M_next = n; l._M_node._M_data->_M_prev = n;
    return n;
}

static int Fail(int line) { std::printf("00699bf0_TEST FAIL (line %d)\n", line); return 1; }
#define CHECK(c) if (!(c)) return Fail(__LINE__)

int main()
{
    CCameraModeDef modeA, modeB, defaultMode; CCameraManagerSetDef set; CCameraManagerDef def;
    modeA.RefCount = 2; modeB.RefCount = 1; defaultMode.RefCount = 1; set.RefCount = 1; def.RefCount = 3;
    g_DefaultCameraModeDef.Object = &defaultMode;
    CGameCameraManager* manager = new CGameCameraManager;      // PObservedThing is built by the implicit constructor
    Empty(manager->CamTools.ScanThings);
    manager->CamTools.ScanThings._M_start = (CIntelligentPointer<const CThing>*)std::malloc(2 * sizeof(CIntelligentPointer<const CThing>));
    new (&manager->CamTools.ScanThings._M_start[0]) CIntelligentPointer<const CThing>();
    manager->CamTools.ScanThings._M_finish = manager->CamTools.ScanThings._M_end_of_storage._M_data = manager->CamTools.ScanThings._M_start + 1;
    Counted(manager->CurrentMode, 1); Counted(manager->PendingMode, 0);
    Counted(manager->PCurrentScriptModule, 2); Counted(manager->PPendingScriptModule, 0);
    CCPPointerInfo* sharedScript = manager->PCurrentScriptModule.Info;
    Header(manager->AvailableCamPoints);
    new (&Append(manager->AvailableCamPoints)->_M_data) CIntelligentPointer<CThing>();
    new (&Append(manager->AvailableCamPoints)->_M_data) CIntelligentPointer<CThing>();
    Header(manager->DebugPoints); Append(manager->DebugPoints);
    Empty(manager->EarthQuakes);
    manager->EarthQuakes._M_start = (CGameCameraManager::CQuake*)std::malloc(2 * sizeof(CGameCameraManager::CQuake));
    manager->EarthQuakes._M_finish = manager->EarthQuakes._M_end_of_storage._M_data = manager->EarthQuakes._M_start + 2;
    Empty(manager->CameraList); Empty(manager->SpecialCameraList);
    manager->CameraList._M_start = (CDefPointer<const CCameraModeDef>*)std::malloc(4 * sizeof(CDefPointer<const CCameraModeDef>));
    manager->CameraList._M_start[0].Object = &modeA; manager->CameraList._M_start[1].Object = &modeB;
    manager->CameraList._M_finish = manager->CameraList._M_start + 2; manager->CameraList._M_end_of_storage._M_data = manager->CameraList._M_start + 4;
    manager->CameraListIndex = manager->CameraList._M_start + 1; manager->InitialModeCameraListIndex = manager->CameraList._M_start;
    manager->SpecialCameraListIndex = manager->CameraList._M_start;
    manager->PCameraManagerSet.Object = &set; manager->PDef.Object = &def;
    std::_Rb_tree_node<std::pair<const NCameraModes::EMode, CCountedPointer<NCameraModes::CBaseInit> > >* header =
        (std::_Rb_tree_node<std::pair<const NCameraModes::EMode, CCountedPointer<NCameraModes::CBaseInit> > >*)std::malloc(sizeof(*header));
    std::_Rb_tree_node<std::pair<const NCameraModes::EMode, CCountedPointer<NCameraModes::CBaseInit> > >* node =
        (std::_Rb_tree_node<std::pair<const NCameraModes::EMode, CCountedPointer<NCameraModes::CBaseInit> > >*)std::malloc(sizeof(*node));
    Counted(node->_M_value_field.second, 1);
    node->_M_parent = header; node->_M_left = node->_M_right = 0;
    header->_M_parent = node; header->_M_left = header->_M_right = node;
    manager->RegisteredModes._M_t._M_header._M_data = header; manager->RegisteredModes._M_t._M_node_count = 1;

    CAIGameCameraBase* base = manager;
    delete base;                                               // virtual destructor
    CHECK(g_DefaultCameraModeDef.Object == 0 && defaultMode.RefCount == 0);
    CHECK(modeA.RefCount == 1 && modeB.RefCount == 0 && set.RefCount == 0 && def.RefCount == 2);
    CHECK(g_destroyed == 3);                                   // modeB, set and the default camera mode def
    CHECK(g_deleteFuncs == 2 && sharedScript->RefCount == 1);  // CurrentMode and the registered mode's init
    CHECK(g_listNodes == 3 && g_mapNodes == 1);
    CHECK(g_pointerDtors == 1 + 2 + 1);                        // the scan thing, two camera points, the observed thing
    delete sharedScript;
    std::printf("00699bf0_TEST PASS\n");
    return 0;
}