// CThingPhysical::SerialiseGameState(CPersistContext& context) -- outside the load modes reset max health, camera flag, creators and hit flags; then persist Health (default MaxHealth, clamped for non-creatures) and Scale (default 1)
// Leaf: SerialiseGameState
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006A5AF0, length 315 bytes (+ int3 padding), `ret 4`.
// Callees: CBaseIntelligentPointer ctor @ 0x00A01B10 / SetPItem @ 0x00A01B90 / GetPItem @ 0x00A01B50 / dtor @ 0x00A01C10
// (temporaries CIntelligentPointer<CThingPhysical>, vtable 0x01244DCC), CPersistContext::Transfer<float>(const char*,
// float&, const float&) @ 0x00410620 (catalog "Transfer<signed_char>" is a propagated twin).
// Strings: 0x0125D090 "Health", 0x0125D088 "Scale".
// Layout: as the landed CThingPhysical::ConstructFromParams 0x006A5950 (retail CThingPhysical, local model since
// engine/CThingPhysical.h carries the PDB offsets); CThing ThingClass byte +0x10 (2 = creature); CPersistContext Mode
// +0x18 (Ego_r.pdb).
// Shape: the reset block is ConstructFromParams' tail (creators cleared through CIntelligentPointer(0) temporaries,
// MaxHealth copied from the def as a dword); the Scale default is a 1.0f local homed in the dead `context` slot; the
// health clamp is `if (Health > MaxHealth) Health = MaxHealth;` (`fcomp ; test ah,41h`).
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

struct CPersistContext {
    unsigned char _pad00[0x18];
    long          Mode;                                        // +0x18 EMode
    void Transfer(const char* name, float& value, const float& defaultValue);   // 0x00410620
};
extern const char g_strHealth[];                               // 0x0125D090 "Health"
extern const char g_strScale[];                                // 0x0125D088 "Scale"

class CThing {
public:
    virtual void v00();
    unsigned char  _pad04[0x10 - 0x04];
    unsigned char  ThingClass;                                 // +0x10
    unsigned char  _pad11[0x70 - 0x11];
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
    bool SerialiseGameState(CPersistContext& context);
};

bool CThingPhysical::SerialiseGameState(CPersistContext& context)
{
    if (context.Mode != 1 && context.Mode != 3) {
        MaxHealth = PBaseDef->Health;
        CanComeBetweenCameraAndObservedThing = -1;
        PLastCreator = CIntelligentPointerPhysical(0);
        POriginalCreator = CIntelligentPointerPhysical(0);
        Drawable = true;
        EvadeAllHits = false;
        NegateAllHits = false;
        Killable = true;
        RespondToHits = true;
        Damageable = PBaseDef->Damageable;
        AbleToBeEngagedInCombat = true;
    }
    context.Transfer(g_strHealth, Health, MaxHealth);
    if (ThingClass != 2 && Health > MaxHealth)
        Health = MaxHealth;
    float defaultScale = 1.0f;
    context.Transfer(g_strScale, Scale, defaultScale);
    return true;
}
