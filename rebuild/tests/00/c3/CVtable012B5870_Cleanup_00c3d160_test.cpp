// Behaviour test for CVtable_012B5870::Cleanup (retail 0x00C3D160): model copy + stub callees.
#include <cstdio>

static int g_elementDtors = 0;
static int g_rootDtors = 0;
static int g_deviceDtors = 0;
static int g_closeCalls = 0;
static int g_clearCalls = 0;
static int g_released = 0;

struct CElement08 { virtual ~CElement08() { ++g_elementDtors; } };
struct CElement0C { virtual ~CElement0C() { ++g_elementDtors; } };
struct CElement10 { virtual ~CElement10() { ++g_elementDtors; } };
struct CElement14 { virtual ~CElement14() { ++g_elementDtors; } };

struct COwnedDevice {
    virtual ~COwnedDevice() { ++g_deviceDtors; }
    void Close() { ++g_closeCalls; }
};
struct COwnedRoot {
    virtual ~COwnedRoot() { ++g_rootDtors; }
};

struct SSlotEntry {
    SSlotEntry*   m_pNext;
    unsigned long m_flags;
    bool IsReleasable() const { return (unsigned char)(m_flags >> 2) & 1; }
};
static SSlotEntry* g_releasedList[8];
extern "C" void Callback_00C3CDE0(void* block) { g_releasedList[g_released++] = (SSlotEntry*)block; }

struct CSlot_00c3cb30 {
    SSlotEntry* m_pHead;
    long        m_index;
    long        m_count;
    void Clear() { ++g_clearCalls; }
    bool HasEntries() { return m_count != 0 || m_index >= 0; }
};
CSlot_00c3cb30 g_slot_0143bc00;

struct CVtable_012B5870 {
    COwnedRoot*   m_pRoot;
    CElement08*   m_pArray08;
    CElement0C*   m_pArray0C;
    CElement10*   m_pArray10;
    CElement14*   m_pArray14;
    COwnedDevice* m_pDevice;
    CSlot_00c3cb30 m_slot1;
    CSlot_00c3cb30 m_slot2;
    CSlot_00c3cb30 m_slot3;
    CSlot_00c3cb30 m_slot4;
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

int main()
{
    int fail = 0;
    SSlotEntry e3 = { 0, 4 }, e2 = { &e3, 3 }, e1 = { &e2, 0x7 };
    g_slot_0143bc00.m_pHead = &e1; g_slot_0143bc00.m_index = -1; g_slot_0143bc00.m_count = 3;

    CVtable_012B5870 o;
    o.m_pRoot = new COwnedRoot;
    o.m_pArray08 = new CElement08[2];
    o.m_pArray0C = 0;
    o.m_pArray10 = new CElement10[3];
    o.m_pArray14 = new CElement14[1];
    o.m_pDevice = new COwnedDevice;
    o.Cleanup();
    if (g_clearCalls != 4 || g_elementDtors != 6) fail = 1;
    if (o.m_pArray08 || o.m_pArray10 || o.m_pArray14 || o.m_pDevice || o.m_pRoot) fail = 2;
    if (g_closeCalls != 1 || g_deviceDtors != 1 || g_rootDtors != 1) fail = 3;
    if (g_released != 2 || g_releasedList[0] != &e1 || g_releasedList[1] != &e3) fail = 4;

    // a second cleanup without the root does nothing
    o.m_pDevice = new COwnedDevice;
    o.Cleanup();
    if (g_clearCalls != 4 || g_closeCalls != 1 || o.m_pDevice == 0) fail = 5;
    delete o.m_pDevice;

    // an empty global slot is not walked
    g_slot_0143bc00.m_count = 0; g_released = 0;
    o.m_pRoot = new COwnedRoot; o.m_pDevice = 0;
    o.Cleanup();
    if (g_released != 0 || g_rootDtors != 2 || g_clearCalls != 8) fail = 6;

    if (fail) { std::printf("00c3d160_TEST FAIL\n"); return 1; }
    std::printf("00c3d160_TEST PASS\n");
    return 0;
}