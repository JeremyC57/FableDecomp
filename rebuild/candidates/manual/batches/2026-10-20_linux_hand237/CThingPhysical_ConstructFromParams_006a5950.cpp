// CThingPhysical::ConstructFromParams(long defIndex, const C3DVector* pos, long a3, unsigned long a4, CThingPhysical* originalCreator, CThingPhysical* lastCreator, bool a7) -- construct the CThing on the map under the position, then reset scale, health (from the def), creators and the hit flags
// Leaf: ConstructFromParams
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006A5950, length 289 bytes, `ret 1Ch`.
// Callees: the world map's GetMapNumberAt(const C3DVector&) (virtual slot 16, +0x40) on g_pThingManager @ 0x013B8A1C
// (WorldMap +0x28), CThing::ConstructFromParams(long, long, long, bool) @ 0x004CA010, CBaseIntelligentPointer ctor
// @ 0x00A01B10 / SetPItem @ 0x00A01B90 / GetPItem @ 0x00A01B50 / dtor @ 0x00A01C10 (the temporaries' vtable 0x01244DCC is
// CIntelligentPointer<CThingPhysical>, as the landed CThingPhysical ctor 0x005296B0).
// Caller: CThingGameObject::ConstructFromParams 0x008388D0 (landed; the fourth argument is not used here).
// Layout (retail, as the landed CThingPhysical ctor; engine/CThingPhysical.h carries the PDB offsets 0x18 higher, so the
// class is modelled locally): CThing PBaseDef +0x70, Drawable = bit 1 of +0x92; Scale +0x9c, PLastCreator +0xa0,
// POriginalCreator +0xa8, MaxHealth +0xb0, Health +0xb4, CanComeBetweenCameraAndObservedThing +0xb8, the hit flags
// at +0xbc (Damageable, Killable, RespondToHits, EvadeAllHits, NegateAllHits, AbleToBeEngagedInCombat).
// CThingBaseDef (retail = Ego_r.pdb - 4): Damageable bit 6 of +0x42, Health +0x50.
// Shape: `Health = MaxHealth = def->Health` (MaxHealth stored first, through the FPU); each creator is assigned through
// a CIntelligentPointer<CThingPhysical>(T*) temporary (inline ctor: base ctor, vtable, SetPItem) and the inline
// `operator=` (SetPItem(other.GetPItem())), destroyed by the base dtor; `if (base) {...; return true;} return false;`
// keeps the false exit last.
struct C3DVector { float X, Y, Z; };

class CWorldMap {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual long GetMapNumberAt(const C3DVector& pos) const;   // slot 16 (+0x40)
};
class CThingManager {
public:
    unsigned char _pad00[0x28];
    CWorldMap*    WorldMap;                                    // +0x28
};
extern CThingManager* g_pThingManager;                         // 0x013B8A1C

struct CThingBaseDef {
    unsigned char _pad00[0x42];
    unsigned char AvailableInEditor_42 : 6;                    // +0x42 bits 0..5 (IsLoadableGlobal .. DrawProjectileWeaponTrails, IsVulnerableToMelee)
    unsigned char Damageable : 1;                              // +0x42 bit 6
    unsigned char AlwaysIncludeInObjectStrikeScans : 1;        // +0x42 bit 7
    unsigned char _pad43[0x50 - 0x43];
    float         Health;                                      // +0x50
};

class CThingPhysical;
struct CBaseIntelligentPointer {
    virtual void v00();
    void* m_pItem;                                             // +0x04
    CBaseIntelligentPointer();                                 // 0x00A01B10
    ~CBaseIntelligentPointer();                                // 0x00A01C10
    CThingPhysical* GetPItem() const;                          // 0x00A01B50
    void SetPItem(CThingPhysical* item);                       // 0x00A01B90
};
struct CIntelligentPointerPhysical : CBaseIntelligentPointer { // CIntelligentPointer<CThingPhysical>, vtable 0x01244DCC
    virtual void v01();
    CIntelligentPointerPhysical() { }
    CIntelligentPointerPhysical(CThingPhysical* item) { SetPItem(item); }
    CIntelligentPointerPhysical& operator=(const CIntelligentPointerPhysical& other)
    {
        CThingPhysical* item = other.GetPItem();
        SetPItem(item);
        return *this;
    }
};

class CThing {
public:
    virtual void v00();
    unsigned char  _pad04[0x70 - 0x04];
    CThingBaseDef* PBaseDef;                                   // +0x70
    unsigned char  _pad74[0x92 - 0x74];
    unsigned char  DeletedOnOwnerLevelOutOfScope : 1;          // +0x92 bit 0
    unsigned char  Drawable : 1;                               // bit 1
    unsigned char  _pad92 : 6;
    unsigned char  _pad93[0x98 - 0x93];
    bool ConstructFromParams(long defIndex, long a2, long mapNumber, bool a4);   // 0x004CA010
};

class CThingPhysical : public CThing {
public:
    void*                       HealthOverTimeEffects;         // +0x98 (STLport list head)
    float                       Scale;                         // +0x9c
    CIntelligentPointerPhysical PLastCreator;                  // +0xa0
    CIntelligentPointerPhysical POriginalCreator;              // +0xa8
    float                       MaxHealth;                     // +0xb0
    float                       Health;                        // +0xb4
    long                        CanComeBetweenCameraAndObservedThing;   // +0xb8
    unsigned char               Damageable : 1;                // +0xbc bit 0
    unsigned char               Killable : 1;
    unsigned char               RespondToHits : 1;
    unsigned char               EvadeAllHits : 1;
    unsigned char               NegateAllHits : 1;
    unsigned char               AbleToBeEngagedInCombat : 1;
    bool ConstructFromParams(long defIndex, const C3DVector* pos, long a3, unsigned long a4,
                             CThingPhysical* originalCreator, CThingPhysical* lastCreator, bool a7);
};

bool CThingPhysical::ConstructFromParams(long defIndex, const C3DVector* pos, long a3, unsigned long a4,
                                         CThingPhysical* originalCreator, CThingPhysical* lastCreator, bool a7)
{
    long mapNumber = g_pThingManager->WorldMap->GetMapNumberAt(*pos);
    if (CThing::ConstructFromParams(defIndex, a3, mapNumber, a7)) {
        Scale = 1.0f;
        Health = MaxHealth = PBaseDef->Health;
        CanComeBetweenCameraAndObservedThing = -1;
        PLastCreator = CIntelligentPointerPhysical(lastCreator);
        POriginalCreator = CIntelligentPointerPhysical(originalCreator);
        Drawable = true;
        EvadeAllHits = false;
        NegateAllHits = false;
        Killable = true;
        RespondToHits = true;
        Damageable = PBaseDef->Damageable;
        AbleToBeEngagedInCombat = true;
        return true;
    }
    return false;
}
