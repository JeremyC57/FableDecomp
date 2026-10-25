#include <cstdio>
#include <cstdlib>
#include <cstring>
struct SRetailAllocProxy { char* m_data; SRetailAllocProxy(char* data) : m_data(data) { } };
struct CRetailStringBase {
    char* m_start; char* m_finish; SRetailAllocProxy m_endOfStorage;
    CRetailStringBase(size_t n) : m_start(0), m_finish(0), m_endOfStorage(0)
    {
        m_start = static_cast<char*>(std::malloc(n));
        m_finish = m_start;
        m_endOfStorage.m_data = m_start + n;
    }
};
struct CRetailString : CRetailStringBase {
    CRetailString() : CRetailStringBase(8) { *m_finish = 0; }
    CRetailString& Assign(const char* first, const char* last)
    {
        size_t n = last - first;
        std::memcpy(m_start, first, n);
        m_finish = m_start + n;
        *m_finish = 0;
        return *this;
    }
    CRetailString& operator=(const char* text) { return Assign(text, text + std::strlen(text)); }
};
namespace LHNetworkLib {
struct LAddress {
    CRetailString ServerName; unsigned long ServerIP; unsigned short ServerPort; long TransportProtocol; long TransportFlags;
    LAddress();
};
LAddress::LAddress()
    : ServerName()
{
    ServerName = "";
    ServerPort = 0;
    ServerIP = 0;
    TransportProtocol = 0;
    TransportFlags = 0;
}
}
int main()
{
    LHNetworkLib::LAddress r;
    if (!r.ServerName.m_start || r.ServerName.m_finish != r.ServerName.m_start || *r.ServerName.m_finish != 0 || r.ServerName.m_endOfStorage.m_data != r.ServerName.m_start + 8
        || r.ServerIP || r.ServerPort || r.TransportProtocol || r.TransportFlags || sizeof(r) != 0x1c) { std::printf("00841e90_TEST FAIL\n"); return 1; }
    std::printf("00841e90_TEST PASS\n");
    return 0;
}
