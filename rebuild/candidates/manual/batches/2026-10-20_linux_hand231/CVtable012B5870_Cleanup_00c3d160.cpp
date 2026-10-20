// CVtable_012B5870::Cleanup() -- tear the (no-RTTI) library object down: clear its four slot lists, free its four element arrays, release the flagged entries of the shared global slot list, then delete its two owned objects
// Leaf: Cleanup
//
// Retail 0x00C3D160, 304 bytes incl. padding (plain `ret`). Identity: the catalog label "CTCFishingSpot::CleanupAndDestructor"
// is propagated -- the only caller is the destructor 0x00C3CB30 of the class whose constructor 0x00C3D7D0 stores the no-RTTI
// vtable 0x012B5870 (static instance 0x0143BB58, landed GetPair80 0x00C3D2D0); the landed destructor calls this body
// `Cleanup` (state 4, before the four slot members are destroyed).
// Callees: CSlot_00c3cb30::Clear() @ 0x00C3C930 (four members at +0x1c/+0x30/+0x44/+0x58), MSVCR71 operator delete
// @ 0x00BFE9BC (zero-count arrays), the shared slot's "has entries" test @ 0x00C3C980 (`m_count != 0 || m_index >= 0`;
// label "CTCTavernGame::IsBetting" is propagated) on the global slot @ 0x0143BC00, the entry release @ 0x00C3CDE0
// (`__cdecl`, landed as Callback_00C3CDE0), the +0x18 object's close member @ 0x00C3D2D0's neighbour 0x00C4E2D0 (label
// "CGuiWindow::OnResize" propagated, name descriptive).
// Shape: `if (p) { delete[] p; p = 0; }` for each array of polymorphic elements (VC7.1's vector delete: a zero count frees
// the block, else the virtual vector deleting destructor with 3); the global list walk caches `next` before the
// release; the flag is bit 2 of a dword read as `(unsigned char)(flags >> 2) & 1`; the +0x18 object is closed and then
// deleted (`delete` re-reads the member and keeps its own null test), the +4 object is a bare `delete p; p = 0;`.
struct CElement08 { virtual ~CElement08(); unsigned char _body[0xc]; };
struct CElement0C { virtual ~CElement0C(); unsigned char _body[0xc]; };
struct CElement10 { virtual ~CElement10(); unsigned char _body[0xc]; };
struct CElement14 { virtual ~CElement14(); unsigned char _body[0xc]; };

struct COwnedDevice {                               // the +0x18 object
    virtual ~COwnedDevice();
    void Close();                                   // 0x00C4E2D0
};
struct COwnedRoot {                                 // the +4 object
    virtual ~COwnedRoot();
};

struct SSlotEntry {                                 // an entry of the global slot's list
    unsigned char _pad00[8];
    SSlotEntry*   m_pNext;                          // +0x08
    unsigned char _pad0c[0x2c - 0x0c];
    unsigned long m_flags;                          // +0x2c (bit 2: release on cleanup)
    bool IsReleasable() const { return (unsigned char)(m_flags >> 2) & 1; }
};
extern "C" void __cdecl Callback_00C3CDE0(void* block);   // 0x00C3CDE0

struct CSlot_00c3cb30 {                             // 0x14 bytes
    virtual void v00();
    SSlotEntry* m_pHead;                            // +0x04
    void*       m_pTail;                            // +0x08
    long        m_index;                            // +0x0c
    long        m_count;                            // +0x10
    void Clear();                                   // 0x00C3C930
    bool HasEntries();                              // 0x00C3C980
};
extern CSlot_00c3cb30 g_slot_0143bc00;              // 0x0143BC00

struct CVtable_012B5870 {
    virtual void v00();
    COwnedRoot*   m_pRoot;                          // +0x04
    CElement08*   m_pArray08;                       // +0x08
    CElement0C*   m_pArray0C;                       // +0x0c
    CElement10*   m_pArray10;                       // +0x10
    CElement14*   m_pArray14;                       // +0x14
    COwnedDevice* m_pDevice;                        // +0x18
    CSlot_00c3cb30 m_slot1;                         // +0x1c
    CSlot_00c3cb30 m_slot2;                         // +0x30
    CSlot_00c3cb30 m_slot3;                         // +0x44
    CSlot_00c3cb30 m_slot4;                         // +0x58
    void Cleanup();
};

void CVtable_012B5870::Cleanup()
{
    if (m_pRoot == 0)
        return;
    m_slot1.Clear();
    m_slot2.Clear();
    m_slot3.Clear();
    m_slot4.Clear();
    if (m_pArray08) { delete[] m_pArray08; m_pArray08 = 0; }
    if (m_pArray0C) { delete[] m_pArray0C; m_pArray0C = 0; }
    if (m_pArray10) { delete[] m_pArray10; m_pArray10 = 0; }
    if (m_pArray14) { delete[] m_pArray14; m_pArray14 = 0; }
    if (g_slot_0143bc00.HasEntries()) {
        SSlotEntry* entry = g_slot_0143bc00.m_pHead;
        while (entry) {
            SSlotEntry* next = entry->m_pNext;
            if (entry->IsReleasable())
                Callback_00C3CDE0(entry);
            entry = next;
        }
    }
    if (m_pDevice) {
        m_pDevice->Close();
        delete m_pDevice;
        m_pDevice = 0;
    }
    delete m_pRoot;
    m_pRoot = 0;
}
