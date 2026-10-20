// Behaviour test for CMainGameComponent::CheckSync @ 0x004165E8: the world checksum is peeked once, then every packet
// of the package is visited (the packet count re-read each iteration); only sync packets (type 1) are copied into the
// one zero-initialised 0x2c-byte read buffer, from which three 4-byte words are read in order. Checks: empty package,
// a mix of sync and non-sync packets (copy count, buffer identity/initial state, read sizes and values), no sync packets.
#include <cstdio>
#include <cstring>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail = 1; std::printf("check failed line %d\n", __LINE__); } } while (0)

#pragma optimize("s", on)
#pragma optimize("y", off)
struct CReadBuffer {                               // 0x2c bytes
    unsigned char m_data[0x20];
    unsigned long m_length;                        // +0x20
    unsigned long m_readPos;                       // +0x24
    unsigned long m_spare;                         // +0x28
    CReadBuffer() : m_length(0), m_readPos(0), m_spare(0) { }
    void Read(unsigned long size, void* out);      // 0x009F18F0
};
struct CEventPacket {                              // 40 bytes
    long          m_type;                          // +0x00
    unsigned char _body[0x24];
    long CopyToBuffer(CReadBuffer* buffer) const;  // 0x009F17A0
};
struct CEventPackage {
    unsigned long GetNoPackets() const;            // 0x009F16E0
    const CEventPacket* GetPacket(unsigned long index) const;   // 0x009F16D0
};
struct CMainGameComponent {
    unsigned long PeekWorldChecksum();             // 0x00416392
    void CheckSync(const CEventPackage& package);
};

void CMainGameComponent::CheckSync(const CEventPackage& package)
{
    CReadBuffer buffer;
    PeekWorldChecksum();
    for (unsigned long i = 0; i < package.GetNoPackets(); ++i)
    {
        const CEventPacket* packet = package.GetPacket(i);
        if (packet->m_type == 1)
        {
            unsigned long worldChecksum, frame, random;
            packet->CopyToBuffer(&buffer);
            buffer.Read(4, &worldChecksum);
            buffer.Read(4, &frame);
            buffer.Read(4, &random);
        }
    }
}
#pragma optimize("", on)

// ---- instrumented stubs ----
static int g_peeks = 0, g_counts = 0, g_gets = 0, g_copies = 0, g_reads = 0;
static bool g_initOk = true, g_sameBuffer = true, g_sizesOk = true;
static const CReadBuffer* g_firstBuffer = 0;
static unsigned long g_readValues[16];
static const CEventPacket* g_packets = 0;
static unsigned long g_noPackets = 0;
static const CEventPackage* g_expectPackage = 0;

unsigned long CMainGameComponent::PeekWorldChecksum() { ++g_peeks; return 0x12345678; }
unsigned long CEventPackage::GetNoPackets() const { ++g_counts; if (this != g_expectPackage) g_fail = 1; return g_noPackets; }
const CEventPacket* CEventPackage::GetPacket(unsigned long index) const
{
    ++g_gets; if (this != g_expectPackage || index >= g_noPackets) { g_fail = 1; return 0; }
    return &g_packets[index];
}
long CEventPacket::CopyToBuffer(CReadBuffer* buffer) const
{
    if (g_copies == 0) {
        g_firstBuffer = buffer;
        g_initOk = buffer->m_length == 0 && buffer->m_readPos == 0 && buffer->m_spare == 0;
    } else if (buffer != g_firstBuffer) g_sameBuffer = false;
    ++g_copies;
    std::memcpy(buffer->m_data, _body, 12);       // payload: three sync words
    buffer->m_length = 12; buffer->m_readPos = 0;
    return 12;
}
void CReadBuffer::Read(unsigned long size, void* out)
{
    if (size != 4 || m_readPos + size > m_length) { g_sizesOk = false; return; }
    std::memcpy(out, m_data + m_readPos, size); m_readPos += size;
    unsigned long v; std::memcpy(&v, out, 4);
    if (g_reads < 16) g_readValues[g_reads] = v;
    ++g_reads;
}

static void reset() { g_peeks = g_counts = g_gets = g_copies = g_reads = 0; g_initOk = g_sameBuffer = g_sizesOk = true; g_firstBuffer = 0; }
static void makePacket(CEventPacket& p, long type, unsigned long a, unsigned long b, unsigned long c)
{
    std::memset(&p, 0, sizeof(p)); p.m_type = type;
    std::memcpy(p._body, &a, 4); std::memcpy(p._body + 4, &b, 4); std::memcpy(p._body + 8, &c, 4);
}

int main()
{
    CMainGameComponent mgc; CEventPackage package; g_expectPackage = &package;
    CEventPacket packets[4];

    // Empty package: checksum peeked, count read once, nothing else.
    reset(); g_packets = packets; g_noPackets = 0;
    mgc.CheckSync(package);
    CHECK(g_peeks == 1 && g_counts == 1 && g_gets == 0 && g_copies == 0 && g_reads == 0);

    // Mixed: sync, other, sync, other.
    makePacket(packets[0], 1, 0x11, 0x22, 0x33);
    makePacket(packets[1], 2, 0xdead, 0xdead, 0xdead);
    makePacket(packets[2], 1, 0x44, 0x55, 0x66);
    makePacket(packets[3], 0, 0xbeef, 0xbeef, 0xbeef);
    reset(); g_noPackets = 4;
    mgc.CheckSync(package);
    CHECK(g_peeks == 1 && g_counts == 5 && g_gets == 4);
    CHECK(g_copies == 2 && g_initOk && g_sameBuffer && g_sizesOk && g_reads == 6);
    CHECK(g_readValues[0] == 0x11 && g_readValues[1] == 0x22 && g_readValues[2] == 0x33);
    CHECK(g_readValues[3] == 0x44 && g_readValues[4] == 0x55 && g_readValues[5] == 0x66);

    // No sync packets: visited but never copied.
    packets[0].m_type = 3; packets[2].m_type = -1;
    reset(); g_noPackets = 3;
    mgc.CheckSync(package);
    CHECK(g_peeks == 1 && g_counts == 4 && g_gets == 3 && g_copies == 0 && g_reads == 0);

    if (g_fail) { std::printf("004165e8_TEST FAIL\n"); return 1; }
    std::printf("004165e8_TEST PASS\n");
    return 0;
}