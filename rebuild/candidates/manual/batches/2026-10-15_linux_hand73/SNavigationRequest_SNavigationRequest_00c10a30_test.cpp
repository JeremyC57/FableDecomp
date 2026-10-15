// Behaviour test for 0x00C10A30 (SNavigationRequest constructor): both member constructors run, the
// scalars are zero except the two 1.0f scales.
#include <cstdio>
#include <new>

static int g_ownerCtors = 0, g_layerCtors = 0;

struct CMessageListOwner {
    unsigned char _body[0x1c];
    CMessageListOwner() { ++g_ownerCtors; }
};

struct CNavigationLayer {
    void* m_pFirst;
    void* m_pSecond;
    void* m_pThird;
    unsigned char m_flag;
    CNavigationLayer() : m_pFirst(0), m_pSecond(0), m_pThird(0), m_flag(0) { ++g_layerCtors; }
};

struct SNavigationRequest {
    CMessageListOwner m_messages;
    long              m_1c;
    long              m_20;
    float             m_scaleX;
    float             m_scaleY;
    long              m_2c;
    CNavigationLayer  m_layer;
    long              m_40;
    short             m_44;
    long              m_48;
    long              m_4c;
    SNavigationRequest();
};

SNavigationRequest::SNavigationRequest()
    : m_1c(0), m_20(0), m_scaleX(1.0f), m_scaleY(1.0f), m_2c(0), m_40(0), m_44(0), m_48(0), m_4c(0)
{
}

int main()
{
    unsigned char raw[sizeof(SNavigationRequest)];
    for (unsigned i = 0; i < sizeof(raw); ++i) raw[i] = 0xCD;
    SNavigationRequest* r = new (raw) SNavigationRequest;
    if (g_ownerCtors != 1 || g_layerCtors != 1 || r->m_1c || r->m_20 || r->m_scaleX != 1.0f || r->m_scaleY != 1.0f ||
        r->m_2c || r->m_40 || r->m_44 || r->m_48 || r->m_4c || r->m_layer.m_pFirst) {
        std::printf("00c10a30_TEST FAIL\n");
        return 1;
    }
    std::printf("00c10a30_TEST PASS\n");
    return 0;
}
