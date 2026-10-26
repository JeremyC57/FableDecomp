// CTCObjectAugmentations::GetFreeSlot() const -- index of the first empty augmentation slot, or -1
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00765D50, length 47 bytes.
// Retail CTCObjectAugmentations (PDB): PDef +0x0c, LoadedFromSaveGame +0x10, Augmentations (std::vector<CDefPointer<const
// CThingBaseDef> >, 12 bytes in retail) +0x14.
// Shape: an unsigned index loop against size() re-read every iteration (`sar` of the pointer difference, `jb`), the
// element walked by a strength-reduced pointer started from the first load of m_pFirst.
class CThingBaseDef;

struct CThingBaseDefPointer {                       // CDefPointer<const CThingBaseDef>
    const CThingBaseDef* m_pDef;
};

struct CAugmentationVector {
    CThingBaseDefPointer* m_pFirst;
    CThingBaseDefPointer* m_pLast;
    CThingBaseDefPointer* m_pEnd;
    unsigned long size() const { return m_pLast - m_pFirst; }
    const CThingBaseDefPointer& operator[](unsigned long i) const { return m_pFirst[i]; }
};

struct CTCBase_00765d50 {
    virtual void v00();
    unsigned char _pad04[8];
};

class CTCObjectAugmentations : public CTCBase_00765d50 { public:
    const void*         PDef;                       // +0x0c
    bool                LoadedFromSaveGame;         // +0x10
    CAugmentationVector Augmentations;              // +0x14
    long GetFreeSlot() const;
};

long CTCObjectAugmentations::GetFreeSlot() const
{
    for (unsigned long i = 0; i < Augmentations.size(); ++i) {
        if (Augmentations[i].m_pDef == 0)
            return i;
    }
    return -1;
}