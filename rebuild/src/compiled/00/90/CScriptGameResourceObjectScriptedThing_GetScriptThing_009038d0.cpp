// CScriptGameResourceObjectScriptedThing::GetScriptThing() const -- the CScriptThing handle of the scripted thing when it exists and is not dead; else a null CScriptThing
// Leaf: ?GetScriptThing@CScriptGameResourceObjectScriptedThing@@UBE?AVCScriptThing@@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009038D0, length 75 bytes; vtable 0x0128D86C slot 12.
// PDB: ?GetScriptThing@CScriptGameResourceObjectScriptedThing@@UBE?AVCScriptThing@@XZ.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 three times (on PThing at +0x14, a CIntelligentPointer<CThing>),
// CThing::GetScriptThing @ 0x004C7E50 (Ego_r ?GetScriptThing@CThing@@QBE?AVCScriptThing@@XZ: it asks the script interface
// at [0x013B89FC]+4 to build the handle), CGameScriptThing::CreateNullThing @ 0x008CFF40 (static __fastcall, the null handle).
// Shape: the hidden result pointer is read late in each arm (edi in the live arm, esi in the null arm)
// because both arms construct it in place: `return thing->GetScriptThing();` forwards the caller's
// result slot, `return CGameScriptThing::CreateNullThing();` constructs into it directly (no copy, VC7.1 RVO of a temporary).
// Layout: engine/CScriptGameResourceObjectScriptedThing.h models the vptr as a data word, so it cannot declare this
// virtual member; the class is modelled locally with the header's offsets (ScriptInterface +0x10, PThing +0x14,
// ScriptID +0x1c, NullResource +0x20, ActionPriority +0x24; 0x28 bytes). The returned CScriptThing needs its out-of-line
// constructors, so it is a local model with engine/CScriptThing.h's {vftable, PImp_Data, PImp_Info} layout.
class CScriptThing {
public:
    void* __vftable;
    void* PImp_Data;
    void* PImp_Info;
    CScriptThing(const CScriptThing& other);
};
class CGameScriptThing : public CScriptThing {      // Ego_r.pdb: CGameScriptThing derives from CScriptThing
public:
    static CScriptThing __fastcall CreateNullThing(); // 0x008CFF40 (?CreateNullThing@CGameScriptThing@@SI?AVCScriptThing@@XZ)
};
class CThing {
public:
    unsigned char  _pad00[0x91];
    unsigned char  m_dead : 1;                     // +0x91 bit 0
    unsigned char  m_91_1 : 7;
    CScriptThing GetScriptThing() const;           // 0x004C7E50
};
class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
};
class CGameScriptInterface;
class CScriptGameResourceObjectScriptedThing {     // vtable 0x0128D86C, 0x28 bytes
public:
    virtual ~CScriptGameResourceObjectScriptedThing();
    unsigned char               _pad_0x04[0xc];    // +0x04 CBaseObject / CScriptGameResourceObjectBase members
    CGameScriptInterface*       ScriptInterface;   // +0x10
    CIntelligentPointer<CThing> PThing;            // +0x14
    long                        ScriptID;          // +0x1c
    bool                        NullResource;      // +0x20
    long                        ActionPriority;    // +0x24
    virtual CScriptThing GetScriptThing() const;   // slot 12
};

CScriptThing CScriptGameResourceObjectScriptedThing::GetScriptThing() const
{
    if (PThing.GetPItem()) {
        if (!PThing.GetPItem()->m_dead)
            return PThing.GetPItem()->GetScriptThing();
    }
    return CGameScriptThing::CreateNullThing();
}
