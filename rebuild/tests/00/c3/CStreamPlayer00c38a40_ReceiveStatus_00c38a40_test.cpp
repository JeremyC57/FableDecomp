// Behaviour test for 00c38a40: CStreamPlayer::ReceiveStatus -- reads one message from each mailbox, reports the stream message's flag, and on a full pair caches format/rate/position and copies the range
#include <cstdio>

struct CStreamRange { long m_start, m_end; };
struct CStreamMessage { bool m_received; CStreamRange m_range; long m_position; };
struct CFormatMessage { bool m_valid; long m_format; long m_rate; };

struct CStreamMailbox {
    CStreamMessage m_next; int m_reads;
    CStreamMessage Read() { ++m_reads; return m_next; }
};
struct CFormatMailbox {
    CFormatMessage m_next; int m_reads;
    CFormatMessage Read() { ++m_reads; return m_next; }
};

struct CStreamPlayer {
    CFormatMailbox m_formatBox;
    CStreamMailbox m_streamBox;
    long m_position, m_format, m_rate;
    bool ReceiveStatus(bool* pReceived, CStreamRange* pRange)
    {
        CStreamMessage stream = m_streamBox.Read();
        CFormatMessage format = m_formatBox.Read();
        *pReceived = stream.m_received;
        if (!stream.m_received || !format.m_valid)
            return false;
        m_format = format.m_format;
        m_rate = format.m_rate;
        m_position = stream.m_position;
        if (pRange)
            *pRange = stream.m_range;
        return true;
    }
};

static int fail() { std::printf("00c38a40_TEST FAIL\n"); return 1; }

int main()
{
    CStreamPlayer p;
    p.m_formatBox.m_reads = 0; p.m_streamBox.m_reads = 0;
    p.m_position = p.m_format = p.m_rate = -1;
    CStreamMessage s = { true, { 10, 20 }, 30 };
    CFormatMessage f = { false, 1, 44100 };
    p.m_streamBox.m_next = s; p.m_formatBox.m_next = f;
    bool received = false;
    CStreamRange range = { 0, 0 };
    if (p.ReceiveStatus(&received, &range) || !received || p.m_format != -1 || p.m_formatBox.m_reads != 1) return fail();
    p.m_formatBox.m_next.m_valid = true;
    if (!p.ReceiveStatus(&received, &range) || p.m_format != 1 || p.m_rate != 44100 || p.m_position != 30
        || range.m_start != 10 || range.m_end != 20) return fail();
    if (!p.ReceiveStatus(&received, 0)) return fail();
    p.m_streamBox.m_next.m_received = false;
    if (p.ReceiveStatus(&received, &range) || received) return fail();
    std::printf("00c38a40_TEST PASS\n");
    return 0;
}