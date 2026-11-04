// NCreatureMode::CModeDragonStrafe::CModeDragonStrafe(CThing*, float, float, float, float) -- CModeDragonFlyingBase (empty flight spline, tension 0.5), the "STRAFE" group filter, strafe timing converted from seconds to frames
// Grade: RELOCATION_MATCH (retry 10, RTM 3077) -- the spline's CArray modelled with the STLport alloc-proxy end-of-storage member: the `push -1` of the filter name lands after the three zero stores as in retail
// Leaf: ??0CModeDragonStrafe
// Plan phase 3 (2026-11-04), CArray wave: the local CArray (an empty public std::vector<T>) and its STLport stand-ins replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>).
// Naming wave 18: the global stand-in g_framesPerSecond (0x01375550) is the protected static long CMainGameComponent::ConstantFPS, read through the
//   inline static accessor GetConstantFPS() as retail code outside the class does (name-only: the bytes are unchanged).
// Retail: Fable.exe (TLC, VC7.1) @ 0x008DDFB0, 200 bytes (`ret 14h`). Vtable 0x012686C8 (NCreatureMode::CModeDragonStrafe).
// Callees: CModeBase::CModeBase(CThing*) 0x0073EAC0; CCharString(const char*, long) 0x0099EBF0, CCharString() 0x0099E4B0,
// operator= 0x0099EFB0, ~CCharString 0x0099EAE0; CRC 0x004014A0; __ftol2 0x00BFEA70 (x3).
// Globals: the frames-per-second long 0x01375550 (`fild`).
// Layout (retail; PDB CModeDragonStrafe 0x5C sits 4 bytes higher because retail CArray is 0x0c): CModeBase 0x1c,
// CModeDragonFlyingBase: FlightSpline +0x1c (CArray control points +0x1c, Tension +0x28, EndPointsConnected +0x2c),
// SplinePercentageComplete +0x30, LegAnimAlpha +0x34; then Filter +0x38, Timer +0x44, BurninateStartTime +0x48,
// BurninateDuration +0x4c, Burninated +0x50, Speed +0x54.
// Shape: CModeDragonFlyingBase is `__declspec(novtable)` with an inline ctor (only the derived vptr is stored);
// the spline's inline ctor gives the three zero words from ebx, the 0.5 tension and the bool before the filter;
// the frame counts are body assignments (`fild fps ; fmul [arg]`), BurninateStartTime re-reading Timer.
class CThing;
class CThingCreatureBase;
class CBaseObject;

class CCharStringData { public:
    const char*   m_pData;                 // +0  (CBasicString<char>::PData)
    unsigned long m_length;                // +4  (StringLength)
};

class CCharString { public:
    CCharStringData* m_pData;
    CCharString();                         // 0x0099E4B0
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                        // 0x0099EAE0
    const CCharString& operator=(const CCharString& other);   // 0x0099EFB0
    unsigned long GetLength() const { return m_pData == 0 ? 0 : m_pData->m_length; }
    const char* GetPString() const { return m_pData == 0 ? "" : m_pData->m_pData; }
};

class CCRC {
public:
    static unsigned long __fastcall Calc(unsigned long, const void*, unsigned long);   // 0x004014A0
};   // 0x004014A0: CRC

class CAnimFilterOfGroup { public:                // vtable 0x01267568
    virtual void v00();
    CCharString   Group;                   // +4
    unsigned long GroupCRC;                // +8
    CAnimFilterOfGroup(const CCharString& group)
        : Group()
    {
        Group = group;
        GroupCRC = CCRC::Calc(0, group.GetPString(), group.GetLength());
    }
};

struct CBaseIntelligentPointer {           // 8 bytes: vptr + the pointed-to item
    virtual void v00();
    CBaseObject* PItem;                    // +4
    CBaseIntelligentPointer();             // 0x00A01B10
    void SetPItem(void* item);             // 0x00A01B90
};

