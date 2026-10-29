// CIntelligentPointer<T>::operator= (pointee unproven; the row is one of 24 byte-identical instance bodies)
// Leaf: ??4?$CIntelligentPointer@
// Smart-pointer copy assignment: SetPItem(rhs->GetPItem()); return this;

class CBaseObject;
class CBaseIntelligentPointer
{
public:
    CBaseObject* GetPItem() const;          // 0x00A01B50 (Ego_r.pdb)
    void SetPItem(CBaseObject* item);       // 0x00A01B90 (Ego_r.pdb)
};

class CThingCreatureBase;

class CIntelligentPointer : public CBaseIntelligentPointer
{
public:
    CIntelligentPointer* operator=(CIntelligentPointer* rhs);

    CThingCreatureBase* m_ptr;
};

// Out-of-line so the compiler emits the body into the .obj.
CIntelligentPointer* CIntelligentPointer::operator=(CIntelligentPointer* rhs)
{
    this->SetPItem(rhs->GetPItem());
    return this;
}
