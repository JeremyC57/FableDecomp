// CIntelligentPointer<CThingCreatureBase>::operator= (retail 0x0050f3d0)
// Leaf: ??4?$CIntelligentPointer@
class CThingCreatureBase;

class CBaseObject;
class CBaseIntelligentPointer {
public:
    CBaseObject* GetPItem() const;   // 0x00A01B50 (Ego_r.pdb ?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ)
private: public:
    void SetPItem(CBaseObject*a0);   // 0x00A01B90 (Ego_r.pdb ?SetPItem@CBaseIntelligentPointer@@QAEXPAVCBaseObject@@@Z)
};
class CIntelligentPointer_CThingCreatureBase
{

    CIntelligentPointer_CThingCreatureBase* operator=(
        CIntelligentPointer_CThingCreatureBase* rhs);
};

CIntelligentPointer_CThingCreatureBase*
CIntelligentPointer_CThingCreatureBase::operator=(
    CIntelligentPointer_CThingCreatureBase* rhs)
{
    ((CBaseIntelligentPointer*)this)->SetPItem((CBaseObject*)(((CThingCreatureBase*)((CBaseIntelligentPointer*)rhs)->GetPItem())));
    return this;
}