// NTCScriptedControl::CActionMoveTo::OnActivate() -- start the move: enter scripted movement, swap in the navigator physics, claim the target's action point or widen the proximity by both radii, stop hero pushing, and set up a route-thing route
// Layout: CActionMoveTo offsets are the PDB's (Pos +0xc, Proximity +0x20, PTargetThing +0x24, UseTargetThing +0x30, PTCOldPhysics +0x34,
//   PRouteThing +0x38, UseRouteThing +0x44 .. CancelAllActionsOnFailure +0x4e); shared types as the landed OnCleanup 0x00714730.
template<class T> __forceinline const T& Max(const T& a, const T& b) { return a < b ? b : a; }
class CCharString {
public:
    void* PStringData;
    CCharString(const char* text, long length = -1); // 0x0099EBF0
    ~CCharString();                                  // 0x0099EAE0
    bool operator!=(const char* text) const;         // 0x0099E960 (NotEqual)
};
struct CTCBase;
struct C3DVector { float X, Y, Z; C3DVector() { } C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { } };
struct CBoundingBox { C3DVector Min, Max; };

class CBaseClassNonCopyable {
public:
    virtual void v000();
    CBaseClassNonCopyable();                         // 0x0099A2F0
    ~CBaseClassNonCopyable();                        // 0x0099A300
};
class CPhysicsSnapshot : public CBaseClassNonCopyable {   // vtable 0x0125C818
public:
    C3DVector Position;
    CBoundingBox Bounds;
    virtual void v000();
    __forceinline CPhysicsSnapshot(const C3DVector& position, const CBoundingBox& bounds) : Position(position), Bounds(bounds) { }
};

struct CTCPhysicsBase;
struct CPhysicsObject {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007();
    virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015();
    virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031();
    virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047();
    virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055();
    virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059(); virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063();
    virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069(); virtual void v070(); virtual void v071();
    virtual const CBoundingBox& GetBounds();   // slot 72 (+0x120)
    virtual void v073(); virtual void v074(); virtual void v075(); virtual void v076(); virtual void v077(); virtual void v078(); virtual void v079(); virtual void v080();
    virtual void v081(); virtual void v082();
    virtual bool IsPushableByHero();                 // slot 83 (+0x14c)
    virtual void SetPushableByHero(bool pushable);   // slot 84 (+0x150)
    char _pad04[0xc - 0x4];
    C3DVector Position;                              // +0x0C
    struct CTCPhysicsBase* Detach();                 // 0x00686FA0
    unsigned char _pad18[0x34 - 0x18];
    float Radius;                                    // +0x34
};

struct CTCPhysicsBase {
    virtual ~CTCPhysicsBase();   // slot 0 (+0x0)
    virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007(); virtual void v008();
    virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016();
    virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021();
    virtual CCharString GetName() const;   // slot 22 (+0x58)
};

struct CThing;
struct CTCUserActionPointBase {
    bool ClaimActionPoint(CThing* user, C3DVector* position, float* facing, long* index);   // 0x007FEDE0
};
struct CTCActionPointUser { unsigned char _pad00[0x10]; void* PActionPoint; };   // interface 0x47, +0x10 current point
struct CTCRoute {
    unsigned char _pad00[0x20];
    unsigned char Points[0x0c];                      // +0x20
    bool HasRoute() const;                           // 0x00800500
    C3DVector GetEndPosition() const;                // 0x008004D0
};
struct CTCNavigator { void FollowRoute(const C3DVector& from, const void* points, const C3DVector* to); };   // 0x006CB9C0

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);            // 0x0040F020
    const SEntry* LowerBound(const long* type) const;   // 0x004365B0
};

