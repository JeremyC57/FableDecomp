// CScriptThing::CScriptThing() -- default constructor: the CBaseClass base, the CScriptThing vtable, and a null PImp counted pointer
// Leaf: ??0CScriptThing@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x006E7B40, 26 bytes, thiscall, plain `ret`, returns this.
// Identity: stores vtable 0x01238C8C (RTTI .?AVCScriptThing@@, direct base CBaseClass whose ctor 0x0099A2D0 it calls); Ego_r.pdb ??0CScriptThing@@QAE@XZ and member PImp (CCountedPointer<CScriptThing>, +4); 70 callers (by-value CScriptThing temporaries; agent950 F014 used it as the CScriptThing() member ctor).
// Shape (family K014, representative 0x006E7B40): out-of-line base ctor, then the vtable store, then the embedded
//   CCountedPointer's inline default ctor zeroing Data/Info through one zero register (a member object with its own
//   inline ctor stores after the vtable). Header debt: engine/CScriptThing.h (where present) flattens the vptr into data, so the
//   class is modelled locally from the PDB layout.
class CBaseClass {                                       // Ego_r.pdb / RTTI: the polymorphic engine root
public:
    CBaseClass();                                        // 0x0099A2D0
    virtual ~CBaseClass();                               // 0x0099A2E0
};
class CCPPointerInfo;
template <class T> class CCountedPointer {               // Ego_r.pdb: Data, Info
public:
    T*              Data;                                // +0x00
    CCPPointerInfo* Info;                                // +0x04
    CCountedPointer() : Data(0), Info(0) { }
};

class CScriptThing : public CBaseClass {
public:
    CScriptThing();
    virtual ~CScriptThing();
    CCountedPointer<CScriptThing> PImp;   // +0x04
};
CScriptThing::CScriptThing() { }