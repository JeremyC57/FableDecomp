// CGameScriptThing::CGameScriptThing(CWorld*, CThing*) -- the script thing for an existing thing: base members as the empty constructor, then InitFromThing(thing)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008D0250, length 107 bytes (padded to 112).
// Callees: CBase::CBase @ 0x0099A2D0, CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10,
//          CCharString::CCharString @ 0x0099E4B0 (x2), CGameScriptThing::InitFromThing @ 0x008CFFE0.
// Vtables: CGameScriptThing 0x012852CC, CIntelligentPointer<CThing> 0x01238C6C.
// Shape: as 0x008CFF60 but the intelligent pointer is default-constructed (no SetPItem call) and the body is a
// single InitFromThing(thing) (which fills the name and the UID itself).
// Leaf: ??0CGameScriptThing
class CThing;
class CWorld;
namespace N_0099a2d0 { struct CBase {
    virtual void v00();
    CBase();                                       // 0x0099A2D0
}; } using N_0099a2d0::CBase;
struct CCountedPointerWords {                      // the script thing's counted-pointer pair (+0x04 / +0x08)
    void* m_pObject;
    void* m_pCount;
    CCountedPointerWords() : m_pObject(0), m_pCount(0) {}
};
struct __declspec(novtable) CScriptThing : CBase { // the script-facing interface: no vtable store of its own
    CCountedPointerWords m_ref;                    // +0x04
    CScriptThing() {}
};
struct CBaseIntelligentPointer {
    virtual void v00();
    void* m_pItem;                                 // +0x04
    CBaseIntelligentPointer();                     // 0x00A01B10
    void SetPItem(void* item);                     // 0x00A01B90
    CThing* GetPItem() const;                      // 0x00A01B50
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:
    CIntelligentPointer() {}
    CIntelligentPointer(T* item) { SetPItem(item); }
    virtual void v00();                            // own vtable (0x01238C6C)
};
class CCharString { public:
    void* m_pData;
    CCharString();                                 // 0x0099E4B0
    const CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};
extern CCharString g_CCharString_013bafcc;         // 0x013BAFCC (the default script-thing name)
class C3DVector { public:
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
};
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