// Behaviour test for PKCBufferedReader::CopyOutOfBuffers (0x00C4B640): completed reads are returned first; the resident blocks
// from the current position are copied (slot = (first slot + block) % slot count, a slot still owned by a read is waited for
// and the listener told when the wait fails), the position advances, the read-ahead is refilled, the byte count is returned;
// the copy stops at the last resident block.
#include <cstdio>
#include <cstring>
static int g_returned = 0, g_refills = 0, g_stalls = 0, g_waits = 0, g_traces = 0;
static bool g_waitResult = true;
struct CListener { void OnSlotStalled(unsigned long) { ++g_stalls; } };
struct CReader {
    unsigned char* m_slotOwners; unsigned long m_blockSize, m_slotCount; unsigned char* m_pBufferMemory;
    unsigned long m_firstSlot, m_blockCount, m_position; CListener* m_pListener;
    void Refill() { ++g_refills; }
    bool WaitForSlot(unsigned long slot) { ++g_waits; m_slotOwners[slot] = 0xFF; return g_waitResult; }
    void ReturnCompletedBuffers() { ++g_returned; }
    unsigned long CopyOutOfBuffers(void* buffer, unsigned long size)
    {
        unsigned long block = m_position / m_blockSize;
        unsigned long offset = m_position - block * m_blockSize;
        unsigned long copied = 0;
        ReturnCompletedBuffers();
        while (size != 0 && block < m_blockCount) {
            unsigned long count = m_blockSize;
            if (offset >= count) ++g_traces;
            if (offset != 0) count -= offset;
            if (count > size) count = size;
            if (count == 0) ++g_traces;
            unsigned long slot = (m_firstSlot + block) % m_slotCount;
            if (m_slotOwners[slot] != 0xFF && !WaitForSlot(slot) && m_pListener) m_pListener->OnSlotStalled(slot);
            std::memcpy(buffer, m_pBufferMemory + slot * m_blockSize + offset, count);
            size -= count; offset = 0; ++block; buffer = (char*)buffer + count; m_position += count; copied += count;
        }
        Refill();
        return copied;
    }
};
int main()
{
    unsigned char owners[4] = { 0xFF, 0xFF, 3, 0xFF };
    unsigned char memory[4 * 8];
    for (int i = 0; i < 32; ++i) memory[i] = (unsigned char)i;
    CListener listener;
    CReader r = { owners, 8, 4, memory, 1, 3, 5, &listener };
    unsigned char out[32]; std::memset(out, 0xEE, sizeof out);
    // position 5: block 0 (slot 1) bytes 5..7, then block 1 (slot 2, owned -> waited) bytes 0..7, then block 2 (slot 3)
    g_waitResult = false;
    unsigned long n = r.CopyOutOfBuffers(out, 13);
    bool ok = n == 13 && r.m_position == 18 && out[0] == 13 && out[2] == 15 && out[3] == 16 && out[10] == 23 && out[11] == 24
              && out[12] == 25 && out[13] == 0xEE && g_waits == 1 && g_stalls == 1 && g_returned == 1 && g_refills == 1 && g_traces == 0;
    // 30 more bytes requested: only block 2's remaining 6 bytes are resident (block count 3)
    n = r.CopyOutOfBuffers(out, 30);
    ok = ok && n == 6 && r.m_position == 24 && out[0] == 26 && out[5] == 31 && g_refills == 2;
    n = r.CopyOutOfBuffers(out, 4);
    ok = ok && n == 0 && g_refills == 3 && g_traces == 0;
    if (!ok) { std::printf("00c4b640_TEST FAIL\n"); return 1; }
    std::printf("00c4b640_TEST PASS\n");
    return 0;
}