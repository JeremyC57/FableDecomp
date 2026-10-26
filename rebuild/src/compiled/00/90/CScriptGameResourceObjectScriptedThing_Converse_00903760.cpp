// CScriptGameResourceObjectScriptedThing::Converse(const CScriptGameResourceObjectScriptedThingBase& other, const char* text) -- speak the text group at the other scripted thing
// Leaf: ?Converse@CScriptGameResourceObjectScriptedThing@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00903760, length 120 bytes (+8 int3 padding).
// PDB: ?Converse@CScriptGameResourceObjectScriptedThing@@UAEXABVCScriptGameResourceObjectScriptedThingBase@@PBD@Z.
// Callees: other's virtual GetScriptThing() const (slot 12, +0x30; CScriptThing by value through a hidden pointer),
// this's virtual Speak(const CScriptThing&, const char*, ETextGroupSelectionMethod, bool, bool, bool) (slot 13,
// +0x34; vtable 0x0128D86C), CBase::~CBase @ 0x0099A2E0 (the temporary's base).
// Shape: `Speak(other.GetScriptThing(), text, method 0, true, true, false)`: the constant arguments and text are
// pushed first, this's vtable is read before the nested call; the CScriptThing temporary (vtable 0x01238C8C,
// CCountedPointer PImp at +4) dies inline: vptr store, counted release, both PImp words zeroed, base dtor call.
// Model: local polymorphic model (engine/CScriptGameResourceObjectScriptedThing.h holds the vptr as a plain word).
struct SCountBlock {
    long References;                                 // +0
    void (__fastcall* Destroy)(void*);               // +4
    void* Object;                                    // +8
};

template <class T> class CCountedPointer { public:
    T* Data;
    SCountBlock* Info;
    ~CCountedPointer()
    {
        if (Info)
        {
            --Info->References;
            if (Info->References == 0)
            {
                Info->Destroy(Info->Object);
                operator delete(Info);
            }
        }
        Data = 0;
        Info = 0;
    }
};

class CBase {
public:
    virtual ~CBase();                                // 0x0099A2E0
};

class CScriptThingImp;

class CScriptThing : public CBase {                  // vtable 0x01238C8C
public:
    virtual ~CScriptThing() {}
    CCountedPointer<CScriptThingImp> PImp;           // +0x04
};

enum ETextGroupSelectionMethod { TEXT_GROUP_SELECT_RANDOM = 0 };

class CScriptGameResourceObjectScriptedThingBase {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual CScriptThing GetScriptThing() const;     // slot 12 (+0x30)
    virtual void Speak(const CScriptThing& listener, const char* textGroup, ETextGroupSelectionMethod method,
                       bool b1, bool b2, bool b3);   // slot 13 (+0x34)
};

class CScriptGameResourceObjectScriptedThing : public CScriptGameResourceObjectScriptedThingBase {
public:
    virtual void Converse(const CScriptGameResourceObjectScriptedThingBase& other, const char* text);
};

void CScriptGameResourceObjectScriptedThing::Converse(const CScriptGameResourceObjectScriptedThingBase& other, const char* text)
{
    Speak(other.GetScriptThing(), text, TEXT_GROUP_SELECT_RANDOM, true, true, false);
}