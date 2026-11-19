// Test for 0x00415DD0 CDefString::operator CCharString() const: the implicit conversion yields the global def-string table's string for
// TablePos (through GetString), constructed into the caller's result; the table is asked once per conversion with exactly TablePos.
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
    operator CCharString() const;
    CCharString GetString() const { return GDefStringTable.GetString(TablePos); }
};
CDefString::operator CCharString() const
{
    return GetString();
}
static void* Take(const CCharString& s) { return s.PStringData; }
int main()
{
    CDefString d; d.TablePos = 0x2A;
    CCharString s = d;
    if (s.PStringData != (void*)0x102A || GDefStringTable.lastIndex != 0x2A || GDefStringTable.calls != 1) return 1;
    d.TablePos = -1;
    if (Take(d) != (void*)(0x1000 - 1) || GDefStringTable.lastIndex != -1 || GDefStringTable.calls != 2) return 2;
    printf("PASS_00415dd0\n");
    return 0;
}
