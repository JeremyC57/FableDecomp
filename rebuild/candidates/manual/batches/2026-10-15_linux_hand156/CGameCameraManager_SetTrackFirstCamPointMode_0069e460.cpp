// CGameCameraManager::SetTrackFirstCamPointMode() -- switch the camera to a new CTrack mode on the first available camera point
// Leaf: SetTrackFirstCamPointMode
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0069E460, length 145 bytes.
// NOT CSACCombat::SetUpPhysics (propagated label): `this` is the camera manager -- the body calls
// CGameCameraManager::SetCameraMode @ 0x00697E20 on it. The front node of AvailableCamPoints (STLport
// list<CIntelligentPointer<CThing> > whose head-node pointer retail keeps at +0xAC, PDB +0xB0) gives the point
// thing through CBaseIntelligentPointer::GetPItem @ 0x00A01B50; then `new` (0x00BFEA1A) 0xCC bytes for
// NCameraModes::CTrack, constructed by 0x0069DF80 (vtable 0x0125C50C) with (this, the camera-mode def pointer
// 0x013BA8D8, point). Function name is descriptive (PDB name not recovered).
// Shape: `SetCameraMode(CCountedPointer<CBase>(new X(...)))`: the counted pointer temporary is built straight in
// the argument area (`sub esp,8 ; mov esi,esp`) by the inline raw-pointer ctor (a null object stores a null count
// block; else a 12-byte block {1, destroy 0x0056C0F0, object}, a null block if that new fails); the ctor
// tests and re-reads its Data member (if/else form) so VC tail-duplicates the call into three exits. The
// front node goes through an STLport-style iterator (`*begin()` -> `lea ecx,[node+8]`).
void* operator new(unsigned int size);              // 0x00BFEA1A
void __fastcall Destroy_0056c0f0(void* object);     // 0x0056C0F0

struct CBaseObject;
struct CThing;
struct CCameraModeDef;
template <class T> struct CDefPointer { T* Data; };
extern CDefPointer<const CCameraModeDef> g_CameraModeDef;   // 0x013BA8D8

struct CBaseIntelligentPointer {
    virtual ~CBaseIntelligentPointer();
    CBaseObject* GetPItem() const;                  // 0x00A01B50
    CBaseObject* Item;
};
template <class T> struct CIntelligentPointer : CBaseIntelligentPointer {
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
};

struct _List_node_base {                            // STLport list node links
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};
template <class T> struct _List_node : _List_node_base {
    T _M_data;                                      // +0x08
};
template <class T> struct _List_iterator {
    _List_node_base* _M_node;
    _List_iterator(_List_node_base* n) : _M_node(n) {}
    T& operator*() const { return ((_List_node<T>*)_M_node)->_M_data; }
};
template <class T> struct CList {                   // STLport list: the head node pointer
    _List_node<T>* _M_node;
    _List_iterator<T> begin() { return _List_iterator<T>(_M_node->_M_next); }
    T& front() { return *begin(); }
};

struct SCountBlock {
    long References;                                // +0
    void (__fastcall* Destroy)(void*);              // +4
    void* Object;                                   // +8
    SCountBlock(void* object) : References(1), Destroy(Destroy_0056c0f0), Object(object) {}
};

template <class T> struct CCountedPointer {
    T* Data;
    SCountBlock* Info;
    CCountedPointer(T* p) : Data(p)
    {
        if (Data)
            Info = new SCountBlock(Data);
        else
            Info = 0;
    }
    CCountedPointer(const CCountedPointer& other);
};

class CGameCameraManager;
namespace NCameraModes {
    struct CBase { virtual ~CBase(); };
    struct CTrack : CBase {
        CTrack(CGameCameraManager* manager, const CDefPointer<const CCameraModeDef>& def, CThing* point);   // 0x0069DF80
        unsigned char _body[0xCC - 4];
    };
}

class CGameCameraManager {
public:
    void SetCameraMode(CCountedPointer<NCameraModes::CBase> mode);   // 0x00697E20
    void SetTrackFirstCamPointMode();
private:
    unsigned char _pad[0xAC];
    CList<CIntelligentPointer<CThing> > AvailableCamPoints;          // +0xAC
};

void CGameCameraManager::SetTrackFirstCamPointMode()
{
    CThing* point = AvailableCamPoints.front().GetPItem();
    SetCameraMode(CCountedPointer<NCameraModes::CBase>(new NCameraModes::CTrack(this, g_CameraModeDef, point)));
}
