class CThingBuilding;

struct CIntelligentPointer {
    CThingBuilding* p;
};

// callee at 0xa01b50: CIntelligentPointer<CThingBuilding>::Get / dereference
class CBaseObject;
class CBaseIntelligentPointer { public:
    CBaseObject* GetPItem() const;   // 0x00A01B50 (Ego_r.pdb ?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ)
};

bool __fastcall thingbuilding_ip_equal(CIntelligentPointer* a, CIntelligentPointer* b)
{
    return ((CBaseIntelligentPointer*)a)->GetPItem() == ((CBaseIntelligentPointer*)b)->GetPItem();
}