struct CIntelligentPointerThing : CBaseIntelligentPointer {      // CIntelligentPointer<CThing>, vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointerThing() { }
    CIntelligentPointerThing(CThing* item) { SetPItem(item); }
};

struct CIntelligentPointerCreature : CBaseIntelligentPointer {   // CIntelligentPointer<CThingCreatureBase>, vtable 0x0123E7D8
    virtual void v00();
    CIntelligentPointerCreature() { }
};

template <class T> struct CAllocator {     // STLport allocator<T>: empty
    CAllocator() { }
};
template <class T> struct CAllocProxy : CAllocator<T> {    // STLport _STLP_alloc_proxy<T*, T, allocator<T> >
    T* m_data;
    CAllocProxy(const CAllocator<T>& alloc, T* p) : CAllocator<T>(alloc), m_data(p) { }
};
template <class T> struct CVectorBase {    // STLport _Vector_base<T, allocator<T> >
    T*             _M_start;
    T*             _M_finish;
    CAllocProxy<T> _M_end_of_storage;
    CVectorBase(const CAllocator<T>& alloc) : _M_start(0), _M_finish(0), _M_end_of_storage(alloc, 0) { }
};
template <class T> struct CVector : CVectorBase<T> {   // STLport vector<T> (12 bytes)
    CVector() : CVectorBase<T>(CAllocator<T>()) { }
};

class C3DVector { public: float X, Y, Z; };

struct CModeBase {                         // retail 0x1c: vptr, ParentThing, WeaponBasedFilter, FramesBeforeRemovingSelf, Active
    virtual void v00();
    unsigned char _body[0x18];
    CModeBase(CThing* thing);              // 0x0073EAC0
};

template <class T> struct CArrayAllocProxy {   // STLport _STLP_alloc_proxy: the end-of-storage pointer with its own constructor
    T* p;
    CArrayAllocProxy(T* v) : p(v) { }
};
#include "engine/CArray.h"

class CCardinalSpline { public:                   // retail 0x14
    CArray<C3DVector> ControlPoints;       // +0
    float             Tension;             // +0xc
    bool              EndPointsConnected;  // +0x10
    CCardinalSpline() : Tension(0.5f), EndPointsConnected(false) { }
};

class CMainGameComponent {
protected:
    static long ConstantFPS;                             // 0x01375550 ?ConstantFPS@CMainGameComponent@@1JA (protected static)
public:
    static long __fastcall GetConstantFPS() { return ConstantFPS; }   // Ego_r / FableWin ?GetConstantFPS@CMainGameComponent@@SIJXZ (inline)
};

namespace NCreatureMode {

struct __declspec(novtable) CModeDragonFlyingBase : CModeBase {   // vtable 0x01268658 (never stored)
    CCardinalSpline FlightSpline;              // +0x1c
    float           SplinePercentageComplete;  // +0x30
    float           LegAnimAlpha;              // +0x34
    CModeDragonFlyingBase(CThing* thing) : CModeBase(thing) { }
};

struct CModeDragonStrafe : CModeDragonFlyingBase {   // vtable 0x012686C8
    CAnimFilterOfGroup Filter;               // +0x38
    long               Timer;                // +0x44
    long               BurninateStartTime;   // +0x48
    long               BurninateDuration;    // +0x4c
    bool               Burninated;           // +0x50
    float              Speed;                // +0x54
    virtual void v00();
    CModeDragonStrafe(CThing* thing, float duration, float speed, float burninateBeforeEnd, float burninateDuration);
};

CModeDragonStrafe::CModeDragonStrafe(CThing* thing, float duration, float speed, float burninateBeforeEnd, float burninateDuration)
    : CModeDragonFlyingBase(thing)
    , Filter("STRAFE")
{
    Speed = speed;
    Timer = (long)(duration * CMainGameComponent::GetConstantFPS());
    Burninated = false;
    BurninateStartTime = (long)(Timer - burninateBeforeEnd * CMainGameComponent::GetConstantFPS());
    BurninateDuration = (long)burninateDuration;
}

}