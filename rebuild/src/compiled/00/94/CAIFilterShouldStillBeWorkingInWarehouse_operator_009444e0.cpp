// CAIFilter_ShouldStillBeWorkingInWarehouse::operator()() -- true while the crate and the warehouse are live, a carried crate still has a put-down task, the crate accepts the warehouse and the village (if any) is in its work hours
// Leaf: ??RCAIFilter_ShouldStillBeWorkingInWarehouse
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009444E0, length 117 bytes (vtable 0x01292A8C slot 0).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (re-fetched at every use, five calls),
// CTCVillage::IsThereACratePutDownTask @ 0x00658E09, CTCVillage::IsCookTime @ 0x00658B20 (manifest
// labels, kept as hints), the crate's virtual slot 7 (+0x1c) taking the warehouse thing.
// Shape: nested ifs with one `return true` and one shared `return false` (early `return false` tests
// duplicate the tail and push ebx late); the crate pointer member's
// address is formed once into esi, the warehouse pointer's into ebx; the slot-7 call fetches the vtable
// before the argument's GetPItem (plain nested call). Layout from the constructor 0x00944470.
struct CBaseObject;

struct CCrateThing {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void v04(); virtual void v05(); virtual void v06();
    virtual bool CanBePutIn(CBaseObject* place);        // slot 7 (+0x1c)
    unsigned char _pad[0x14];
    unsigned char m_bCarried;                           // +0x18
};

struct CTCVillage {
    bool IsThereACratePutDownTask();                    // 0x00658E09
    bool IsCookTime_00658b20();                                  // 0x00658B20
};

struct CBaseIntelligentPointer {
    virtual void v00();
    void* m_pItem;                                      // +0x04
    CCrateThing* GetPItem() const;                      // 0x00A01B50
};

struct CAIFilter_ShouldStillBeWorkingInWarehouse {
    CTCVillage*             m_pVillage;                 // +0x04
    CBaseIntelligentPointer m_pCrate;                   // +0x08
    CBaseIntelligentPointer m_pWarehouse;               // +0x10
    virtual bool operator()();
};

bool CAIFilter_ShouldStillBeWorkingInWarehouse::operator()()
{
    if (m_pCrate.GetPItem() != 0 && m_pWarehouse.GetPItem() != 0) {
        if (!m_pCrate.GetPItem()->m_bCarried || m_pVillage->IsThereACratePutDownTask()) {
            if (m_pCrate.GetPItem()->CanBePutIn((CBaseObject*)m_pWarehouse.GetPItem())) {
                if (m_pVillage == 0 || m_pVillage->IsCookTime_00658b20())
                    return true;
            }
        }
    }
    return false;
}