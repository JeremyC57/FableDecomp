#include <cstdio>

class CDefClassBase { public: int base[10]; int f28; 
public:
    CDefClassBase& operator=(const CDefClassBase& other);
};

class CExpressionSubDef { public:
    int base[10];
    int f28;
    void BaseClone(const CDefClassBase* src);
};

static int g_base_called = 0;
static void* g_base_self = 0;
static const void* g_base_src = 0;

CDefClassBase& CDefClassBase::operator=(const CDefClassBase& other_)
{
    const CDefClassBase* src = (const CDefClassBase*)&other_; (void)src;
    g_base_called = 1;
    g_base_self = ((CExpressionSubDef*)this);
    g_base_src = src;
    return *this;
}

void __fastcall CExpressionSubDef_Copy(CExpressionSubDef* self, int /*edx*/, const CDefClassBase* src);

int main()
{
    CExpressionSubDef dst; dst.f28 = 111;
    CExpressionSubDef src; src.f28 = 999;

    CExpressionSubDef_Copy(&dst, 0, (const CDefClassBase*)&src);

    if (g_base_called == 1 && g_base_self == &dst && g_base_src == (const void*)&src && dst.f28 == 999)
        printf("004e0ca7_TEST PASS\n");
    else
        printf("FAIL called=%d self=%d src=%d f28=%d\n", g_base_called, g_base_self==&dst, g_base_src==(void*)&src, dst.f28);
    return 0;
}