#include <cstdio>

class CDefClassBase { public:
    char pad[0x28];
    unsigned long field28;

public:
    CDefClassBase& operator=(const CDefClassBase& other);
};

struct BaseDef {
    void Clone(CDefClassBase const* src);
};

class CInterestingToVillagersDef : public BaseDef { public:
    char pad[0x28 - sizeof(BaseDef)];
    unsigned long field28;
};

static bool g_cloned = false;
CDefClassBase& CDefClassBase::operator=(const CDefClassBase& other_)
{
    CDefClassBase const* src = (CDefClassBase const*)&other_; (void)src; g_cloned = true;
    return *this;
}

void __fastcall CInterestingToVillagersDef__Copy(CInterestingToVillagersDef* self, void* /*edx*/, CDefClassBase const* src);

int main() {
    CInterestingToVillagersDef dst;
    CDefClassBase src;
    dst.field28 = 0;
    src.field28 = 0xDEADBEEF;
    CInterestingToVillagersDef__Copy(&dst, 0, &src);
    if (g_cloned && dst.field28 == 0xDEADBEEF)
        printf("004e0e0a_TEST PASS\n");
    else
        printf("FAIL cloned=%d f28=%08x\n", g_cloned, dst.field28);
    return 0;
}