#include <cstdio>
#include <cstdlib>

static int g_freed = 0;
static void* g_lastFreed = (void*)0;

void operator delete[](void* p)
{
    g_freed++;
    g_lastFreed = p;
}

struct CPackedUIntArray
{
    void* buffer;
    int   Size;
    char  Bits;
    char  pad9;
    char  padA;
    char  padB;
    int   Bias;
    ~CPackedUIntArray();
};

CPackedUIntArray::~CPackedUIntArray()
{
    operator delete[](this->buffer);
    this->buffer = 0;
    this->Size = 0;
    this->Bits = 0;
    this->Bias = 0;
}

int main()
{
    CPackedUIntArray a;
    void* marker = (void*)0xDEADBEEF;
    a.buffer = marker;
    a.Size = 123;
    a.Bits = 7;
    a.Bias = 456;

    a.~CPackedUIntArray();

    int ok = (g_freed == 1) && (g_lastFreed == marker) &&
             (a.buffer == 0) && (a.Size == 0) && (a.Bits == 0) && (a.Bias == 0);

    if (ok) printf("DTOR_OK\n");
    else    printf("DTOR_FAIL\n");
    return ok ? 0 : 1;
}