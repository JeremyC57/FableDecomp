// CThingSwitch::ConstructFromParams(long defIndex, const C3DVector* pos, float triggerRadius, CCharString triggerableObjectScriptName) -- construct the game-object base with no script name, then set the switch's radius, untriggered state, target script name, trigger count and full health
// Leaf: ConstructFromParams
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0083BD80, length 160 bytes (`ret 10h`; the by-value CCharString is
// destroyed by the callee on both paths).
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0 (literal 0x0122D70E = ""), CThingGameObject::
// ConstructFromParams(long, const C3DVector*, long, unsigned long, const CCharString&, CThingPhysical*, CThingPhysical*,
// bool) @ 0x008388D0 (landed), CCharString::~CCharString @ 0x0099EAE0 (manifest label is a propagated twin),
// CCharString::operator= @ 0x0099EFB0.
// Layout (retail; Ego_r.pdb CThingSwitch in brackets, retail 0x18 lower as for CThingPhysicalSwitch):
// CThingPhysical MaxHealth +0xb0 [+0xc8], Health +0xb4 [+0xcc]; TriggerRadius +0xd0 [+0xe8], Triggered +0xdc [+0xf4],
// TriggerableObjectScriptName +0xe0 [+0xf8], TimesTriggered +0xe4 [+0xfc].
// Shape: the empty name is a named local built before any argument push, in its own block so it is destroyed
// right after the base call (before the result test; the bool kept in bl); the member stores follow retail's order with the 1.0f health pair as dword stores.
struct C3DVector;
struct CThingPhysical;

struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);  // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
    CCharString& operator=(const CCharString& other);  // 0x0099EFB0
};

struct CThingGameObject {
    unsigned char _pad_0x00[0xb0];
    float MaxHealth;                                   // +0xb0 CThingPhysical [PDB +0xc8]
    float Health;                                      // +0xb4 CThingPhysical [PDB +0xcc]
    unsigned char _pad_0xb8[0xd0 - 0xb8];
    bool ConstructFromParams(long defIndex, const C3DVector* pos, long a3, unsigned long a4,
                             const CCharString& scriptName, CThingPhysical* creator,
                             CThingPhysical* originalCreator, bool a8);   // 0x008388D0
};

struct CThingSwitch : CThingGameObject {
    float       TriggerRadius;                         // +0xd0
    CCharString EnvironmentDef;                        // +0xd4
    float       TimeToChangeEnvironmentDef;            // +0xd8
    bool        Triggered;                             // +0xdc
    CCharString TriggerableObjectScriptName;           // +0xe0
    long        TimesTriggered;                        // +0xe4
    bool ConstructFromParams(long defIndex, const C3DVector* pos, float triggerRadius, CCharString triggerableObjectScriptName);
};

bool CThingSwitch::ConstructFromParams(long defIndex, const C3DVector* pos, float triggerRadius, CCharString triggerableObjectScriptName)
{
    bool constructed;
    {
        CCharString noScriptName("");
        constructed = CThingGameObject::ConstructFromParams(defIndex, pos, -1, 0, noScriptName, 0, 0, true);
    }
    if (constructed) {
        Triggered = false;
        TriggerRadius = triggerRadius;
        TriggerableObjectScriptName = triggerableObjectScriptName;
        TimesTriggered = 0;
        Health = 1.0f;
        MaxHealth = 1.0f;
        return true;
    }
    return false;
}