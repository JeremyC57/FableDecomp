// Test for CFontDataBankEntry::~CFontDataBankEntry (0x00a63480): the destructor's Unload() releases the Font counted pointer (deleter + count block on the
// last reference, Info then Data cleared), the member destructor then finds Info null and does nothing more, and the base
// destructor runs last.
#include <cstdio>
static int g_deleter = 0, g_baseDtor = 0, g_infoFrees = 0;
struct Info { unsigned long RefCount; void (*DeleteFunc)(void*); void* Data; };
static void DeleteFont(void*) { ++g_deleter; }
template <class T> struct CountedPointer {
    T* Data; Info* Info_;
    void Release() { if (Info_) { --Info_->RefCount; if (Info_->RefCount == 0) { Info_->DeleteFunc(Info_->Data); delete Info_; ++g_infoFrees; } } }
    void ReleaseData() { Release(); Info_ = 0; Data = 0; }
    ~CountedPointer() { Release(); Data = 0; Info_ = 0; }
};
struct Base { virtual ~Base() { ++g_baseDtor; } };
struct FontBank { int id; };
class CFontDataBankEntry : public Base { public:
    CountedPointer<FontBank> Font;
    void Unload() { Font.ReleaseData(); }
    ~CFontDataBankEntry() { Unload(); if (Font.Info_ != 0 || Font.Data != 0) g_deleter += 100; }
};
int main()
{
    FontBank bank;
    {
        CFontDataBankEntry* e = new CFontDataBankEntry; e->Font.Data = &bank;
        e->Font.Info_ = new Info; e->Font.Info_->RefCount = 1; e->Font.Info_->DeleteFunc = DeleteFont; e->Font.Info_->Data = &bank;
        delete e;
    }
    if (g_deleter != 1 || g_infoFrees != 1 || g_baseDtor != 1) { std::printf("00a63480_TEST FAIL\n"); return 1; }
    {
        Info* shared = new Info; shared->RefCount = 2; shared->DeleteFunc = DeleteFont; shared->Data = &bank;
        CFontDataBankEntry* e = new CFontDataBankEntry; e->Font.Data = &bank; e->Font.Info_ = shared;
        delete e;
        if (shared->RefCount != 1 || g_deleter != 1 || g_infoFrees != 1 || g_baseDtor != 2) { std::printf("00a63480_TEST FAIL\n"); return 1; }
        delete shared;
    }
    {
        CFontDataBankEntry* e = new CFontDataBankEntry; e->Font.Data = 0; e->Font.Info_ = 0;
        delete e;
        if (g_deleter != 1 || g_baseDtor != 3) { std::printf("00a63480_TEST FAIL\n"); return 1; }
    }
    std::printf("00a63480_TEST PASS\n");
    return 0;
}