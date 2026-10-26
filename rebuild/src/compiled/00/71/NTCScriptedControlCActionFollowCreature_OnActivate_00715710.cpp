// NTCScriptedControl::CActionFollowCreature::OnActivate() -- make the creature able to move, swap its physics component for a navigator (keeping the old one), start sneaking behind a sneaking target, and remember/clear the pushable flag
// Layout: CActionFollowCreature offsets are the PDB's (+0xC PTargetThing, +0x18 PTCOldPhysics, +0x25/+0x26); +8 is the base's activated flag;
// the creature is re-read through the owning control at +4 (its +4), as in the landed CActionLeadCreature::OnActivate 0x007122D0.
// Leaf: ?OnActivate@CActionFollowCreature@NTCScriptedControl@@UAEXXZ
// PDB spelling (sweep 3): the class lives in namespace NTCScriptedControl with the Ego_r CActionBase member PTCScript (+0x04);
//   OnActivate is its virtual (?OnActivate@CActionFollowCreature@NTCScriptedControl@@UAEXXZ) and 0x00713D50 is the private
//   SetAsSneaking (?SetAsSneaking@CActionFollowCreature@NTCScriptedControl@@AAEX_N@Z, was an invented global SetSneaking).
//   Same bytes.
class CCharString {
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);     // 0x0099EBF0
    ~CCharString();                                  // 0x0099EAE0
    bool operator!=(const char* text) const;         // 0x0099E960
};
class CTCBase;
class C3DVector { public: float X, Y, Z; C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { } };
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

class CTCPhysicsBase { public:
    virtual ~CTCPhysicsBase();   // slot 0 (+0x0)
    virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007(); virtual void v008();
    virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016();
    virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021();
    virtual CCharString GetName() const;   // slot 22 (+0x58)
    virtual void v023(); virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029(); virtual void v030();
    virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038();
    virtual void v039(); virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046();
    virtual void v047(); virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054();
    virtual void v055(); virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059(); virtual void v060(); virtual void v061(); virtual void v062();
    virtual void v063(); virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069(); virtual void v070();
    virtual void v071();
    virtual const CBoundingBox& GetBounds();   // slot 72 (+0x120)
    virtual void v073(); virtual void v074(); virtual void v075(); virtual void v076(); virtual void v077(); virtual void v078(); virtual void v079(); virtual void v080();
    virtual void v081(); virtual void v082();
    virtual bool IsPushableByHero();   // slot 83 (+0x14c)
    virtual void SetPushableByHero(bool pushable);   // slot 84 (+0x150)
    char _pad04[0xc - 0x4];
    C3DVector Position;                              // +0x0C
    CTCPhysicsBase* Clone();                         // 0x00686FA0
};

class CTCStealth { public: char _pad[0xf]; bool Sneaking; };   // interface 0x36

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) const;            // 0x004365B0
};

class CThing { public:
    void*               __vftable;                   // +0x00
    unsigned char       _pad04[0x20 - 0x04];
    unsigned long       UsingComponents[9];          // +0x20 bitset<274>
    CInterfaceMap       Components;                  // +0x44
    unsigned char       _pad4c[0x60 - 0x4c];
    CTCPhysicsBase*     PPhysics;                    // +0x60
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

bool __fastcall IsAbleToMove(CThing* creature);                  // 0x0085F3E0
void __fastcall RemoveMovementBlockingModes(CThing* creature);   // 0x0085F600

class CScriptThing { public:
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

class CTCScriptedControl { public: void* __vftable; CThing* ParentThing; };   // CTCBase: ParentThing +0x04

namespace NTCScriptedControl {
class CActionFollowCreature {
public:
    virtual void OnActivate();                       // vptr +0x00 (UAE: the action's virtual OnActivate)
    CTCScriptedControl* PTCScript;                   // +0x04 (Ego_r CActionBase)
    bool Activated;                                  // +0x08
    CScriptThing PTargetThing;                       // +0x0C
    CTCPhysicsBase* PTCOldPhysics;                   // +0x18
    bool Sneaking;                                   // +0x1C
    float FollowRange;                               // +0x20
    bool AddedStealthTC;                             // +0x24
    bool AvoidDynamicObstacles;                      // +0x25
    bool PreviousPushableByHero;                     // +0x26
    bool Running;                                    // +0x27
private:
    void SetAsSneaking(bool sneaking);               // 0x00713D50 (Ego_r ?SetAsSneaking@CActionFollowCreature@NTCScriptedControl@@AAEX_N@Z)
};
} // namespace NTCScriptedControl

void NTCScriptedControl::CActionFollowCreature::OnActivate()
{
    Activated = true;
    if (!IsAbleToMove(PTCScript->ParentThing))
        RemoveMovementBlockingModes(PTCScript->ParentThing);
    PTCOldPhysics = 0;
    CTCPhysicsBase* physics = PTCScript->ParentThing->PPhysics;
    if (physics->GetName() != "CTCPhysicsNavigator") {
        PTCOldPhysics = physics->Clone();
        C3DVector position = physics->Position;
        CBoundingBox bounds = physics->GetBounds();
        CPhysicsSnapshot snapshot(position, bounds);
        CCharString navigatorName("CTCPhysicsNavigator");
        PTCScript->ParentThing->AddTC(navigatorName, true, snapshot);
    }
    CTCStealth* stealth;
    if (PTargetThing.IsAlive() && PTargetThing.GetThing()->GetInterface(&stealth, 0x36) && stealth->Sneaking)
        SetAsSneaking(true);
    if (!AvoidDynamicObstacles) {
        PreviousPushableByHero = PTCScript->ParentThing->PPhysics->IsPushableByHero();
        PTCScript->ParentThing->PPhysics->SetPushableByHero(false);
    }
}