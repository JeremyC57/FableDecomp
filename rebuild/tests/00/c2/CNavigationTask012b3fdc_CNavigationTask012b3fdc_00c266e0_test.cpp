// Behaviour test for 0x00C266E0 (CNavigationTask_012b3fdc constructor): the four plain words and the two
// shorts are zeroed, the embedded request is constructed, the other words are left alone.
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
    SNavigationRequest()
        : m_1c(0), m_20(0), m_scaleX(1.0f), m_scaleY(1.0f), m_2c(0), m_40(0), m_44(0), m_48(0), m_4c(0)
    {
    }
};

struct CNavigationTask_012b3fdc {
    virtual void v00() { }
    virtual ~CNavigationTask_012b3fdc() { }
    long               m_04;
    long               m_08;
    long               m_0c;
    long               m_10;
    long               m_14;
    long               m_18;
    long               m_1c;
    long               m_20;
    SNavigationRequest m_request;
    short              m_74;
    short              m_76;
    CNavigationTask_012b3fdc();
};

CNavigationTask_012b3fdc::CNavigationTask_012b3fdc()
    : m_04(0), m_08(0), m_14(0), m_18(0), m_74(0), m_76(0)
{
}

int main()
{
    CNavigationTask_012b3fdc* t = new CNavigationTask_012b3fdc;
    if (t->m_04 || t->m_08 || t->m_14 || t->m_18 || t->m_74 || t->m_76 || g_ownerCtors != 1 || g_layerCtors != 1 ||
        t->m_request.m_scaleX != 1.0f || t->m_request.m_scaleY != 1.0f || t->m_request.m_4c) {
        std::printf("00c266e0_TEST FAIL\n");
        return 1;
    }
    delete t;
    std::printf("00c266e0_TEST PASS\n");
    return 0;
}