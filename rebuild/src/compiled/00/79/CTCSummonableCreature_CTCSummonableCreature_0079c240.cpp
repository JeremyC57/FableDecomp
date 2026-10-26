// CTCSummonableCreature::CTCSummonableCreature(CThing* parent) -- thing-component base, null def, not yet summoned, empty summoner pointer, then fetch the summonable-creature sub-def from the parent thing's def
// Leaf: ??0CTCSummonableCreature
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0079C240, length 70 bytes.
// Callees: CTCBase::CTCBase(CThing*) @ 0x00686800, CBaseIntelligentPointer::CBaseIntelligentPointer() @ 0x00A01B10,
// CDefClassBase::GetSubDef<CSummonableCreatureDef>(CDefPointer<T>*) const @ 0x0079C4B0 (manifest
// "GetSubDef<CAICreatureWillPowerIndicatorDef>" names a byte-identical instantiation).
// Vtables: 0x0126C41C = CTCSummonableCreature, 0x0123E7D8 = CIntelligentPointer (rtti-vtables.tsv).
// Layout: Ego_r.pdb CTCSummonableCreature (0x1c bytes): PDef (CDefPointer<CSummonableCreatureDef const>) +0x0c,
// HasBeenSummoned +0x10, Summoner (CIntelligentPointer<CThingCreatureBase>) at the UNALIGNED +0x11; the thing's counted
// def pointer is at +0x70 (as CTCAISummon::OnInitialActivate 0x007FDE70).
// Shape: vtable first, then the member constructors in order (the def pointer's inline constructor stores an immediate
// zero, the bool initialiser, the intelligent pointer's base call + vtable), then the body's GetSubDef into PDef.
class CThing;
class CBaseObject;

class CTCBase { public:
    virtual void v00();
    CThing* ParentThing;                                    // +0x04
    bool    ToBeRemoved;                                    // +0x08
    bool    IsInGlobalUpdateTCs;                            // +0x09
    CTCBase(CThing* parent);                                // 0x00686800
};

class CSummonableCreatureDef;

struct CDefPointerSummonable {                              // CDefPointer<CSummonableCreatureDef const>
    const CSummonableCreatureDef* Object;
    CDefPointerSummonable() : Object(0) { }
};

struct CThingDef {
    bool GetSubDef(CDefPointerSummonable* out) const;       // 0x0079C4B0
};

class CThing { public:
    unsigned char _pad[0x70];
    CThingDef*    PDef;                                     // +0x70 (counted def pointer: object word)
};

#pragma pack(push, 1)
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

class CThingCreatureBase;
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {      // CIntelligentPointer<CThingCreatureBase>, vtable 0x0123E7D8
public:
    CIntelligentPointer() : CBaseIntelligentPointer() { }

    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};

class CTCSummonableCreature : public CTCBase { public:
    CDefPointerSummonable PDef;                             // +0x0c
    bool                  HasBeenSummoned;                  // +0x10
    CIntelligentPointer<CThingCreatureBase>   Summoner;                         // +0x11
    virtual void v00();                                     // own vtable 0x0126C41C
    CTCSummonableCreature(CThing* parent);
};
#pragma pack(pop)

CTCSummonableCreature::CTCSummonableCreature(CThing* parent)
    : CTCBase(parent)
    , HasBeenSummoned(false)
{
    ParentThing->PDef->GetSubDef(&PDef);
}