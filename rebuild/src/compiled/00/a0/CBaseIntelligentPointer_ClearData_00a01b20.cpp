// CBaseIntelligentPointer::ClearData() const -- drop this pointer's share of the record: count down, free the record when no pointer and no object remain, forget it
// Leaf: ?ClearData@CBaseIntelligentPointer@@ABEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A01B20.
// Identity: Ego_r.pdb + FableWin.pdb ?ClearData@CBaseIntelligentPointer@@ABEXXZ (private, const: PData is mutable).
// Callees: operator delete @ 0x00BFE9BC.
class CBaseObject;
class CBaseObjectPointer {                             // Ego_r.pdb, 8 bytes
public:
    CBaseObject*  object;                              // +0x00 the observed object, 0 once it died
    unsigned long refcount;                            // +0x04 intelligent pointers sharing this record
};
class CBaseIntelligentPointer {                        // vtable 0x0129C95C
public:
    virtual ~CBaseIntelligentPointer();                // 0x00A01C10
private:
    void ClearData() const;                            // 0x00A01B20
    mutable CBaseObjectPointer* PData;                 // +0x04
};

void CBaseIntelligentPointer::ClearData() const
{
    if (PData != 0) {
        PData->refcount--;
        if (PData->refcount == 0 && PData->object == 0)
            delete PData;
        PData = 0;
    }
}
