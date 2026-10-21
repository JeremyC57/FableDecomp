// NTCScriptedControl::CActionMoveTo::OnCleanup() -- free the claimed action point, restore the old physics component, the pushable flag and the creature's navigation state
// Layout: CActionMoveTo offsets are the PDB's (+0x24 PTargetThing, +0x34 PTCOldPhysics, +0x45/+0x46/+0x48/+0x4c); the creature is
// re-read through the owning control at +4 (its +4), as in the landed CActionLeadCreature::OnActivate 0x007122D0.
class CCharString {
public:
    void* PStringData;
    ~CCharString();                                  // 0x0099EAE0
};
struct CTCBase;
struct C3DVector { float X, Y, Z; C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { } };
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
    virtual void v081(); virtual void v082(); virtual void v083();
    virtual void SetPushableByHero(bool pushable);   // slot 84 (+0x150)
    char _pad04[0xc - 0x4];
    C3DVector Position;                              // +0x0C
};

struct CTCPhysicsBase {
    virtual ~CTCPhysicsBase();   // slot 0 (+0x0)
    virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007(); virtual void v008();
    virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016();
    virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021();
    virtual CCharString GetName() const;   // slot 22 (+0x58)
};

struct CTCUserActionPointBase { void FreeActionPoint(long index); };   // 0x007FEF40
struct CTCNavigator { void ResetNavigation(); };                         // 0x006CB6B0

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);            // 0x0040F020
};

struct CPhysicsSnapshot_fwd;
struct CThing {
    void*               __vftable;                   // +0x00
    unsigned char       _pad04[0x20 - 0x04];
    unsigned long       UsingComponents[9];          // +0x20 bitset<274>
    CInterfaceMap       Components;                  // +0x44
    unsigned char       _pad4c[0x60 - 0x4c];
    CPhysicsObject*     PPhysics;                    // +0x60
    unsigned char       _pad64[0x91 - 0x64];
    unsigned char       Flags91;                     // +0x91
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

struct CCreaturePointer { void* __vftable; CThing* PThing; };

class CActionMoveTo {
public:
    void* __vftable;                                 // +0x00
    CCreaturePointer* PControl;                      // +0x04
    char _pad08[0x24 - 0x08];
    CScriptThing PTargetThing;                       // +0x24
    bool UseTargetThing;                             // +0x30
    CTCPhysicsBase* PTCOldPhysics;                   // +0x34
    char _pad38[0x45 - 0x38];
    bool AvoidDynamicObstacles;                      // +0x45
    bool PreviousPushableByHero;                     // +0x46
    bool UseActionPoint;                             // +0x47
    long ActionPointIndex;                           // +0x48
    bool GotActionPointIndex;                        // +0x4C
    void OnCleanup();
};

void CActionMoveTo::OnCleanup()
{
    if (GotActionPointIndex && PTargetThing.IsAlive()) {
        CTCUserActionPointBase* user;
        PTargetThing.GetThing()->GetInterface(&user, 0x2d);
        user->FreeActionPoint(ActionPointIndex);
    }
    if (PTCOldPhysics) {
        CPhysicsObject* physics = PControl->PThing->PPhysics;
        C3DVector position = physics->Position;
        CBoundingBox bounds = physics->GetBounds();
        CPhysicsSnapshot snapshot(position, bounds);
        CThing* thing = PControl->PThing;
        thing->AddTC(PTCOldPhysics->GetName(), true, snapshot);
        delete PTCOldPhysics;
        PTCOldPhysics = 0;
    }
    if (!AvoidDynamicObstacles) {
        CThing* creature = PControl->PThing;
        if (creature && !(creature->Flags91 & 1))
            creature->PPhysics->SetPushableByHero(PreviousPushableByHero);
    }
    CTCNavigator* navigator;
    if (PControl->PThing->GetInterface(&navigator, 7))
        navigator->ResetNavigation();
}
