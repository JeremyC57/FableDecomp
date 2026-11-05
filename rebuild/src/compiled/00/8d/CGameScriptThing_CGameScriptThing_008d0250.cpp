// CGameScriptThing::CGameScriptThing(CWorld*, CThing*) -- the script thing for an existing thing: base members as the empty constructor, then InitFromThing(thing)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008D0250, length 107 bytes (padded to 112).
// Callees: CBaseClass::CBaseClass @ 0x0099A2D0, CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10,
//          CCharString::CCharString @ 0x0099E4B0 (x2), CGameScriptThing::InitFromThing @ 0x008CFFE0.
// Vtables: CGameScriptThing 0x012852CC, CIntelligentPointer<CThing> 0x01238C6C.
// Shape: as 0x008CFF60 but the intelligent pointer is default-constructed (no SetPItem call) and the body is a
// single InitFromThing(thing) (which fills the name and the UID itself).
// Leaf: ??0CGameScriptThing
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
class CThing;
class CWorld;
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0 (slot 0), implicit copy
struct CCountedPointerWords {                      // the script thing's counted-pointer pair (+0x04 / +0x08)
    void* m_pObject;
    void* m_pCount;
    CCountedPointerWords() : m_pObject(0), m_pCount(0) {}
};
struct __declspec(novtable) CScriptThing : CBaseClass { // the script-facing interface: no vtable store of its own
    CCountedPointerWords m_ref;                    // +0x04
    CScriptThing() {}
};
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
class CCharString { public:
    void* m_pData;
    CCharString();                                 // 0x0099E4B0
    const CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};
extern CCharString g_CCharString_013bafcc;         // 0x013BAFCC (the default script-thing name)
#include "engine/C3DVector.h"
class CGameScriptThing : public CScriptThing { public:
    CIntelligentPointer<CThing> PThing;            // +0x0c
    long             ScriptCounter;                // +0x14
    unsigned __int64 ThingUID;                     // +0x18
    CCharString      Name;                         // +0x20
    CCharString      DataString;                   // +0x24
    C3DVector        Pos;                          // +0x28
    C3DVector        HomePos;                      // +0x34
    float            AngleXY;                      // +0x40
    CWorld*          World;                        // +0x44
    virtual void v00();                            // own vtable (0x012852CC)
    CGameScriptThing(CWorld* world);
    CGameScriptThing(CWorld* world, CThing* thing);
    void InitFromThing(CThing* thing);             // 0x008CFFE0
};

CGameScriptThing::CGameScriptThing(CWorld* world, CThing* thing)
    : ScriptCounter(0), Pos(0.0f, 0.0f, 0.0f), HomePos(0.0f, 0.0f, 0.0f), AngleXY(0.0f), World(world)
{
    InitFromThing(thing);
}