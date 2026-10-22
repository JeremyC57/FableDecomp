struct CResult {
    void Init(void* p);
};

struct CCategory {
    char pad[0x30];
    char* base;    /* +0x30 */
    char pad2[8];
    int count;     /* +0x3c */
};

long __fastcall CCategory_GetHighlightedItemCountInCategory(CCategory* category, void* edx, CResult* out)
{
    (void)edx;
    void* p = (void*)(category->base + category->count * 0x44 + 0xc);
    out->Init(p);
    return (long)out;
}