#include <cstdio>
#include <cstring>
struct CComponent { int id; };
struct CEditBox;
class CWideString { public: void* PStringData; CWideString() : PStringData(0) {} };
struct CVirtualKeyboard {
    CWideString ProfileName; bool Canceled; bool Done; CComponent* PEntryScreen; CEditBox* PEntryBox;
    CVirtualKeyboard(CComponent* entryScreen) : Canceled(false), Done(false), PEntryScreen(entryScreen), PEntryBox(0) {}
};
int main()
{
    int ok = 1;
    CComponent screen;
    CVirtualKeyboard k(&screen);
    if (k.ProfileName.PStringData != 0 || k.Canceled || k.Done || k.PEntryScreen != &screen || k.PEntryBox != 0) ok = 0;
    if (!ok) { std::printf("00851700_TEST FAIL\n"); return 1; }
    std::printf("00851700_TEST PASS\n");
    return 0;
}