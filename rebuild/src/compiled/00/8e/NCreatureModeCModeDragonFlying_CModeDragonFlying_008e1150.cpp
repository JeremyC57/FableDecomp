// NCreatureMode::CModeDragonFlying::CModeDragonFlying(CThing& thing, std::vector<C3DVector>& points, const C3DVector& position, float speed, CCountedPointer<CCreatureActionBase> endAction) -- the dragon's flight mode: an empty cardinal flight spline (tension 0.5), the "FLYING" group filter, speed and the end action; the spline gets the current position followed by every flight point, the destination is the last point
// Leaf: ??0CModeDragonFlying
// Retail: Fable.exe (TLC, VC7.1) @ 0x008E1150, 548 bytes (`ret 18h`). Vtable 0x0126868C (NCreatureMode::CModeDragonFlying);
//   the inline CModeDragonFlyingBase vtable store (0x01268658) is dead and dropped.
// Callees: CModeBase::CModeBase 0x0073EAC0, CCharString(const char*, long) 0x0099EBF0 / CCharString() 0x0099E4B0 /
//   operator= 0x0099EFB0 / ~CCharString 0x0099EAE0, CRC Calc 0x004014A0 (__fastcall), vector<C3DVector>::resize(n, const T&)
//   0x0075C040 (catalog label "vector<CBrainBehaviour>::resize" is propagated), operator delete 0x00BFE9BC.
// Layout (retail = Ego_r.pdb minus the 4 bytes of the STLport vector): CModeBase 0x1c; CModeDragonFlyingBase FlightSpline
//   (CCardinalSpline: ControlPoints +0x1c, Tension +0x28, EndPointsConnected +0x2c), SplinePercentageComplete +0x30,
//   LegAnimAlpha +0x34; Filter +0x38, Destination +0x44, StartPos +0x50, PreviousPos +0x5c, DistanceToTravel +0x68, Speed +0x6c,
//   StartSpeed +0x70, MovementDuringAnimationLoop +0x74, Acceleration +0x78, AnimLength +0x7c, PEndAction +0x80,
//   NumAnimLoops +0x88, NumAnimLoopsToDo +0x8c, FirstFrame +0x90, UsePreviousPos +0x91. The filter model is the landed
//   CModeSummonerLightningOrb ctor 0x008CC180.
class CThing;
class CCharStringData { public:
    const char*   m_pData;
    unsigned long m_length;
};
class CCharString { public:
    CCharStringData* m_pData;
    CCharString();                         // 0x0099E4B0
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                        // 0x0099EAE0
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
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
void operator delete(void* p);             // 0x00BFE9BC
class CCPPointerInfo { public:                    // CCountedPointer's shared block
    long RefCount;
    void (__fastcall* DeleteFunc)(void* data);
    void* Data;
};
template <class T> class CCountedPointer { public:
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer() : Data(0), Info(0) { }
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
    CCountedPointer& operator=(const CCountedPointer& other)
    {
        if (Info != other.Info) {
            if (Info) {
                --Info->RefCount;
                if (Info->RefCount == 0) {
                    Info->DeleteFunc(Info->Data);
                    delete Info;
                }
            }
            Data = other.Data;
            Info = other.Info;
            if (Info)
                ++Info->RefCount;
        }
        return *this;
    }
};
class CCreatureActionBase;
class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
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
struct C3DVectorVector : CVectorBase<C3DVector> {   // STLport vector<C3DVector> (12 bytes)
    C3DVectorVector() : CVectorBase<C3DVector>(CAllocator<C3DVector>()) { }
    unsigned long size() const { return _M_finish - _M_start; }
    C3DVector* begin() { return _M_start; }
    C3DVector& operator[](unsigned long n) { return *(begin() + n); }
    void resize(unsigned long n, const C3DVector& x);   // 0x0075C040
};
class CCardinalSpline { public:
    C3DVectorVector ControlPoints;         // +0x00
    float           Tension;               // +0x0c
    bool            EndPointsConnected;    // +0x10
    CCardinalSpline() : Tension(0.5f), EndPointsConnected(false) { }
};
struct CModeBase {
    virtual void v00();
    unsigned char _body[0x18];
    CModeBase(CThing& thing);              // 0x0073EAC0
};
namespace NCreatureMode {
struct CModeDragonFlyingBase : CModeBase { // vtable 0x01268658
    CCardinalSpline FlightSpline;          // +0x1c
    float SplinePercentageComplete;        // +0x30
    float LegAnimAlpha;                    // +0x34
    virtual void v00();
    CModeDragonFlyingBase(CThing& thing) : CModeBase(thing) { }
};
struct CModeDragonFlying : CModeDragonFlyingBase {   // vtable 0x0126868C
    CAnimFilterOfGroup Filter;             // +0x38
    C3DVector Destination;                 // +0x44
    C3DVector StartPos;                    // +0x50
    C3DVector PreviousPos;                 // +0x5c
    float DistanceToTravel;                // +0x68
    float Speed;                           // +0x6c
    float StartSpeed;                      // +0x70
    float MovementDuringAnimationLoop;     // +0x74
    float Acceleration;                    // +0x78
    float AnimLength;                      // +0x7c
    CCountedPointer<CCreatureActionBase> PEndAction;   // +0x80
    long NumAnimLoops;                     // +0x88
    long NumAnimLoopsToDo;                 // +0x8c
    bool FirstFrame;                       // +0x90
    bool UsePreviousPos;                   // +0x91
    virtual void v00();
    CModeDragonFlying(CThing& thing, C3DVectorVector& points, const C3DVector& position, float speed,
                      CCountedPointer<CCreatureActionBase> endAction);
};

CModeDragonFlying::CModeDragonFlying(CThing& thing, C3DVectorVector& points, const C3DVector& position, float speed,
                                     CCountedPointer<CCreatureActionBase> endAction)
    : CModeDragonFlyingBase(thing)
    , Filter("FLYING")
    , Speed(speed)
{
    PEndAction = endAction;
    StartPos = points[0];
    Destination = points[points.size() - 1];
    FlightSpline.ControlPoints.resize(points.size() + 1, C3DVector());
    NumAnimLoopsToDo = points.size() - 1;
    FlightSpline.ControlPoints[0] = position;
    UsePreviousPos = true;
    for (unsigned long i = 0; i < points.size(); ++i)
        FlightSpline.ControlPoints[i + 1] = points[i];
    SplinePercentageComplete = 0;
    NumAnimLoops = 0;
}
}