// CThingPhysicalSwitch::Alloc() -- the class-registry factory for CThingPhysicalSwitch: allocate and construct a physical switch (thing type 12): the physical/game-object base chain and an empty triggerable-object script name; null when the thing manager refuses the allocation
// Leaf: ?Alloc@CThingPhysicalSwitch
// Retail: Fable.exe (TLC, VC7.1) @ 0x0052AD50, 80 bytes, `ret` (Ego_r.pdb `?Alloc@CThingPhysicalSwitch@@SIPAVCClass@@XZ`: static __fastcall,
//   no arguments, returns CClass*).
// Callees: CThing::operator new(unsigned int, bool) 0x004C7380 (landed; size 0xd8, flag false), CThingPhysical::
//   CThingPhysical(long type) 0x005296B0 (type 12), CBaseIntelligentPointer::CBaseIntelligentPointer 0x00A01B10, CCharString::CCharString 0x0099E4B0.
// Vtables: CThingGameObject 0x01244F7C, CIntelligentPointer<CThing> 0x01238C6C, CThingPhysicalSwitch 0x01245644 (rtti-vtables.tsv).
// Callers: a call at 0x0083DBEB (the CThingPhysicalSwitch TU) and the class-registry reference at 0x00523155
//   (CThingManager TU).
// Layout: Ego_r.pdb CThingPhysicalSwitch (0xf0) has Triggered +0xe8, TriggerableObjectScriptName +0xec; retail is 0x18
//   bytes narrower: +0xd0 / +0xd4, 0xd8 bytes. Local retail model (engine/CThingGameObject.h is the PDB layout).
// Shape: `new (false) T` through CThing's protected static __fastcall operator new(size, bool); the CCharString member's
//   out-of-line default ctor (0x0099E4B0) runs after the derived vtable store; Triggered stays uninitialised.
class CCharString {
public:
    void* PData;                                // CBasicString<char>* (4 bytes)
    CCharString();                              // 0x0099E4B0
};

class CClass {                                  // the reflection root every thing class derives from (vptr owner)
public:
    virtual void v00();
};

struct CBaseIntelligentPointer {                // 8 bytes: vptr + the pointee link
    virtual void v00();
    void* m_pItem;                              // +0x04
    CBaseIntelligentPointer();                  // 0x00A01B10
};

struct CThingPointer : CBaseIntelligentPointer {            // CIntelligentPointer<CThing>, vtable 0x01238C6C
    virtual void v00();
    CThingPointer() { }
};

class CThing : public CClass {
protected:
    static void* __fastcall operator new(unsigned int size, bool flag);   // 0x004C7380 (null when the manager refuses)
};

class CThingPhysical : public CThing {
public:
    unsigned char _pad04[0xbc];                 // +0x04 .. +0xbf (CThing + CThingPhysical members, not touched here)
    CThingPhysical(long type);                  // 0x005296B0
};

class CThingGameObject : public CThingPhysical {           // vtable 0x01244F7C
public:
    CThingPointer PThingStandingOn;             // +0xc0
    unsigned char m_gameObjectFlags;            // +0xc8 (bitfield group, not initialised here)
    unsigned char _padc9[7];                    // +0xc9 tail padding (the class is 8-aligned through its 64-bit UID)
    virtual void v00();
    CThingGameObject(long type) : CThingPhysical(type) { }
};

class CThingPhysicalSwitch : public CThingGameObject {          // vtable 0x01245644
public:
    bool Triggered;                             // +0xd0 (not initialised)
    CCharString TriggerableObjectScriptName;    // +0xd4
    virtual void v00();
    CThingPhysicalSwitch() : CThingGameObject(12) { }
    static CClass* __fastcall Alloc();
};

CClass* __fastcall CThingPhysicalSwitch::Alloc()
{
    return new (false) CThingPhysicalSwitch;
}