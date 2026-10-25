// Test for 0x00415DD0 CDefString::operator CCharString() const: the conversion returns the global table's string for
// TablePos, constructed into the caller's result.
#include <stdio.h>
class CCharString {
public:
    void* PStringData;
};
class CDefStringTable {
public:
    long lastIndex; int calls;
    CCharString GetString(long index) const;
};
CDefStringTable g_DefStringTable;
CCharString CDefStringTable::GetString(long index) const
{
    CDefStringTable* self = const_cast<CDefStringTable*>(this);
    self->lastIndex = index; ++self->calls;
    CCharString s; s.PStringData = (void*)(0x1000 + index);
    return s;
}
class CDefString {
public:
    long TablePos;
    operator CCharString() const;
};
int main()
{
    CDefString d; d.TablePos = 0x2A;
    CCharString s = d;
    if (s.PStringData != (void*)0x102A || g_DefStringTable.lastIndex != 0x2A || g_DefStringTable.calls != 1) return 1;
    d.TablePos = -1;
    CCharString t = (CCharString)d;
    if (t.PStringData != (void*)(0x1000 - 1) || g_DefStringTable.lastIndex != -1 || g_DefStringTable.calls != 2) return 2;
    printf("PASS_00415dd0\n");
    return 0;
}
