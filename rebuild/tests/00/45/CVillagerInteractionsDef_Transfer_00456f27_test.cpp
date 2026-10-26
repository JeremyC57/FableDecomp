#include <cstdio>

class CDefString;
class CPersistContext { public:
    int calls;
    template <class T> void Transfer(T& value);
};
template <> void CPersistContext::Transfer<CDefString>(CDefString& value);
template <> void CPersistContext::Transfer<float>(float& value);

class CVillagerInteractionsDef { public:
    char pad[0x3c];
    int fieldA;
    int fieldB;
    int fieldC;
};

static int g_seq[4];
static int g_n;

template <> void CPersistContext::Transfer<CDefString>(CDefString& value_)
{
    int& r = *(int*)&value_; (void)&r; calls++; g_seq[g_n++] = 1; }
template <> void CPersistContext::Transfer<float>(float& value_)
{
    int& r = *(int*)&value_; (void)&r; calls++; g_seq[g_n++] = 2; }

void __fastcall CVillagerInteractionsDef_Transfer(CVillagerInteractionsDef* def, void* edx, CPersistContext* ctx)
{
    ctx->Transfer(*(CDefString*)&(def->fieldA));
    ctx->Transfer(*(CDefString*)&(def->fieldB));
    ctx->Transfer(*(float*)&(def->fieldC));
}

int main() {
    CVillagerInteractionsDef d;
    CPersistContext c; c.calls = 0; g_n = 0;
    CVillagerInteractionsDef_Transfer(&d, 0, &c);
    if (c.calls == 3 && g_seq[0]==1 && g_seq[1]==1 && g_seq[2]==2) {
        std::printf("CVillagerInteractionsDef_00456f27_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL\n");
    return 1;
}