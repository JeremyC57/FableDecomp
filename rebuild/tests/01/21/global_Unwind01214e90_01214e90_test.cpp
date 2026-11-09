// Behaviour test for CAsyncFileReader::ReadRemote (0x00C3BF30): an empty request fails; a block-aligned request goes to
// the block read in blocks; an unaligned buffer becomes one padded script-info load; an aligned buffer with a padded head
// and short tail is split into script-info (padded) and slave (whole-block) loads; the master's Finalize is the result.
#include <cstdio>
#include <cstring>
#include <cstddef>
#include <string>

struct CRACBuffer {
    unsigned char* m_pBase; unsigned long m_size, m_offset, m_alignment, m_headPadding, m_tailPadding;
    CRACBuffer(unsigned char* base, unsigned long size, unsigned long offset, unsigned long alignment)
        : m_pBase(base), m_size(size), m_offset(offset), m_alignment(alignment)
    {
        m_headPadding = m_offset - m_offset / m_alignment * m_alignment;
        m_tailPadding = (m_size + m_headPadding + m_alignment - 1) / m_alignment * m_alignment - m_size - m_headPadding;
    }
    bool IsAligned() const { return (((size_t)m_pBase - m_offset % m_alignment) & 3) == 0; }
    bool IsRangeAligned() const { return m_offset % m_alignment == 0 && m_size % m_alignment == 0; }
    bool IsAligned(unsigned long n) { return n % m_alignment == 0; }
    unsigned long CheckAlignmentOffset(unsigned long bytes) { return bytes / m_alignment; }
    unsigned long GetSize() { return m_size; }
    unsigned char* GetPointer(unsigned long o) const { return m_pBase + o; }
    unsigned long GetHeadPadding() { return m_headPadding; }
    unsigned long GetBlockCount() { return CheckAlignmentOffset(m_headPadding + m_tailPadding + m_size); }
    unsigned long GetFirstBlock() { return CheckAlignmentOffset(m_offset - m_headPadding); }
    unsigned long GetAlignedSize(unsigned long count) const { return m_alignment * count; }
    unsigned long GetAlignment() { return m_alignment; }
};
static std::string g_log; static unsigned char* g_dest;
static void Log(const char* tag, unsigned long a, unsigned long b, unsigned long c, unsigned char* d, unsigned long e)
{ char t[96]; std::sprintf(t, "%s(%lu,%lu,%lu,+%ld,%lu)", tag, a, b, c, (long)(d - g_dest), e); g_log += t; }
struct Reader;
struct Master {
    void LoadScriptInfo(Reader*, unsigned long block, unsigned long blocks, unsigned long padding, unsigned char* dest, unsigned long size) { Log("S", block, blocks, padding, dest, size); }
    void LoadSlave(Reader*, unsigned long block, unsigned long blocks, unsigned char* dest) { Log("L", block, blocks, 0, dest, 0); }
    bool Finalize() { g_log += "F"; return true; }
};
static Master g_master;
static Master* CreateMaster(void*, unsigned long) { return &g_master; }
struct Reader {
    bool ReadBlocks(unsigned long block, unsigned long blocks, unsigned long size, void* dest, unsigned long, void*)
    { Log("R", block, blocks, size, (unsigned char*)dest, 0); return true; }
    unsigned long GetBlockSize() { return 16; }
    bool ReadRemote(unsigned long offset, unsigned long size, void* dest, unsigned long flags, void* callback)
    {
        if (size == 0) return false;
        CRACBuffer buffer((unsigned char*)dest, size, offset, GetBlockSize());
        if (buffer.IsAligned() && buffer.IsRangeAligned())
            return ReadBlocks(buffer.CheckAlignmentOffset(offset), buffer.CheckAlignmentOffset(size), size, dest, flags, callback);
        Master* master = CreateMaster(callback, flags);
        if (!buffer.IsAligned()) {
            master->LoadScriptInfo(this, buffer.GetFirstBlock(), buffer.GetBlockCount(), buffer.GetHeadPadding(), buffer.GetPointer(0), buffer.GetSize());
        } else {
            unsigned long blocksLeft = buffer.GetBlockCount();
            unsigned long bytesLeft = buffer.GetSize();
            unsigned long padding = buffer.GetHeadPadding();
            unsigned long block = buffer.GetFirstBlock();
            unsigned char* out = buffer.GetPointer(0);
            while (blocksLeft != 0) {
                unsigned long blocks = blocksLeft;
                bool padded = false;
                unsigned long bytes = bytesLeft + padding;
                if (padding != 0) { blocks = 1; padded = true; }
                if (!buffer.IsAligned(bytes)) {
                    if (!padded && blocks > 1) --blocks;
                    else { blocks = 1; padded = true; }
                }
                if (bytes > buffer.GetAlignedSize(blocks)) bytes = buffer.GetAlignedSize(blocks);
                if (padded) master->LoadScriptInfo(this, block, blocks, padding, out, bytes - padding);
                else master->LoadSlave(this, block, blocks, out);
                blocksLeft -= blocks;
                bytesLeft -= bytes - padding;
                out += bytes - padding;
                block += blocks;
                padding = 0;
            }
        }
        return master->Finalize();
    }
};

int main()
{
    static long long storage[32];
    unsigned char* d = (unsigned char*)storage; g_dest = d;
    Reader r; bool ok = !r.ReadRemote(0, 0, d, 0, 0) && g_log.empty();
    ok = ok && r.ReadRemote(32, 64, d, 0, 0) && g_log == "R(2,4,64,+0,0)";
    g_log = "";
    ok = ok && r.ReadRemote(32, 20, d + 1, 0, 0) && g_log == "S(2,2,0,+1,20)F";
    g_log = "";
    ok = ok && r.ReadRemote(20, 40, d + 4, 0, 0) && g_log == "S(1,1,4,+4,12)L(2,1,0,+16,0)S(3,1,0,+32,12)F";
    if (!ok) { std::printf("00c3bf30_TEST FAIL %s\n", g_log.c_str()); return 1; }
    std::printf("01214e90_TEST PASS\n");
    return 0;
}