// CThingPhysicalSwitch::~CThingPhysicalSwitch() -- stores the class vtable, destroys TriggerableObjectScriptName (+0xd4), then chains to CThingGameObject::~CThingGameObject (tail jump)
// Leaf: ??1CThingPhysicalSwitch@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0083D6D0, 28 bytes: push esi ; mov esi,ecx ; lea ecx,[esi+0D4h] ;
//   mov [esi],0x01245644 ; call ~CCharString 0x0099EAE0 ; mov ecx,esi ; pop esi ; jmp 0x00838A00.
// Identity (proven): the only caller is 0x00529CA0 (call at 0x00529CA3), slot 0 (scalar deleting destructor) of vtable
//   0x01245644, whose RTTI is .?AVCThingPhysicalSwitch@@ (rtti-vtables.tsv: slots 2 Save 0x0083D800, 3 SerialiseGameState
//   0x0083D940, 8 OnCreate 0x0083D6F0 ... all landed CThingPhysicalSwitch members); the body stores that same vtable.
//   Ego_r.pdb / FableWin.pdb: ??1CThingPhysicalSwitch@@UAE@XZ (user-declared virtual destructor, so the vtable store).
//   RTTI base: CThingPhysicalSwitch : CThingGameObject; 0x00838A00 is ??1CThingGameObject@@UAE@XZ (bound so).
//   The manifest label "CInputProcessManager::~CInputProcessManager" is propagated from the byte-identical-shape twin family.
// Callees: CCharString::~CCharString 0x0099EAE0, CThingGameObject::~CThingGameObject 0x00838A00 (tail jump).
// Layout: Ego_r.pdb CThingPhysicalSwitch (240 bytes): Triggered +0xe8, TriggerableObjectScriptName +0xec; retail sits 0x18
//   lower (+0xd0 / +0xd4; the landed CThingPhysicalSwitch::Save 0x0083D800 model), so CThingGameObject is 0xd0 bytes in
//   retail. engine/CThingGameObject.h flattens the vptr into a data member, so the polymorphic base with its out-of-line
//   destructor is modelled locally over the same size.
class CCharStringData;
class CCharString {
public:
    ~CCharString();                                     // 0x0099EAE0
    CCharStringData* PStringData;
};

class CThingGameObject {
public:
    virtual ~CThingGameObject();                        // 0x00838A00
private:
    unsigned char _pad_0x04[0xd0 - 4];
};

class CThingPhysicalSwitch : public CThingGameObject {
public:
    virtual ~CThingPhysicalSwitch();
    bool        Triggered;                              // +0xd0
    CCharString TriggerableObjectScriptName;            // +0xd4
};

CThingPhysicalSwitch::~CThingPhysicalSwitch()
{
}