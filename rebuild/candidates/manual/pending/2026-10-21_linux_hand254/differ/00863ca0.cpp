// Grade: DIFFER(361v358) -- frame layout: retail keeps a 0x30 aligned frame with the C2DBoxI temporary in its own words (O+0x30) and sinks the local vector's zero stores to just before the search call (`xor esi,esi`); VC7.1 here packs the box over the size float (frame 0x20) and zeroes the vector at the top, so every frame offset and the register picks after it shift
// CThingBodyReorienter::FillBodyReorientationPossibleIntersecting() -- start a reorientation check: gather the things with a physics mesh in a square round the body (twice its physics radius, at least 1) as the possible intersecting things; with none, the reorientation is done at once
// Leaf: FillBodyReorientationPossibleIntersecting
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00863CA0, length 358 bytes (aligned frame: `and esp,-8`, `mov esp,ebp`,
//   plain `ret`; Ego_r.pdb `?FillBodyReorientationPossibleIntersecting@CThingBodyReorienter@@AAEXXZ`).
// Callees: CThing::GetPosition() const 0x004C73D0; __ftol2 0x00BFEA70 (x3); CThingSearchTools::GetAllThingsInRoughArea
//   <const CThing, vector<const CThing*>, CThingFilter_HasPhysicsMesh>(const C2DBoxI&, const CThingFilter_HasPhysicsMesh&,
//   vector&) 0x00863E70 (the Ego_r.pdb instantiation that follows FillBodyReorientationPossibleIntersecting; the signature
//   matches the pushes); CBaseIntelligentPointer() 0x00A01B10, SetPItem 0x00A01B90, GetPItem 0x00A01B50,
//   ~CBaseIntelligentPointer 0x00A01C10 (CIntelligentPointer<CThing>, vtable 0x01238C6C); vector<CIntelligentPointer
//   <CThing> >::_M_insert_overflow 0x004AC940 (the inline push_back); free 0x00BFEA14 (the local vector).
// Data: 1.0f 0x0122DED8; g_pMainGameComponent 0x013B86A0 (+0x24 world, +0x20 thing search tools, as landed).
// Layout: Ego_r.pdb CThingBodyReorienter (as the landed UpdateBodyReorienation 0x00863E10) with the 12-byte STLport
//   vector: StillCheckingForNewBodyReorientationAngle +0x08, HasPossibleIntersectingThings +0x14,
//   PossibleIntersectingThings +0x18, Body +0x24 (PDB +0x28); CThing PhysicsTC +0x60, CTCPhysicsBase Radius +0x34 (as
//   the landed CTCUnholyPower::OnCreate 0x007949F0).
// Shape: the size is `radius * 2` clamped below at 1; the box is a C2DBoxI temporary from the truncated position and
//   size; the found things are wrapped one by one in a CIntelligentPointer temporary pushed onto the member vector.
struct C3DVector { float X, Y, Z; };
struct C2DCoordI {
    long X, Y;
    C2DCoordI(long x, long y) : X(x), Y(y) { }
};
struct C2DBoxI {
    long TLX, TLY, BRX, BRY;
    C2DBoxI(long tlx, long tly, long brx, long bry) : TLX(tlx), TLY(tly), BRX(brx), BRY(bry) { }
};
extern "C" void __cdecl free(void* block);                   // 0x00BFEA14

struct CTCPhysicsBase {
    unsigned char _pad00[0x34];
    float         Radius;                                    // +0x34
};
class CThing {
public:
    unsigned char   _pad00[0x60];
    CTCPhysicsBase* PhysicsTC;                               // +0x60
    const C3DVector& GetPosition() const;                    // 0x004C73D0
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                                         // +0x04
    CBaseIntelligentPointer();                               // 0x00A01B10
    ~CBaseIntelligentPointer();                              // 0x00A01C10
    CThing* GetPItem() const;                                // 0x00A01B50
    void SetPItem(CThing* item);                             // 0x00A01B90
};
struct CIntelligentPointer : CBaseIntelligentPointer {       // CIntelligentPointer<CThing>, vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointer(CThing* item) { SetPItem(item); }
    CIntelligentPointer(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); }
};
inline void* operator new(unsigned int, void* where) { return where; }
struct __false_type { };
template <class T> struct CRetailVector {                    // STLport vector (12 bytes)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    CRetailVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~CRetailVector() { if (_M_start) free(_M_start); }
    T* begin() const { return _M_start; }
    T* end() const { return _M_finish; }
    bool empty() const { return _M_start == _M_finish; }
    void _M_insert_overflow(T* position, const T& x, const __false_type& tag, unsigned int n, bool atEnd);   // 0x004AC940
    void push_back(const T& x)
    {
        if (_M_finish != _M_end_of_storage) {
            if (_M_finish)
                new (_M_finish) T(x);
            ++_M_finish;
        } else
            _M_insert_overflow(_M_finish, x, __false_type(), 1, true);
    }
};
struct CThingFilter_HasPhysicsMesh { };
struct CThingSearchTools {
    template <class T, class TVector, class TFilter>
    void GetAllThingsInRoughArea(const C2DBoxI& box, const TFilter& filter, TVector& things) const;   // 0x00863E70
};
struct CWorld {
    unsigned char       _pad00[0x20];
    CThingSearchTools*  ThingSearchTools;                    // +0x20
};
struct CMainGameComponent {
    unsigned char _pad00[0x24];
    CWorld*       World;                                     // +0x24
};
extern CMainGameComponent* g_pMainGameComponent;             // 0x013B86A0

struct CThingBodyReorienter {
    bool  BodyReorientationDone;                             // +0x00
    bool  ReorientingBody;                                   // +0x01
    long  ReorientationCount;                                // +0x04
    bool  StillCheckingForNewBodyReorientationAngle;         // +0x08
    bool  HaveFinalReorientationAngle;                       // +0x09
    float FinalReorientationAngleXY;                         // +0x0c
    long  RotationDirection;                                 // +0x10 ERotationDirection
    bool  HasPossibleIntersectingThings;                     // +0x14
    CRetailVector<CIntelligentPointer> PossibleIntersectingThings;   // +0x18
    CThing* Body;                                            // +0x24
    void FillBodyReorientationPossibleIntersecting();
};

void CThingBodyReorienter::FillBodyReorientationPossibleIntersecting()
{
    CRetailVector<CThing*> things;
    CThingFilter_HasPhysicsMesh filter;
    HasPossibleIntersectingThings = true;
    StillCheckingForNewBodyReorientationAngle = true;
    float size = Body->PhysicsTC->Radius * 2.0f;
    if (size < 1.0f)
        size = 1.0f;
    const C3DVector& position = Body->GetPosition();
    C2DCoordI centre((long)position.X, (long)position.Y);
    long half = (long)size;
    g_pMainGameComponent->World->ThingSearchTools->GetAllThingsInRoughArea<const CThing>(
        C2DBoxI(centre.X - half, centre.Y - half, centre.X + half, centre.Y + half), filter, things);
    for (CThing** it = things.begin(); it != things.end(); ++it)
        PossibleIntersectingThings.push_back(CIntelligentPointer(*it));
    if (PossibleIntersectingThings.empty()) {
        BodyReorientationDone = true;
        StillCheckingForNewBodyReorientationAngle = false;
        HasPossibleIntersectingThings = false;
    }
}
