// CThingGameObject::ConstructFromParams(long defIndex, const C3DVector* pos, long a3, unsigned long a4, const CCharString& scriptName, CThingPhysical* creator, CThingPhysical* originalCreator, bool a8) -- fail for a non-positive def index, else run CThingPhysical::ConstructFromParams with the other seven arguments and, on success, adopt a non-empty script name (CThing::ScriptName, +0x74)
// Leaf: ConstructFromParams
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008388D0, length 96 bytes.
// Callees: CThingPhysical::ConstructFromParams @ 0x006A5950 (7 arguments, `ret 1Ch`), CCharString::operator!=(const
// char*) const @ 0x0099E960 (literal 0x0122D70E = ""), CCharString::operator= @ 0x0099EFB0.
// Layout: local model. CThing::ScriptName sits at retail +0x74 (PDB +0x84 less the retail CThing's 0x10 narrower head;
// the def pointer at +0x70 matches the landed getters 0x00838B20/0x00838B60). engine/CThingGameObject.h only pads this
// region, so the base call and the name member are modelled locally. Parameter names past the fifth follow the
// manifest's CThingPhysical prototype; the 3rd/4th/8th are not established (descriptive).
// Shape: the fifth argument is not forwarded (the pushes skip [esp+18h]); `if (index > 0 && Base(...)) { if (name !=
// "") ScriptName = name; return true; } return false;`.
class C3DVector;
struct CThingPhysicalArg;

class CCharString { public:
    void* m_pStringData;
    bool operator!=(const char* text) const;        // 0x0099E960
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};

class CThingPhysical { public:
    bool ConstructFromParams(long defIndex, const C3DVector* pos, long a3, unsigned long a4,
                             CThingPhysicalArg* creator, CThingPhysicalArg* originalCreator, bool a7);   // 0x006A5950
};

class CThingGameObject : public CThingPhysical { public:
    unsigned char _pad00[0x74];
    CCharString   ScriptName;                       // +0x74
    bool ConstructFromParams(long defIndex, const C3DVector* pos, long a3, unsigned long a4, const CCharString& scriptName,
                             CThingPhysicalArg* creator, CThingPhysicalArg* originalCreator, bool a8);
};

bool CThingGameObject::ConstructFromParams(long defIndex, const C3DVector* pos, long a3, unsigned long a4,
                                           const CCharString& scriptName, CThingPhysicalArg* creator,
                                           CThingPhysicalArg* originalCreator, bool a8)
{
    if (defIndex > 0 && CThingPhysical::ConstructFromParams(defIndex, pos, a3, a4, creator, originalCreator, a8)) {
        if (scriptName != "")
            ScriptName = scriptName;
        return true;
    }
    return false;
}