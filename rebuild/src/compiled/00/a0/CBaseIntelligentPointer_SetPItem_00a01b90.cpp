// CBaseIntelligentPointer::SetPItem(CBaseObject* item) -- point this intelligent pointer at `item`: keep the shared record when it already observes the same live object, otherwise drop the old record and join (or create) the object's record
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A01B90, 123 bytes (+5 int3), __fastcall, ret 4.
// Callees: operator delete 0x00BFE9BC and operator new 0x00BFEA1A (both __cdecl, `add esp,4`).
// Layout: as the landed CBaseIntelligentPointer rows (ClearData 0x00A01B20, the destructor 0x00A01C10): vptr,
// m_pData (+4) -> CBaseObjectPointer {PItem, RefCount}; Ego_r.pdb CBaseObject keeps its record in
// IntelligentPointer (+4, behind the vptr).
// Shape: the early-out (same record object and a non-null item) is tested first; ClearData (the same
// translation unit, inlined) repeats its own null test; joining an existing record stores the member and
// bumps the count through a register; a new record (8 bytes, `new` of a POD: no null check) is filled
// through the re-read member and published back into the object.
struct CBaseObjectPointer;

struct CBaseObject {
    virtual void v00();
    CBaseObjectPointer* IntelligentPointer;        // +0x04
};

struct CBaseObjectPointer {
    CBaseObject* PItem;                            // +0x00 the observed object, 0 once it died
    long         RefCount;                         // +0x04 intelligent pointers sharing this record
};

struct CBaseIntelligentPointer {
    virtual void v00();
    CBaseObjectPointer* m_pData;                   // +0x04
    void ClearData()                               // 0x00A01B20 (landed), inlined here
    {
        if (m_pData != 0) {
            m_pData->RefCount--;
            if (m_pData->RefCount == 0 && m_pData->PItem == 0)
                delete m_pData;
            m_pData = 0;
        }
    }
    void SetPItem(CBaseObject* item);
};

void CBaseIntelligentPointer::SetPItem(CBaseObject* item)
{
    if (m_pData != 0 && m_pData->PItem == item && item != 0)
        return;
    ClearData();
    if (item != 0) {
        CBaseObjectPointer* data = item->IntelligentPointer;
        if (data != 0) {
            m_pData = data;
            data->RefCount++;
            return;
        }
        m_pData = new CBaseObjectPointer;
        m_pData->PItem = item;
        m_pData->RefCount = 1;
        item->IntelligentPointer = m_pData;
    }
}