// Byte-exact reconstruction of operator==(CIntelligentPointer<CThingBuilding>&, CIntelligentPointer<CThingBuilding>&) @ 0x0065d673
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
class CThingBuilding;

struct CIntelligentPointer_CThingBuilding_ {
    CThingBuilding* Get();
};

// retail 0xa01b50 = CIntelligentPointer<CThingBuilding>::Get, this-in-ecx.
// modeled as __fastcall free fn (self in ecx). extern so a real call is emitted.

bool __fastcall operator_eq_CThingBuilding_(CIntelligentPointer_CThingBuilding_* lhs,
                                            CIntelligentPointer_CThingBuilding_* rhs)
{
    return ((CThingBuilding*)(((CBaseIntelligentPointer*)(lhs))->GetPItem())) == ((CThingBuilding*)(((CBaseIntelligentPointer*)(rhs))->GetPItem()));
}