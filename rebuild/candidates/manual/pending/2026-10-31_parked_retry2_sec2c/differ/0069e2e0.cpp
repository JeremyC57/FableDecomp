// CGameCameraManager::SetupCameraMode() -- build the string-and-cage camera mode from the first available camera
// Grade: DIFFER(237v237) 12 diff lines (parked 231v237/14): inline bool getter GetFlag4F() gives retail's byte store/reload push; left: frame 0x10 v 0xc (retail packs the uninitialised GetTC result into the key slot) (retry wave 2 sec2c)
// Was: DIFFER(ours v retail) -- 231v237: the bool ctor argument -- retail loads the byte early (mov dl), parks it in the dead key frame word and pushes that dword; our build pushes it as xor ecx,ecx / mov cl; the following scratch registers rotate (named float/bool locals hoist above operator new instead)
// point's camera-point TC (0x3F) settings and make it the current mode
// Leaf: SetupCameraMode
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0069E2E0, length 233 bytes (padded to 240), `ret`.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50, the TC map lower bound copy @ 0x004365B0, operator new
// @ 0x00BFEA1A, NCameraModes::CStringAndCage::CStringAndCage(CGameCameraManager&, const CDefPointer<const
// CCameraModeDef>&, float, float, float, bool, float, float) @ 0x0069DE30 (0xD0 bytes), CCountedPointer::Reset
// @ 0x0069C2F0 (the landed body, defined above under auto_inline(off)), SetCameraMode(CCountedPointer<CBase>)
// @ 0x00697E20, operator delete @ 0x00BFE9BC. Globals: the default camera mode def 0x013BA8D8, the float 1/360
// @ 0x01238E00.
// Retail layout: CGameCameraManager AvailableCamPoints +0xAC (STLport list header pointer; front node data +8);
// TC 0x3F (camera point): +0x10 angle in degrees, +0x40/+0x44/+0x48 three floats, +0x4F bool, +0x50 float.
// Shape: the TC comes from the unchecked inline accessor (`T* tc; if (HasTC) tc = GetTC; return tc;`, the absent path
// reading the unset local from the lookup key's frame word); the mode is the landed 0x0069E3D0 counted-pointer model
// (default local, Reset through operator=, by-value copy into SetCameraMode, cached-block release).

void __fastcall Destroy_0056c0f0(void* object);   // 0x0056C0F0
struct SCountBlock {
    long m_references;                      // +0
    void (__fastcall* m_pfnDestroy)(void*); // +4
    void* m_pObject;                        // +8
    SCountBlock(void* object) : m_references(1), m_pfnDestroy(Destroy_0056c0f0), m_pObject(object) { }
};
template <class T> struct CCountedPointer {
    T* m_pObject;                           // +0
    SCountBlock* m_pCount;                  // +4
    CCountedPointer() : m_pObject(0), m_pCount(0) { }
    CCountedPointer(const CCountedPointer& other) : m_pObject(other.m_pObject), m_pCount(other.m_pCount)
    {
        if (m_pCount)
            ++m_pCount->m_references;
    }
    CCountedPointer& operator=(T* object) { Reset(object); return *this; }
    void Reset(T* object);                  // 0x0069C2F0 (defined below, earlier in the TU)
    ~CCountedPointer()
    {
        if (m_pCount && --m_pCount->m_references == 0) {
            m_pCount->m_pfnDestroy(m_pCount->m_pObject);
            operator delete(m_pCount);
        }
    }
};
#pragma auto_inline(off)
template <class T> void CCountedPointer<T>::Reset(T* object)
{
    if (m_pCount) {
        --m_pCount->m_references;
        if (m_pCount->m_references == 0) {
            m_pCount->m_pfnDestroy(m_pCount->m_pObject);
            delete m_pCount;
        }
    }
    m_pCount = 0;
    m_pObject = object;
    if (object)
        m_pCount = new SCountBlock(m_pObject);
}
#pragma auto_inline(on)

struct CThing {
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];       // +0x20
    struct CInterfaceMap {
        struct SEntry { long type; void* component; };
        SEntry* first;
        SEntry* last;
        SEntry* LowerBound(const long* type);       // 0x004365B0
    } m_interfaces;                       // +0x44
    template<class T> __forceinline T* GetTC(long interfaceType)
    {
        T* tc;
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            tc = (T*)it->component;
        }
        return tc;
    }
};

struct CBaseObject;
struct CBaseIntelligentPointer {
    void* __vftable;
    void* Info;
    CBaseObject* GetPItem() const;                  // 0x00A01B50
};
template<class T> struct CIntelligentPointer : CBaseIntelligentPointer {
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
};
template<class T> struct _List_node {
    _List_node* _M_next;
    _List_node* _M_prev;
    T _M_data;
};
template<class T> struct _List_iterator {
    _List_node<T>* _M_node;
    _List_iterator(_List_node<T>* n) : _M_node(n) {}
    T& operator*() const { return _M_node->_M_data; }
};
template<class T> struct CRetailList {
    _List_node<T>* _M_node;
    _List_iterator<T> begin() { return _List_iterator<T>(_M_node->_M_next); }
    T& front() { return *begin(); }
};

struct CTCCameraPoint_3F {                          // TC 0x3F
    unsigned char _pad00[0x10];
    float AngleDegrees;                             // +0x10
    unsigned char _pad14[0x2c];
    float Param40;                                  // +0x40
    float Param44;                                  // +0x44
    float Param48;                                  // +0x48
    unsigned char _pad4c[3];
    bool Flag4F;                                    // +0x4F
    float Param50;                                  // +0x50
    bool GetFlag4F() const { return Flag4F; }
};

struct CCameraModeDef;
template <class T> struct CDefPointer { T* Object; };
extern CDefPointer<const CCameraModeDef> g_CameraModeDef;   // 0x013BA8D8

class CGameCameraManager;
namespace NCameraModes {
struct CBase { virtual ~CBase(); };
struct CStringAndCage : CBase {
    CStringAndCage(CGameCameraManager& manager, const CDefPointer<const CCameraModeDef>& def, float a, float b, float c,
                   bool flag, float d, float angle);   // 0x0069DE30
    unsigned char _body[0xcc];
};
}

class CGameCameraManager {
public:
    unsigned char _pad[0xac];
    CRetailList<CIntelligentPointer<CThing> > AvailableCamPoints;   // +0xAC
    void SetCameraMode(CCountedPointer<NCameraModes::CBase> mode);   // 0x00697E20
    void SetupCameraMode();
};

void CGameCameraManager::SetupCameraMode()
{
    CTCCameraPoint_3F* point = AvailableCamPoints.front().GetPItem()->GetTC<CTCCameraPoint_3F>(0x3f);
    CCountedPointer<NCameraModes::CBase> mode;
    mode = new NCameraModes::CStringAndCage(*this, g_CameraModeDef, point->Param40, point->Param44, point->Param48,
                                            point->GetFlag4F(), point->Param50, point->AngleDegrees * (1.0f / 360.0f));
    SetCameraMode(mode);
}
