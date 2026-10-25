// CChildListLoader::ProcessAllDrivers() -- runs ProcessDriver(id, count) for every trigger driver in the source's +0x10 list (count = virtual slot 15, item = slot 13)
// Leaf: ProcessAllDrivers
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C2EAB0, 65 bytes, __fastcall `ret`. Descriptive name (no PDB covers this
//   library): the catalog's `ProcessAllEventTriggerDrivers` is kept in spirit. Caller: CChildListLoader::Load
//   0x00C2EB00 (step 4); the class name follows the landed CChildListLoader::AddChildren 0x00C2E270, which walks the
//   source's own (+0) list. Siblings 0x00C2EA30 / 0x00C2EA70 walk the +0x20 list the same way.
// Callee: ProcessDriver @ 0x00C2E5A0 (__fastcall, `ret 8`: looks the id up in the +0xC table, nothing when the count
//   word is 0).
// Shape: a rotated `for` over an unsigned index (edi); the source pointer (+0) is re-read for every list call; the
//   count word is widened to the dword parameter (movzx; an `unsigned short` parameter gives `xor ecx,ecx ; mov cx`)
//   and pushed before the id (arguments right to left).
struct STriggerDriver {
    long           m_id;                    // +0x00
    unsigned char  _pad04[8];
    unsigned short m_count;                 // +0x0C
};

struct CDriverList {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12();
    virtual STriggerDriver* GetAt(unsigned long index);   // slot 13 (+0x34)
    virtual void v14();
    virtual unsigned long GetCount();                     // slot 15 (+0x3C)
};

struct CLoadSource {
    unsigned char _pad00[0x10];             // +0x00 the child list
    CDriverList   m_drivers;                // +0x10
};

struct CChildListLoader {
    CLoadSource*  m_pSource;                // +0x00
    void*         m_pTarget;                // +0x04
    void*         m_pStatus;                // +0x08
    void ProcessDriver(long id, unsigned long count);     // 0x00C2E5A0
    void ProcessAllDrivers();
};

void CChildListLoader::ProcessAllDrivers()
{
    for (unsigned long i = 0; i < m_pSource->m_drivers.GetCount(); ++i) {
        STriggerDriver* driver = m_pSource->m_drivers.GetAt(i);
        ProcessDriver(driver->m_id, driver->m_count);
    }
}