// CThingPlayerCreature::ResetMaxWalkingSpeedToDefault() -- restores MaxWalkingSpeed from the creature def and re-initialises it under the "ST_WALK" speed table name
// Leaf: ResetMaxWalkingSpeedToDefault
// Retail: Fable.exe 0x006A9B70, 67 bytes. The catalog label "ResetMaxRunningSpeedToDefault" is propagated from
// the same-shape 0x006A9BC0 (different member and literal); this body stores MaxWalkingSpeed and passes "ST_WALK".
// Callees: CCharString::CCharString(const char*, long) 0x0099EBF0, CThingCreatureBase::InitialiseMovementSpeed
// (float*, const CCharString&) 0x00663510, CCharString::~CCharString 0x0099EAE0; literal "ST_WALK".
// Retail offsets: PDef (CDefPointer<CThingCreatureDef>) +0xe0, MaxWalkingSpeed +0x190 (Ego_r.pdb +0x1b8; the
// retail player creature is 0x28 narrower there), CThingCreatureDef::MaxWalkingSpeed +0x90 (PDB +0x94: the retail
// def base is 4 bytes narrower). Local model.
// Shape: the member's address held in edi for the store and the call; the name is a named CCharString
// local (re-addressed with `lea` for the call), destroyed out of line at the end.
struct CCharString {
    void* m_pData;
    CCharString(const char* text, long length);          // 0x0099EBF0
    ~CCharString();                                      // 0x0099EAE0
};

struct CThingCreatureDef {
    unsigned char _pad00[0x90];
    float         MaxWalkingSpeed;                             // +0x90
};

template <class T> struct CDefPointer {
    T* m_pDef;
    T* operator->() const { return m_pDef; }
};

struct CThingCreatureBase {
    unsigned char                   _pad00[0xe0];
    CDefPointer<CThingCreatureDef>  PDef;                // +0xe0
    void InitialiseMovementSpeed(float* speed, const CCharString& name);   // 0x00663510
};

struct CThingPlayerCreature : CThingCreatureBase {
    unsigned char _pade4[0x190 - 0xe4];
    float         MaxWalkingSpeed;                             // +0x190
    void ResetMaxWalkingSpeedToDefault();
};

void CThingPlayerCreature::ResetMaxWalkingSpeedToDefault()
{
    MaxWalkingSpeed = PDef->MaxWalkingSpeed;
    CCharString name("ST_WALK", -1);
    InitialiseMovementSpeed(&MaxWalkingSpeed, name);
}