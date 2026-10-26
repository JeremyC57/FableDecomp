// CThingVillage::CThingVillage() -- CThing constructed with type 4, the CThingGameObject layer (vtable 0x01244F7C, intelligent pointer at +0xc0), then CThingVillage's vtable and its CurrentGraphic (CEngineGraphic) zeroed
// Leaf: ??0CThingVillage
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005299C0, length 79 bytes.
// Callees: CThing::CThing(long type) @ 0x005296B0, CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10
// (then the CIntelligentPointer vtable 0x01238C6C). Own vtable 0x012451BC = CThingVillage (RTTI); the
// catalog label "CThingObject::CThingObject" is propagated (callers: 0x0083B80D and a jump at 0x0052AC82).
// Layout: PDB CThingVillage = CThingGameObject + CEngineGraphic CurrentGraphic (14 bytes, packed
// to 2) at retail +0xd0: BankIndex +0xd0, AnimStep +0xd4, RenderSizeX +0xd8, AdditiveAlpha +0xdc,
// Type +0xdd. Shape: the landed CThingSwitch 0x00529B00 layering; every zero store follows the
// derived vtable store, so CurrentGraphic is a member OBJECT with an inline constructor, all five
// fields from one `xor eax,eax`.
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
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {   // vtable 0x01238C6C
public:
    CIntelligentPointer() : CBaseIntelligentPointer() { }

    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};
class CThing { public:                                    // 0xc0 bytes
    virtual void v00();
    unsigned char _body[0xbc];
    CThing(long type);                             // 0x005296B0
};
class CThingGameObject : public CThing { public:                 // vtable 0x01244F7C
    virtual void v00();
    CIntelligentPointer<CThing> m_owner;                   // +0xc0
    unsigned char       _padc8[0x8];               // +0xc8
    CThingGameObject(long type) : CThing(type), m_owner() { }
};
#pragma pack(push, 2)
class CEngineGraphic { public:                            // 14 bytes (PDB size 14)
    unsigned long BankIndex;                       // +0x00
    float         AnimStep;                        // +0x04
    float         RenderSizeX;                     // +0x08
    bool          AdditiveAlpha;                   // +0x0c
    unsigned char Type;                            // +0x0d
    CEngineGraphic() : BankIndex(0), AnimStep(0), RenderSizeX(0), AdditiveAlpha(false), Type(0) { }
};
#pragma pack(pop)
class CThingVillage : public CThingGameObject { public:          // vtable 0x012451BC
    virtual void v00();
    CEngineGraphic CurrentGraphic;                 // +0xd0
    CThingVillage();
};

CThingVillage::CThingVillage()
    : CThingGameObject(4), CurrentGraphic()
{
}