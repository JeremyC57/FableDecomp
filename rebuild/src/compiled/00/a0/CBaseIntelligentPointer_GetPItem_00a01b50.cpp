// CBaseIntelligentPointer::GetPItem() const -- the observed object, or 0 (dropping a dead record on the way)
// Leaf: ?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A01B50, oracle length 54 bytes.
// Identity: Ego_r.pdb + FableWin.pdb ?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ (const; the inlined
//   private const ClearData 0x00A01B20 writes the mutable PData).
// Callees: operator delete @ 0x00BFE9BC (__cdecl, `add esp,4`), through the inlined ClearData.
class CBaseObject;
class CBaseObjectPointer {                             // Ego_r.pdb, 8 bytes
public:
    CBaseObject*  object;                              // +0x00 the observed object, 0 once it died
    unsigned long refcount;                            // +0x04 intelligent pointers sharing this record
};
class CBaseIntelligentPointer {                        // vtable 0x0129C95C
public:
    virtual ~CBaseIntelligentPointer();                // 0x00A01C10
    CBaseObject* GetPItem() const;                     // 0x00A01B50
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

CBaseObject* CBaseIntelligentPointer::GetPItem() const
{
    if (PData != 0) {
        if (PData->object != 0)
            return PData->object;
        ClearData();
    }
    return 0;
}
