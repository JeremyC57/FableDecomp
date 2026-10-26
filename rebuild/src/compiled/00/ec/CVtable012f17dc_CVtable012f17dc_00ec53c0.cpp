// CVtable_012f17dc::CVtable_012f17dc(CScriptDataBase* data, CGameScriptInterfaceBase* iface) -- quest-script constructor: CScriptBase(), the data/interface pair at +0x44/+0x40, own vtable, two empty scripted-thing handles at +0x48 and +0x58
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00EC53C0, length 84 bytes.
// Callees: CScriptBase::CScriptBase @ 0x00CB8110 (the manifest's "CHeroMorphDef" label is propagated),
// CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x0099A380 twice (each handle's base: vtable 0x0129A7D8, +4 nulled).
// Vtable 0x012F17DC has no RTTI locator (quest-script module); the chunk label "CV_MurderTwistFinalScript" is
// carried by five different constructors in the manifest (propagated), so the class is named by its vtable.
// Shape (landed 0x00CE1A00 model): the base runs on `this`, the first argument is stored at +0x44 and the
// second at +0x40 BEFORE this class's vtable store (an inlined `__declspec(novtable)` intermediate base), then
// the two member objects: each base constructor is called on `lea edi,[esi+N]`, the member vtable stored, and
// the two counted-pointer words zeroed from ebx by the inline constructor of an embedded {Data, Info} member object
// (plain init-list zeros would hoist the `xor` above the vtable store); `ret 8`, `this` in eax.
class CScriptBase { public:                               // 0x40 bytes, the quest-script root (landed 0x00CB8110)
    virtual void v00();
    unsigned char _body[0x3c];                     // +0x04..+0x3f
    CScriptBase();                                 // 0x00CB8110
};

struct __declspec(novtable) CScriptObject : CScriptBase {   // the argument-carrying intermediate base: both words stored before the derived vtable
    void* m_pInterface;                            // +0x40 (second argument, CGameScriptInterfaceBase*)
    void* m_pData;                                 // +0x44 (first argument, CScriptDataBase*)
    CScriptObject(void* data, void* iface) : CScriptBase(), m_pInterface(iface), m_pData(data) { }
};

struct CCountedPointerWords {                     // {Data, Info}: a member object so its zero stores follow the owner's vtable store
    void* Data;
    void* Info;
    CCountedPointerWords() : Data(0), Info(0) { }
};

class CBaseObject { public:                               // CBaseIntelligentPointer root (vtable 0x0129A7D8, +4 nulled)
    void* m_pIntelligentPointer;                   // +0x04
protected:
    CBaseObject();                                       // 0x0099A380
    virtual ~CBaseObject();                              // 0x0099A430
};

class CScriptGameResourceObjectScriptedThingBase : public CBaseObject { public:   // vtable 0x0127094C, 0x10 bytes (engine header: PImp_Data +8, PImp_Info +0xc)
    CCountedPointerWords PImp;                     // +0x08 Data, +0x0c Info
    virtual void v00();
    CScriptGameResourceObjectScriptedThingBase() : CBaseObject(), PImp() { }
};

struct CVtable_012f17dc : CScriptObject {          // vtable 0x012F17DC
    CScriptGameResourceObjectScriptedThingBase m_first;    // +0x48
    CScriptGameResourceObjectScriptedThingBase m_second;   // +0x58
    virtual void v00();
    CVtable_012f17dc(void* data, void* iface);
};

CVtable_012f17dc::CVtable_012f17dc(void* data, void* iface)
    : CScriptObject(data, iface), m_first(), m_second()
{
}