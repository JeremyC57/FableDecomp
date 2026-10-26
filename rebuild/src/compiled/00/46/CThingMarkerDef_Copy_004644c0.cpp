// CThingMarkerDef::Copy(const CDefClassBase* src) -- CThingBaseDef's copy, then the def's own Graphic (CEngineGraphic, 14 bytes), TriggerType and Properties
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004644C0, length 58 bytes (vtable 0x01233264 slot 19).
// Callees: CThingBaseDef::CopyBase(const CDefClassBase*) @ 0x00463950 (manifest "CThingBaseDef::operator=").
// Shape: the sibling 0x00464513 CThingVillageDef::Copy model -- the source is static_cast to
// the derived def, the base copy is called non-virtually, then the members are assigned in
// declaration order: the 14-byte CEngineGraphic (packed to 2, PDB size 14) becomes the
// `movsd x3 ; movsw` block copy through esi/edi, the two dwords move through eax. Retail
// CThingMarkerDef sits 4 bytes below the PDB (Graphic +0x70, TriggerType +0x80, Properties +0x84). Size-optimised TU: the speed
// form copies the record through edx/ax (as 0x0083BA20 does); `optimize("s")` gives the movs block.
#pragma optimize("s", on)
#pragma pack(push, 2)
class CEngineGraphic { public:                             // 14 bytes (PDB size 14)
    unsigned long BankIndex;                        // +0x00
    float         AnimStep;                         // +0x04
    float         RenderSizeX;                      // +0x08
    bool          AdditiveAlpha;                    // +0x0c
    unsigned char Type;                             // +0x0d
};
#pragma pack(pop)
class CDefClassBase {
public:
    virtual void Copy(const CDefClassBase* src) = 0;
};
class CThingBaseDef : public CDefClassBase {
public:
    void CopyBase(const CDefClassBase* src);        // 0x00463950
    unsigned char _pad04[0x6c];
};
class CThingMarkerDef : public CThingBaseDef {
public:
    CEngineGraphic Graphic;                         // +0x70
    long           TriggerType;                     // +0x80 (ESwitchTriggerType)
    unsigned long  Properties;                      // +0x84
    virtual void Copy(const CDefClassBase* src);
};

void CThingMarkerDef::Copy(const CDefClassBase* src)
{
    const CThingMarkerDef* s = static_cast<const CThingMarkerDef*>(src);
    CThingBaseDef::CopyBase(src);
    Graphic = s->Graphic;
    TriggerType = s->TriggerType;
    Properties = s->Properties;
}