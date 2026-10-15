// CThingSwitch::`scalar deleting destructor'(unsigned int) -- destroys the two CCharString members and the CThingGameObject base, then frees through the CThing class operator delete
// Leaf: ??_GCThingSwitch
// Retail: Fable.exe 0x00529BA0, 52 bytes; CThingSwitch vtable 0x012453FC slot 0 (rtti-vtables.tsv).
// Callees: CCharString::~CCharString 0x0099EAE0 (TriggerableObjectScriptName, then EnvironmentDef),
// CThingGameObject::~CThingGameObject 0x00838A00, the CThing class operator delete 0x004C73B0 (__fastcall:
// the object in ecx; it forwards to operator delete[]).
// Retail layout: Ego_r.pdb CThingSwitch EnvironmentDef +0xec / TriggerableObjectScriptName +0xf8 sit 0x18
// lower in retail (+0xd4 / +0xe0; the CThing/CThingPhysical shrink), so CThingGameObject is 0xd0 bytes.
// Shape: CThingSwitch declares no destructor -- the compiler-generated one stores no vtable and calls the
// members' destructors in reverse order then the base's; a `new CThingSwitch` factory pulls the vtable
// (and so the `??_G`) into the TU.
struct CCharString {
    void* m_pData;
    CCharString();
    ~CCharString();                                      // 0x0099EAE0
};

struct CThingGameObject {
    unsigned char _body[0xd0 - 4];
    CThingGameObject();
    virtual ~CThingGameObject();                         // 0x00838A00
    static void* operator new(unsigned int size);
    static void __fastcall operator delete(void* p);     // 0x004C73B0
};

struct CThingSwitch : CThingGameObject {
    float         TriggerRadius;                         // +0xd0
    CCharString   EnvironmentDef;                        // +0xd4
    float         TimeToChangeEnvironmentDef;            // +0xd8
    bool          Triggered;                             // +0xdc
    CCharString   TriggerableObjectScriptName;           // +0xe0
    long          TimesTriggered;                        // +0xe4
};

CThingSwitch* NewThingSwitch()
{
    return new CThingSwitch;
}