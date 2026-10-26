// CCreatureAction_DragonFireBlastBase::CCreatureAction_DragonFireBlastBase(const CCreatureAction_DragonFireBlastBase&) -- copy constructor: base CCombatActionBase(other), own vtable (0x01293F9C), PShot (CIntelligentPointer) and Line1 (CEnginePrimitiveHandle) copy-constructed inline
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00953D60, length 96 bytes.
// Callees: CCombatActionBase::CCombatActionBase(const CCombatActionBase&) @ 0x0062F3B0, CBaseIntelligentPointer::CBaseIntelligentPointer
// @ 0x00A01B10, CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CBaseIntelligentPointer::SetPItem @ 0x00A01B90.
// Shape: the landed CIntelligentPointer copy idiom (base ctor, its vtable 0x01238C6C, SetPItem(other.GetPItem())) for PShot at
// +0x114, then CEnginePrimitiveHandle's inline copy: vtable 0x0122F598 and the counted data pointer copied with an
// intrusive add-ref (`test ; mov ; je ; inc [p+4]`). Member names from the engine header (PDB +0x12c/+0x134; retail
// CCombatActionBase is 0x114 bytes). `ret 4`: one argument.
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

class CThing;
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {   // vtable 0x01238C6C
public:
    CIntelligentPointer(const CIntelligentPointer& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); }

    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};

class CEnginePersistentPrimitiveBase { public:     // engine/CEnginePersistentPrimitiveBase.h exists but is not used: only the intrusive count at +4 (behind the vtable) is touched
    virtual void v00();
    long m_references;                      // +0x04
};

class CEnginePrimitiveHandle { public:             // vtable 0x0122F598; {vptr, CIVCountedPointer Data}
    virtual void v00();
    CEnginePersistentPrimitiveBase* Data;   // +0x04
    CEnginePrimitiveHandle(const CEnginePrimitiveHandle& other) : Data(other.Data) { if (Data != 0) ++Data->m_references; }
};

class CCombatActionBase { public:                  // retail sizeof 0x114
    virtual void v00();
    unsigned char _body[0x114 - 4];
    CCombatActionBase(const CCombatActionBase& other);   // 0x0062F3B0
};

class CCreatureAction_DragonFireBlastBase : public CCombatActionBase { public:   // vtable 0x01293F9C; sizeof 0x124
    CIntelligentPointer<CThing>    PShot;           // +0x114 CIntelligentPointer<CThing>
    CEnginePrimitiveHandle Line1;           // +0x11c
    virtual void v00();
    CCreatureAction_DragonFireBlastBase(const CCreatureAction_DragonFireBlastBase& other);
};

CCreatureAction_DragonFireBlastBase::CCreatureAction_DragonFireBlastBase(const CCreatureAction_DragonFireBlastBase& other)
    : CCombatActionBase(other)
    , PShot(other.PShot)
    , Line1(other.Line1)
{
}