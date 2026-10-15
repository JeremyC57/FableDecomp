#include <cstdio>
#include <cstring>

static int g_baseCreates = 0;
struct CThing { unsigned long m_flags; unsigned long GetStateFlags() { return m_flags; } };
struct CTCActionUseBase { CThing* ParentThing; void OnCreate() { ++g_baseCreates; } };
struct CTCActionUseTorch : CTCActionUseBase {
    bool TorchLit;
    void OnCreate() { CTCActionUseBase::OnCreate(); TorchLit = !((ParentThing->GetStateFlags() >> 8) & 1); }
};
int main()
{
    CThing torch; torch.m_flags = 0x100;
    CTCActionUseTorch t; t.ParentThing = &torch; t.TorchLit = true;
    t.OnCreate();
    if (t.TorchLit || g_baseCreates != 1) { std::printf("007eaf20_TEST FAIL\n"); return 1; }
    torch.m_flags = 0xfeff; t.OnCreate();
    if (!t.TorchLit || g_baseCreates != 2) { std::printf("007eaf20_TEST FAIL\n"); return 1; }
    std::printf("007eaf20_TEST PASS\n");
    return 0;
}
