// Behaviour test for PKCBufferedReader::Refill (0x00C4A3B0): the window moves so the current block sits m_lookBehind blocks in
// (backwards: idle slots in front of the first one are claimed; forwards: blocks behind are dropped), every idle slot after the
// window is claimed up to the file's last block, and one remote read is issued per run of consecutive claimed slots (a run
// is cut at slot 0 and at an unclaimed slot); each slot of a run records the run's first slot, whose address is the cookie.
#include <cstdio>
#include <cstring>
#include <cstdlib>
struct ReadCall { unsigned long offset, size, destOffset, cookieSlot; };
static ReadCall g_reads[16];
static int g_readCount = 0, g_traces = 0;
struct CReader {
    unsigned char* m_slotOwners; unsigned long m_blockSize, m_lookBehind, m_slotCount; unsigned char* m_pBufferMemory;
    unsigned long m_size, m_firstBlock, m_firstSlot, m_blockCount, m_position, m_readsIssued;
    unsigned long GetAvailableReadSize(unsigned long offset, unsigned long bytes)
    {
        return offset + bytes > m_size ? m_size - offset : bytes;
    }
    bool ReadRemote(unsigned long offset, unsigned long size, unsigned char* dest, unsigned char* cookie)
    {
        ReadCall c; c.offset = offset; c.size = size; c.destOffset = (unsigned long)(dest - m_pBufferMemory);
        c.cookieSlot = (unsigned long)(cookie - m_slotOwners); g_reads[g_readCount++] = c;
        return true;
    }
    void Flush(unsigned char* run, unsigned long offset, unsigned long& bytes)
    {
        unsigned char* dest = m_pBufferMemory + (run - m_slotOwners) * m_blockSize;
        bytes = GetAvailableReadSize(offset, bytes);
        if (!ReadRemote(offset, bytes, dest, run)) ++g_traces;
        ++m_readsIssued;
    }
    void Refill()
    {
        long current = (long)(m_position / m_blockSize);
        long move = current - (long)m_lookBehind;
        if (labs(move) > (long)m_slotCount) ++g_traces;
        if (move < 0) {
            if (move < -(long)m_firstBlock) move = -(long)m_firstBlock;
            for (; move != 0; ++move) {
                unsigned long slot = (m_slotCount + m_firstSlot - 1) % m_slotCount;
                if (m_slotOwners[slot] != 0xFF) break;
                m_slotOwners[slot] = 0xFE; m_firstSlot = slot; ++m_blockCount; m_position += m_blockSize;
                if (m_firstBlock == 0) ++g_traces;
                --m_firstBlock;
            }
        } else if (move > 0) {
            unsigned long count = m_blockCount - move;
            if (m_blockCount > count) m_blockCount = count;
            m_firstSlot = (m_firstSlot + move) % m_slotCount;
            if (m_position < m_blockSize * move) ++g_traces;
            m_position -= m_blockSize * move; m_firstBlock += move;
        }
        unsigned long claim = m_slotCount - m_blockCount;
        unsigned long fileBlocks = (m_size + m_blockSize - 1) / m_blockSize;
        unsigned long remaining = fileBlocks - (m_firstBlock + m_blockCount);
        if (claim > remaining) claim = remaining;
        for (; claim != 0; --claim) {
            unsigned long slot = (m_firstSlot + m_blockCount) % m_slotCount;
            if (m_slotOwners[slot] != 0xFF) break;
            if (m_firstBlock + m_blockCount >= fileBlocks) ++g_traces;
            m_slotOwners[slot] = 0xFE; ++m_blockCount;
        }
        unsigned char* run = 0; unsigned long offset = 0, bytes = 0;
        for (unsigned long i = 0; i < m_blockCount; ++i) {
            unsigned long slot = (m_firstSlot + i) % m_slotCount;
            if (m_slotOwners[slot] == 0xFE) {
                if (run) {
                    if (slot != 0) { bytes += m_blockSize; m_slotOwners[slot] = *run; continue; }
                    Flush(run, offset, bytes);
                }
                run = m_slotOwners + slot; *run = (unsigned char)slot;
                offset = (m_firstBlock + i) * m_blockSize; bytes = m_blockSize;
            } else if (run) {
                Flush(run, offset, bytes); run = 0;
            }
        }
        if (run) Flush(run, offset, bytes);
    }
};
static bool Read(int i, unsigned long offset, unsigned long size, unsigned long dest, unsigned long cookie)
{
    return g_reads[i].offset == offset && g_reads[i].size == size && g_reads[i].destOffset == dest && g_reads[i].cookieSlot == cookie;
}
int main()
{
    unsigned char owners[4]; unsigned char memory[40];
    std::memset(owners, 0xFF, sizeof owners);
    // 100-byte file in 10-byte blocks, 4 slots, no look-behind: the empty window claims all four slots in one read
    CReader r = { owners, 10, 0, 4, memory, 100, 0, 0, 0, 0, 0 };
    r.Refill();
    bool ok = g_readCount == 1 && Read(0, 0, 40, 0, 0) && r.m_blockCount == 4 && owners[3] == 0 && r.m_readsIssued == 1;
    // the reads complete; move forward two blocks: slots 0 and 1 are re-used for blocks 4 and 5 in one read
    std::memset(owners, 0xFF, sizeof owners); r.m_position = 25; r.Refill();
    ok = ok && r.m_firstBlock == 2 && r.m_firstSlot == 2 && r.m_position == 5 && r.m_blockCount == 4 && g_readCount == 2
            && Read(1, 40, 20, 0, 0) && owners[1] == 0 && owners[2] == 0xFF;
    // a run that would wrap is cut at slot 0
    std::memset(owners, 0xFF, sizeof owners); g_readCount = 0;
    CReader w = { owners, 10, 0, 4, memory, 100, 0, 3, 0, 0, 0 };
    w.Refill();
    ok = ok && g_readCount == 2 && Read(0, 0, 10, 30, 3) && Read(1, 10, 30, 0, 0) && owners[3] == 3 && owners[2] == 0;
    // backwards with one block of look-behind: slot 0 is pulled in front (block 2), slot 3 claimed at the end (block 5);
    // the last block is cut short by the end of the file
    std::memset(owners, 0xFF, sizeof owners); g_readCount = 0;
    CReader b = { owners, 10, 1, 4, memory, 55, 3, 1, 2, 0, 0 };
    b.Refill();
    ok = ok && b.m_firstBlock == 2 && b.m_firstSlot == 0 && b.m_position == 10 && b.m_blockCount == 4 && g_readCount == 2
            && Read(0, 20, 10, 0, 0) && Read(1, 50, 5, 30, 3) && g_traces == 0;
    if (!ok) { std::printf("00c4a3b0_TEST FAIL\n"); return 1; }
    std::printf("01215d0a_TEST PASS\n");
    return 0;
}