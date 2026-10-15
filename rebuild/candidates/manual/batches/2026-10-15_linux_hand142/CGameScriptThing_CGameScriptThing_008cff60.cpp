// CGameScriptThing::CGameScriptThing(CWorld*) -- the empty script thing: no thing, default name, zero positions, the owning world
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008CFF60, length 123 bytes (padded to 128).
// Callees: CBase::CBase @ 0x0099A2D0, CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10,
//          CBaseIntelligentPointer::SetPItem @ 0x00A01B90 (NULL), CCharString::CCharString @ 0x0099E4B0 (x2),
//          CCharString::operator= @ 0x0099EFB0 from the static default name @ 0x013BAFCC.
// Vtables: CGameScriptThing 0x012852CC, CIntelligentPointer<CThing> 0x01238C6C.
// Shape: the +4/+8 zero words stored BEFORE the class vtable belong to a `__declspec(novtable)` CScriptThing base whose
// member has an inline constructor; the intelligent pointer is built by its (CThing*) constructor with NULL; the name
// assignment and the 64-bit UID clear are body statements (after the init list).
// Leaf: ??0CGameScriptThing
struct CThing;
struct CWorld;
struct CBase {
    virtual void v00();
    CBase();                                       // 0x0099A2D0
};
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
template <class T> struct CIntelligentPointer : CBaseIntelligentPointer {
    CIntelligentPointer() {}
    CIntelligentPointer(T* item) { SetPItem(item); }
    virtual void v00();                            // own vtable (0x01238C6C)
};
struct CCharString {
    void* m_pData;
    CCharString();                                 // 0x0099E4B0
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};
extern CCharString g_CCharString_013bafcc;         // 0x013BAFCC (the default script-thing name)
struct C3DVector {
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
};
struct CGameScriptThing : CScriptThing {
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

CGameScriptThing::CGameScriptThing(CWorld* world)
    : PThing(0), ScriptCounter(0), Pos(0.0f, 0.0f, 0.0f), HomePos(0.0f, 0.0f, 0.0f), AngleXY(0.0f), World(world)
{
    Name = g_CCharString_013bafcc;
    ThingUID = 0;
}
