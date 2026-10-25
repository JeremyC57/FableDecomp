// CBankProcessor::ProcessAllDrivers() -- runs AddDriverToGroup(number, group) for every driver record in the bank data's +0x10 list (count = virtual slot 15, item = slot 13)
// Leaf: ?ProcessAllDrivers@CBankProcessor@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C2EAB0, 65 bytes, __fastcall `ret`.
// Naming (sweep1, 2026-10-25): was landed as CChildListLoader::ProcessAllDrivers (chunk 437); the class is the libpk
//   bank processor (chunk 436): its caller is CBankProcessor::Load 0x00C2EB00 (step 4, see that row) and its callee
//   0x00C2E5A0 is the landed CBankProcessor::AddDriverToGroup. Descriptive method name (no PDB covers libpk; the
//   catalog's `ProcessAllEventTriggerDrivers` is kept in spirit). Siblings AddEvents 0x00C2EA30 / AddSubEvents 0x00C2EA70
//   walk the +0x20 list the same way.
// Callee: CBankProcessor::AddDriverToGroup @ 0x00C2E5A0 (`ret 8`: finds or creates the group, nothing when the group
//   word is 0).
// Shape: a rotated `for` over an unsigned index (edi); the source pointer (+0) is re-read for every list call; the
//   count word is widened to the dword parameter (movzx; an `unsigned short` parameter gives `xor ecx,ecx ; mov cx`)
//   and pushed before the id (arguments right to left).
struct SDriverRecord {                      // the bank's driver record (CBankProcessor::AddDriver 0x00C2E2B0 reads the rest)
    unsigned long  m_index;                 // +0x00 the driver number
    unsigned char  _pad04[8];
    unsigned short m_group;                 // +0x0C 0: no group
};

struct CDriverList {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12();
    virtual SDriverRecord* GetAt(unsigned long index);   // slot 13 (+0x34)
    virtual void v14();
    virtual unsigned long GetCount();                     // slot 15 (+0x3C)
};

struct CBankData {
    unsigned char _pad00[0x10];             // +0x00 the sample list
    CDriverList   m_drivers;                // +0x10
};

struct CBankProcessor {
    CBankData*    m_pData;                  // +0x00
    void*         m_pManager;               // +0x04
    void*         m_pLog;                   // +0x08
    void AddDriverToGroup(unsigned long number, unsigned long group);   // 0x00C2E5A0
    void ProcessAllDrivers();
};

void CBankProcessor::ProcessAllDrivers()
{
    for (unsigned long i = 0; i < m_pData->m_drivers.GetCount(); ++i) {
        SDriverRecord* driver = m_pData->m_drivers.GetAt(i);
        AddDriverToGroup(driver->m_index, driver->m_group);
    }
}