#include <cstdio>
#include <cstring>
static int g_live = 0;
struct CWideString {
    char text[16];
    CWideString() { text[0] = 0; ++g_live; }
    CWideString(const CWideString& o) { std::strcpy(text, o.text); ++g_live; }
    CWideString& operator=(const CWideString& o) { std::strcpy(text, o.text); return *this; }
    ~CWideString() { --g_live; }
};
struct CCategory {
    char label[16]; unsigned char _body[0x2C - 16];
    CWideString GetCategoryName() const { CWideString s; std::strcpy(s.text, label); return s; }
};
struct CCategoryVector { CCategory* first; CCategory* last; CCategory* end_of_storage; };
struct Inv {
    CCategoryVector Categories;
    void GetCategoryNameWithIndex(long index, CWideString& name)
    {
        CCategory* first = Categories.first;
        name = first[index].GetCategoryName();
    }
};
int main()
{
    CCategory cats[3]; std::strcpy(cats[0].label, "Weapons"); std::strcpy(cats[1].label, "Food"); std::strcpy(cats[2].label, "Quest");
    Inv inv; inv.Categories.first = cats; inv.Categories.last = cats + 3; inv.Categories.end_of_storage = cats + 3;
    {
        CWideString out;
        inv.GetCategoryNameWithIndex(1, out);
        if (std::strcmp(out.text, "Food") || g_live != 1) { std::printf("005bdb68_TEST FAIL\n"); return 1; }
        inv.GetCategoryNameWithIndex(2, out);
        if (std::strcmp(out.text, "Quest")) { std::printf("005bdb68_TEST FAIL\n"); return 1; }
    }
    if (g_live != 0) { std::printf("005bdb68_TEST FAIL\n"); return 1; }
    std::printf("005bdb68_TEST PASS\n");
    return 0;
}