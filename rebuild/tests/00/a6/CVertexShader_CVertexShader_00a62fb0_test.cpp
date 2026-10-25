// behaviour test for 0x00A62FB0 CVertexShader::CVertexShader(), linked against the built constructor: the data pointer is
// cleared, the vtable installed (virtual destructor dispatch reaches CVertexShader's), this returned
#include <cstdio>
#include <cstring>
#include <new>
static int g_dtors = 0;
int main();
class CVertexShaderData;
class CVertexShader {
    friend int ::main();
public:
    CVertexShader();
    virtual ~CVertexShader();
public:
    CVertexShaderData* Data;
};
CVertexShader::~CVertexShader() { ++g_dtors; }
int main()
{
    if (sizeof(CVertexShader) != 8) { std::printf("00a62fb0_TEST FAIL size\n"); return 1; }
    unsigned char buf[sizeof(CVertexShader)];
    std::memset(buf, 0xCD, sizeof(buf));
    CVertexShader* p = new (buf) CVertexShader();
    bool ok = (void*)p == (void*)buf && p->Data == 0 && *(unsigned long*)buf != 0xCDCDCDCDu;
    p->~CVertexShader();
    ok = ok && g_dtors == 1;
    if (!ok) { std::printf("00a62fb0_TEST FAIL\n"); return 1; }
    std::printf("00a62fb0_TEST PASS\n");
    return 0;
}
