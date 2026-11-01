// Test for CSoundBankLH::~CSoundBankLH (0x00A0B9A0): the library bank object is released (its slot 0) and the pointer
// cleared, then the +0x14 counted pointer, then the CASoundBank counted pointer at +4, then the base destructor.
#include <cstdio>
static char g_log[16]; static int g_n = 0;
struct Info { unsigned long RefCount; void (*DeleteFunc)(void*); void* Data; };
static void DeleteRes(void*) { g_log[g_n++] = 'R'; }
static void DeleteData(void*) { g_log[g_n++] = 'D'; }
template <class T> struct CountedPointer {
    T* Data; Info* Info_;
    ~CountedPointer() { if (Info_) { --Info_->RefCount; if (Info_->RefCount == 0) { Info_->DeleteFunc(Info_->Data); delete Info_; } } Data = 0; Info_ = 0; }
};
struct BankObject { virtual void Release() { g_log[g_n++] = 'B'; } };
struct Base { virtual ~Base() { g_log[g_n++] = 'X'; } };
struct ASoundBank : Base { CountedPointer<int> Data; };
struct SoundBankLH : ASoundBank {
    void* Owner; BankObject* Bank; CountedPointer<int> Resource; bool Loaded;
    void Unload() { if (Bank) { Bank->Release(); Bank = 0; } }
    ~SoundBankLH() { Unload(); }
};
static Info* MakeInfo(void (*f)(void*)) { Info* i = new Info; i->RefCount = 1; i->DeleteFunc = f; i->Data = 0; return i; }
int main()
{
    BankObject obj; int a = 0, b = 0;
    SoundBankLH* p = new SoundBankLH;
    p->Owner = 0; p->Bank = &obj; p->Loaded = true;
    p->Resource.Data = &a; p->Resource.Info_ = MakeInfo(DeleteRes);
    p->Data.Data = &b; p->Data.Info_ = MakeInfo(DeleteData);
    delete p;
    if (g_n != 4 || g_log[0] != 'B' || g_log[1] != 'R' || g_log[2] != 'D' || g_log[3] != 'X') { std::printf("00a0b9a0_TEST FAIL\n"); return 1; }
    p = new SoundBankLH; p->Bank = 0; p->Resource.Data = 0; p->Resource.Info_ = 0; p->Data.Data = 0; p->Data.Info_ = 0;
    delete p;
    if (g_n != 5 || g_log[4] != 'X') { std::printf("00a0b9a0_TEST FAIL\n"); return 1; }
    std::printf("00a0b9a0_TEST PASS\n");
    return 0;
}