// Behaviour test for 00c48f30: SDecoderMessage::SDecoderMessage -- zero +0 and the +8 argument record, build the +0x14 heap block, leave +4 alone
#include <cstdio>
#include <cstring>
#include <new>

static int g_blockCtors = 0;
struct CHeapBlock {
    CHeapBlock() : m_pData(0), m_size(0) { ++g_blockCtors; }
    void*        m_pData;
    unsigned int m_size;
};
struct SMessageArgs {
    int m_a, m_b, m_c;
    SMessageArgs() : m_a(0), m_b(0), m_c(0) { }
};
struct SDecoderMessage {
    SDecoderMessage() : m_type(0) { }
    int          m_type;
    int          m_param;
    SMessageArgs  m_args;
    CHeapBlock    m_block;
    unsigned char m_payload[0x24];
};

int main()
{
    static unsigned char raw[sizeof(SDecoderMessage)];
    std::memset(raw, 0x5A, sizeof(raw));
    SDecoderMessage* m = new (raw) SDecoderMessage();
    if (m->m_type != 0 || m->m_args.m_a != 0 || m->m_args.m_b != 0 || m->m_args.m_c != 0 || m->m_block.m_pData != 0
        || g_blockCtors != 1 || m->m_param != 0x5A5A5A5A || m->m_payload[0] != 0x5A) {
        std::printf("00c48f30_TEST FAIL\n"); return 1;
    }
    std::printf("00c48f30_TEST PASS\n");
    return 0;
}