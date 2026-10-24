// Behaviour test for 0x008F86A0 LSocket::ResolveHost(const string&): inet_addr first, gethostbyname as the fallback.
#include <cstdio>
#include <cstdlib>
#include <cstring>

struct hostent { char* h_name; char** h_aliases; short h_addrtype; short h_length; char** h_addr_list; };

static unsigned int g_addrBytes = 0x0100007F;            // 127.0.0.1 in network order
static char* g_addrList[2] = { (char*)&g_addrBytes, 0 };
static hostent g_host = { (char*)"localhost", 0, 2, 4, g_addrList };

static unsigned int FakeInetAddr(const char* cp) { return std::strcmp(cp, "10.0.0.1") == 0 ? 0x0100000AU : 0xFFFFFFFFU; }
static hostent* FakeGetHostByName(const char* name) { return std::strcmp(name, "localhost") == 0 ? &g_host : 0; }

struct SString { char* _M_start; char* _M_finish; char* _M_end_of_storage; const char* c_str() const { return _M_start; } };

static unsigned int ResolveHost(const SString& host)
{
    char* scratch = (char*)std::malloc(1);
    scratch[0] = 0;
    unsigned int ip = FakeInetAddr(host.c_str());
    if (ip == (unsigned int)-1) {
        hostent* entry = FakeGetHostByName(host.c_str());
        if (entry)
            ip = *(unsigned int*)entry->h_addr_list[0];
    }
    std::free(scratch);
    return ip;
}

int main()
{
    int fail = 0;
    char a[] = "10.0.0.1", b[] = "localhost", c[] = "nowhere";
    SString sa = { a, a + 8, a + 9 }, sb = { b, b + 9, b + 10 }, sc = { c, c + 7, c + 8 };
    if (ResolveHost(sa) != 0x0100000AU) fail = 1;
    if (ResolveHost(sb) != 0x0100007FU) fail = 1;
    if (ResolveHost(sc) != 0xFFFFFFFFU) fail = 1;
    if (fail) { std::printf("008f86a0_TEST FAIL\n"); return 1; }
    std::printf("008f86a0_TEST PASS\n");
    return 0;
}