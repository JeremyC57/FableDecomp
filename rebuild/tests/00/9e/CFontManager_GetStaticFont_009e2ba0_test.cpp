// Behaviour test for 0x009E2BA0 CFontManager::GetStaticFont: the returned counted pointer holds one extra reference and the font points back at the manager.
#include <cstdio>
struct Info { unsigned int RefCount; };
struct CFontManager;
struct CFontBank { CFontManager* PManager; };
struct CP {
    CFontBank* Data; Info* Info_;
    CP(CFontBank* d, Info* i) : Data(d), Info_(i) { if (Info_) ++Info_->RefCount; }
    CP(const CP& o) : Data(o.Data), Info_(o.Info_) { if (Info_) ++Info_->RefCount; }
    ~CP() { if (Info_) --Info_->RefCount; }
    CFontBank* operator->() const { return Data; }
};
static CFontBank g_fonts[3]; static Info g_infos[3];
struct Entry { int i; CP AcquireFont() { return CP(&g_fonts[i], &g_infos[i]); } };
static Entry g_entries[3] = { { 0 }, { 1 }, { 2 } };
struct Bank { Entry* GetFontInfo(unsigned int index) { return &g_entries[index]; } };
struct CFontManager {
    Bank* bank;
    CP GetStaticFont(unsigned int index) { CP font = bank->GetFontInfo(index)->AcquireFont(); font->PManager = this; return font; }
};
int main()
{
    Bank b; CFontManager m; m.bank = &b;
    g_infos[2].RefCount = 1;
    bool ok;
    {
        CP f = m.GetStaticFont(2);
        ok = f.Data == &g_fonts[2] && g_fonts[2].PManager == &m && g_infos[2].RefCount == 2;
    }
    ok = ok && g_infos[2].RefCount == 1;
    if (!ok) { std::printf("009e2ba0_TEST FAIL\n"); return 1; }
    std::printf("009e2ba0_TEST PASS\n");
    return 0;
}