// CGameCameraManager::~CGameCameraManager() -- the camera manager's destructor: clear the three camera-list iterators and the default camera-mode def, then destroy the debug-point list, the registered-mode map, the def pointers, both camera lists, the observed thing, the quake list, the script-module / mode counted pointers, the camera-point list and the tools' scan list
// Leaf: ??1CGameCameraManager@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00699BF0, 624 bytes (610 without padding), thiscall `ret`. Vtables: CGameCameraManager
//   0x0125C754 (rtti-vtables.tsv), CAIGameCameraBase 0x0125C1F4 (the inline base dtor's store at the end).
// Callees: _List_base<pair<C3DVector, CRGBColour> >::~_List_base 0x0069CDB0 (DebugPoints, out of line), _Rb_tree::_M_erase
//   0x0069D3D0 (RegisteredModes), __destroy_aux(CDefPointer<const CCameraModeDef>*, .., const __false_type&) 0x0069D2B0
//   (__fastcall, SpecialCameraList and CameraList), CBaseIntelligentPointer::~CBaseIntelligentPointer 0x00A01C10
//   (PObservedThing), _List_base<CIntelligentPointer<CThing> >::clear 0x004B8050 (AvailableCamPoints), free 0x00BFEA14,
//   operator delete 0x00BFE9BC (CCPPointerInfo blocks), the CCPPointerInfo::DeleteFunc pointer (__fastcall), virtual
//   CDefPointeeBase slot 1 (Destroy) and the CIntelligentPointer virtual destructor (ScanThings elements).
// Global: the default camera-mode def pointer 0x013BA8D8 (the CDefPointer<const CCameraModeDef> the landed PushLookAround
//   0x00699A50 / 0x0069E3D0 pass to the mode constructors).
// Label: catalog "CGameCameraManager_Destructor" -- CGameCameraManager::~CGameCameraManager (non-deleting).
// Layout: retail is 0x1c bytes narrower than Ego_r.pdb (retail CTools 0x20, STLport vector 12, list 4, map 12) -- as the
//   landed CGameCameraManager rows (PDef +0x134, UsingCameraManually +0x12c, PObservedThing +0x100); PDB member names.
// Shape: the body's three iterator stores and the default-def release through the pointee's inline ReleaseTemporary()
//   over the ref-count base's two inline accessors (DecRef, then GetRefCount: the second-base pointer materialised once,
//   `add eax,4`, the decremented value forwarded to the test, as the landed 0x0083E0D0 family), the null store after it; then the implicit member-wise destruction
//   over STLport 4.6 containers (the member def pointers release through a local copy: `dec [ecx+4] ; jne`).
extern "C" void __cdecl free(void* p);                 // 0x00BFEA14
void __cdecl operator delete(void* p);                 // 0x00BFE9BC

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