// CIntelligentPointer<CThingCreatureBase>::operator=
// Leaf: ??4?$CIntelligentPointer@
// retail 0x00668b80

class CThingCreatureBase;

class CBaseObject;
class CBaseIntelligentPointer { public:
    CBaseObject* GetPItem() const;   // 0x00A01B50 (Ego_r.pdb ?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ)
};
class CIntelligentPointer
{
public:
    CIntelligentPointer &operator=(CIntelligentPointer &other);
    // 0xa01b50: read the referenced object out of 'other'
    // 0xa01b90: assign the referenced object into 'this'
    void Assign(CThingCreatureBase *obj);

    void *m_data;
};

CIntelligentPointer &CIntelligentPointer::operator=(CIntelligentPointer &other)
{
    Assign(((CThingCreatureBase*)((CBaseIntelligentPointer*)&other)->GetPItem()));
    return *this;
}