struct CPhysicsSnapshot_fwd;
struct CThing {
    void*               __vftable;                   // +0x00
    unsigned char       _pad04[0x20 - 0x04];
    unsigned long       UsingComponents[9];          // +0x20 bitset<274>
    CInterfaceMap       Components;                  // +0x44
    unsigned char       _pad4c[0x60 - 0x4c];
    CPhysicsObject*     PPhysics;                    // +0x60
    unsigned char       _pad64[0x6c - 0x64];
    unsigned char       Flags6c;                     // +0x6c
    unsigned char       _pad6d[0x91 - 0x6d];
    unsigned char       Flags91;                     // +0x91
    const C3DVector& GetPosition() const;            // 0x004C73D0
    bool IsInScriptedMovement();                     // 0x0085F3E0
    void EnterScriptedMovement();                    // 0x0085F600
    void AddTC(const CCharString& name, bool b, const CPhysicsSnapshot& snapshot);   // 0x004C9D60
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (UsingComponents[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &Components;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType) const
    {
        if (UsingComponents[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            const CInterfaceMap* map = &Components;
            const CInterfaceMap::SEntry* it = map->LowerBound(&interfaceType);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};

struct CScriptThing {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007();
    virtual void v008(); virtual void v009(); virtual void v010();
    virtual CThing* GetThing() const;   // slot 11 (+0x2c)
    virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019();
    virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027();
    virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035();
    virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039(); virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043();
    virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047(); virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051();
    virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055(); virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059();
    virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063(); virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067();
    virtual void v068(); virtual void v069(); virtual void v070(); virtual void v071(); virtual void v072(); virtual void v073(); virtual void v074();
    virtual bool IsAlive() const;   // slot 75 (+0x12c)
    long _a, _b;
};

struct CCreaturePointer { void* __vftable; CThing* PThing; void CancelAllActions(); /* 0x00713630 */ };

class CActionMoveTo {
public:
    void* __vftable;                                 // +0x00
    CCreaturePointer* PControl;                      // +0x04
    bool Activated;                                  // +0x08
    bool Failed;                                     // +0x09
    char _pad0a[0x0c - 0x0a];
    C3DVector Pos;                                   // +0x0c
    long MovementType;                               // +0x18
    bool FaceMovement;                               // +0x1c
    float Proximity;                                 // +0x20
    CScriptThing PTargetThing;                       // +0x24
    bool UseTargetThing;                             // +0x30
    CTCPhysicsBase* PTCOldPhysics;                   // +0x34
    CScriptThing PRouteThing;                        // +0x38
    bool UseRouteThing;                              // +0x44
    bool AvoidDynamicObstacles;                      // +0x45
    bool PreviousPushableByHero;                     // +0x46
    bool UseActionPoint;                             // +0x47
    long ActionPointIndex;                           // +0x48
    bool GotActionPointIndex;                        // +0x4C
    bool UseBackupProximityForActionPointIfRouteBlocked;   // +0x4D
    bool CancelAllActionsOnFailure;                  // +0x4E
    void OnActivate();
};

void CActionMoveTo::OnActivate()
{
    Activated = true;
    if (!PControl->PThing->IsInScriptedMovement())
        PControl->PThing->EnterScriptedMovement();
    PTCOldPhysics = 0;
    CPhysicsObject* physics = PControl->PThing->PPhysics;
    bool swapPhysics = ((CTCPhysicsBase*)physics)->GetName() != "CTCPhysicsNavigator";
    if (swapPhysics) {
        PTCOldPhysics = physics->Detach();
        C3DVector position = physics->Position;
        CBoundingBox bounds = physics->GetBounds();
        CPhysicsSnapshot snapshot(position, bounds);
        PControl->PThing->AddTC("CTCPhysicsNavigator", true, snapshot);
    }
    if (UseTargetThing && PTargetThing.IsAlive()) {
        if (UseActionPoint) {
            CCreaturePointer* control = PControl;
            CTCActionPointUser* pointUser;
            if (control->PThing->GetInterface(&pointUser, 0x47) && pointUser->PActionPoint) {
                Failed = true;
                if (CancelAllActionsOnFailure)
                    control->CancelAllActions();
                return;
            }
            bool claimed;
            {
                CTCUserActionPointBase* actionPoints;
                float facing;
                claimed = PTargetThing.GetThing()->GetInterface(&actionPoints, 0x2d)
                    && actionPoints->ClaimActionPoint(PControl->PThing, &Pos, &facing, &ActionPointIndex);
            }
            if (claimed) {
                GotActionPointIndex = true;
                UseTargetThing = false;
            } else {
                Failed = true;
                if (CancelAllActionsOnFailure)
                    PControl->CancelAllActions();
                return;
            }
        } else {
            CThing* target = PTargetThing.GetThing();
            if ((target->Flags6c & 8) || ((unsigned char*)target)[0x10] == 5 || ((unsigned char*)target)[0x10] == 3) {
                float radii = PControl->PThing->PPhysics->Radius + target->PPhysics->Radius;
                Proximity = Max(Proximity, radii);
            }
        }
    }
    if (!AvoidDynamicObstacles) {
        PreviousPushableByHero = PControl->PThing->PPhysics->IsPushableByHero();
        PControl->PThing->PPhysics->SetPushableByHero(false);
    }
    if (UseRouteThing && PRouteThing.IsAlive()) {
        CTCNavigator* navigator;
        if (PControl->PThing->GetInterface(&navigator, 7)) {
            CTCRoute* route;
            ((const CThing*)PRouteThing.GetThing())->GetInterface(&route, 0xa6);
            if (route->HasRoute()) {
                Pos = route->GetEndPosition();
                navigator->FollowRoute(PControl->PThing->GetPosition(), route->Points, &Pos);
                return;
            }
            Failed = true;
        }
    }
}
