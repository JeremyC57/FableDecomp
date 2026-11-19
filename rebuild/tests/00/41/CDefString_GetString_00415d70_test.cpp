// Test for 0x00415D70 CDefString::GetString() const: returns the global def-string table's string for TablePos, constructed into the
// caller's result; the table is asked once per call with exactly TablePos.
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
CDefStringTable GDefStringTable;
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
    CCharString GetString() const;
};
CCharString CDefString::GetString() const
{
    return GDefStringTable.GetString(TablePos);
}
int main()
{
    CDefString d; d.TablePos = 0x2A;
    CCharString s = d.GetString();
    if (s.PStringData != (void*)0x102A || GDefStringTable.lastIndex != 0x2A || GDefStringTable.calls != 1) return 1;
    d.TablePos = -1;
    CCharString t = d.GetString();
    if (t.PStringData != (void*)(0x1000 - 1) || GDefStringTable.lastIndex != -1 || GDefStringTable.calls != 2) return 2;
    printf("PASS_00415d70\n");
    return 0;
}
