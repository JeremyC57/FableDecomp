// CBaseIntelligentPointer::~CBaseIntelligentPointer() -- set the vtable (0x0129C95C) and drop this pointer's share of the record
// Leaf: ??1CBaseIntelligentPointer@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A01C10.
// Identity: Ego_r.pdb + FableWin.pdb ??1CBaseIntelligentPointer@@UAE@XZ (virtual; slot 0 of 0x0129C95C is its
//   scalar deleting destructor 0x00A01C50). Callees: operator delete @ 0x00BFE9BC through the inlined ClearData 0x00A01B20.
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
    void ClearData() const                             // 0x00A01B20 (private const in Ego_r), inlined here
    {
        if (PData != 0) {
            PData->refcount--;
            if (PData->refcount == 0 && PData->object == 0)
                delete PData;
            PData = 0;
        }
    }
    mutable CBaseObjectPointer* PData;                 // +0x04
};

CBaseIntelligentPointer::~CBaseIntelligentPointer()
{
    ClearData();
}
