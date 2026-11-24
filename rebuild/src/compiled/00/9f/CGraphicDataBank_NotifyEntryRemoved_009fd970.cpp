// Leaf: ?NotifyEntryRemoved@CGraphicDataBank@@MAEXK@Z
class CGraphicDataBank { public:
    char pad[0x1dc];
    char* f1e0;
protected:
    virtual void NotifyEntryRemoved(unsigned long);
};

void CGraphicDataBank::NotifyEntryRemoved(unsigned long idx)
{
    CGraphicDataBank* bank = this;
    char* base = bank->f1e0;
    *(int*)(base + idx * 0x2c + 0x24) = -1;
}
// Flatten route (flatten_pilot v6, NF335): CGraphicDataBank_NotifyEntryRemoved -> CGraphicDataBank::NotifyEntryRemoved, the RTTI-placed PDB member; class model: the row's local class model.
