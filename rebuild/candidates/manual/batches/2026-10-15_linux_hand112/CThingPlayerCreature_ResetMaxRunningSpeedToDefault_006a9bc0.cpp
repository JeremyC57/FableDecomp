// CThingPlayerCreature::ResetMaxRunningSpeedToDefault() -- restores MaxRunningSpeed from the creature def and re-initialises it under the "ST_RUN" speed table name
// Retail: Fable.exe 0x006A9BC0, 67 bytes.
// Callees: CCharString::CCharString(const char*, long) 0x0099EBF0, CThingCreatureBase::InitialiseMovementSpeed
// (float*, const CCharString&) 0x00663510, CCharString::~CCharString 0x0099EAE0; literal "ST_RUN".
// Retail offsets: PDef (CDefPointer<CThingCreatureDef>) +0xe0, MaxRunningSpeed +0x198 (Ego_r.pdb +0x1c0; the
// retail player creature is 0x28 narrower there), CThingCreatureDef::MaxRunningSpeed +0x98 (PDB +0x9c: the retail
// def base is 4 bytes narrower). Local model.
// Shape: the member's address held in edi for the store and the call; the name is a named CCharString
// local (re-addressed with `lea` for the call), destroyed out of line at the end.
struct CCharString {
    void* m_pData;
    CCharString(const char* text, long length);          // 0x0099EBF0
    ~CCharString();                                      // 0x0099EAE0
};

struct CThingCreatureDef {
    unsigned char _pad00[0x98];
    float         MaxRunningSpeed;                             // +0x98
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
    unsigned char _pade4[0x198 - 0xe4];
    float         MaxRunningSpeed;                             // +0x198
    void ResetMaxRunningSpeedToDefault();
};

void CThingPlayerCreature::ResetMaxRunningSpeedToDefault()
{
    MaxRunningSpeed = PDef->MaxRunningSpeed;
    CCharString name("ST_RUN", -1);
    InitialiseMovementSpeed(&MaxRunningSpeed, name);
}
