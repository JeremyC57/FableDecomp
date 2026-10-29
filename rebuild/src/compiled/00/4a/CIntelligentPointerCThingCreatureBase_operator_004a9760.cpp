// CIntelligentPointer<CThing>::operator= @ 0x004A9760 (instance settled in naming wave 7, DECISIONS_W7.tsv)
// Leaf: ??4?$CIntelligentPointer@
// Smart-pointer copy assignment: SetPItem(other.GetPItem()); return *this;

class CBaseObject;
class CBaseIntelligentPointer
{
public:
    CBaseObject* GetPItem() const;          // 0x00A01B50 (Ego_r.pdb)
    void SetPItem(CBaseObject* item);       // 0x00A01B90 (Ego_r.pdb)
};

class CThing;

template <class T> class CIntelligentPointer : public CBaseIntelligentPointer
{
public:
    CIntelligentPointer& operator=(const CIntelligentPointer& other);
};

template <class T> CIntelligentPointer<T>& CIntelligentPointer<T>::operator=(const CIntelligentPointer& other)
{
    SetPItem(other.GetPItem());
    return *this;
}

template class CIntelligentPointer<CThing>;
