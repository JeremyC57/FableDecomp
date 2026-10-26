// behaviour test for 005bda1d: CCategory::GetHighlightedItemText -- the highlighted item's description, or (when it is
// empty, including a string with no data block) the TXT_OBJT_EMPTY text through the inventory when its tag exists
#include <cstdio>
#include <cstring>
class CWideString { public:
    const char* PStringData;               /* 0 = no data block */
    bool IsEmptyText() const
    {
        if (PStringData == 0) {
            if (std::strcmp("", "") == 0) return true;
        } else if (std::strcmp(PStringData, "") == 0)
            return true;
        return false;
    }
};
static long g_tagForEmpty = 0;
static const char* g_lastSymbol = 0;
static long GetTagFromCRC(const char* symbol) { g_lastSymbol = symbol; return g_tagForEmpty; }
static CWideString GetInventoryText(long textId) { CWideString w; w.PStringData = textId == 77 ? "Empty" : "?"; return w; }
struct CItem { const char* description; };
struct CCategory {
    CItem* ItemsInCategory_start;
    long HighlightedItemIndex;
    CWideString GetHighlightedItemText() const
    {
        CWideString text; text.PStringData = ItemsInCategory_start[HighlightedItemIndex].description;
        if (text.IsEmptyText()) {
            long textId = GetTagFromCRC("TXT_OBJT_EMPTY");
            if (textId > 0)
                return GetInventoryText(textId);
        }
        return text;
    }
};
int main()
{
    int bad = 0;
    CItem items[3] = { { "Sword" }, { "" }, { 0 } };
    CCategory c = { items, 0 };
    if (std::strcmp(c.GetHighlightedItemText().PStringData, "Sword") != 0 || g_lastSymbol != 0) ++bad;
    c.HighlightedItemIndex = 1; g_tagForEmpty = 77;
    if (std::strcmp(c.GetHighlightedItemText().PStringData, "Empty") != 0) ++bad;
    if (!g_lastSymbol || std::strcmp(g_lastSymbol, "TXT_OBJT_EMPTY") != 0) ++bad;
    c.HighlightedItemIndex = 2;
    if (std::strcmp(c.GetHighlightedItemText().PStringData, "Empty") != 0) ++bad;   /* no data block counts as empty */
    g_tagForEmpty = 0;                     /* no tag: the (empty) description itself */
    c.HighlightedItemIndex = 1;
    if (std::strcmp(c.GetHighlightedItemText().PStringData, "") != 0) ++bad;
    g_tagForEmpty = -3;
    c.HighlightedItemIndex = 2;
    if (c.GetHighlightedItemText().PStringData != 0) ++bad;
    if (bad) { std::printf("005bda1d_TEST FAIL\n"); return 1; }
    std::printf("005bda1d_TEST PASS\n");
    return 0;
}