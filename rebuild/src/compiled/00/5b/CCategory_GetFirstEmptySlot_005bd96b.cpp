#pragma optimize("s",on)
struct CCategory {
    char pad[8];
    char* m_begin;
    char* m_end;
};
extern int g_predicate;
extern char* __fastcall find_slot(char* begin, char* end, int* pred);
long __fastcall CCategory_GetFirstEmptySlot(CCategory* category)
{
    char* end = category->m_end;
    char* begin = category->m_begin;
    char* found = find_slot(begin, end, &g_predicate);
    if (found != category->m_end)
        return (long)(found - category->m_begin) / 0x14;
    return -1;
}