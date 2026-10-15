#include <cstdio>
#include <string.h>
#define FAIL { std::printf("0099f450_TEST FAIL\n"); return 1; }
struct CCharStringData { char PData[64]; unsigned long StringLength; long NoRefs; };
long g_CCharStringInstanceCount = 0;
char g_EmptyCharString[] = "";
struct CCharString {
    CCharStringData* PStringData;
    CCharString() : PStringData(0) { ++g_CCharStringInstanceCount; }
    CCharString(const CCharString& o) : PStringData(o.PStringData) { ++g_CCharStringInstanceCount; if (PStringData) ++PStringData->NoRefs; }
    ~CCharString() { UnassignString(); --g_CCharStringInstanceCount; }
    CCharString& operator=(const CCharString& other)
    {
        if (&other != this) {
            if (PStringData != 0) UnassignString();
            if (other.PStringData != 0) { PStringData = other.PStringData; ++PStringData->NoRefs; }
        }
        return *this;
    }
    void UnassignString() { if (PStringData) { if (--PStringData->NoRefs <= 0) delete PStringData; PStringData = 0; } }
    CCharString GetSubString(unsigned long start, unsigned long count) const
    {
        CCharString r; r.PStringData = new CCharStringData; r.PStringData->NoRefs = 1;
        unsigned long len = PStringData->StringLength;
        if (count > len - start) count = len - start;
        memcpy(r.PStringData->PData, PStringData->PData + start, count); r.PStringData->PData[count] = 0; r.PStringData->StringLength = count;
        return r;
    }
    unsigned long GetLength() const { return PStringData == 0 ? 0 : PStringData->StringLength; }
    const char* GetText() const { return PStringData == 0 ? g_EmptyCharString : PStringData->PData; }
    void RemoveWhiteSpaceBegin()
    {
        unsigned long i = 0;
        while (i < GetLength()) {
            char c = GetText()[i];
            if (c == ' ' || c == '\t' || c == '\r' || c == '\n') { ++i; continue; }
            if (i > 0) { CCharString rest = GetSubString(i, GetLength()); *this = rest; }
            return;
        }
    }
};
static CCharString* Make(const char* t)
{
    CCharString* s = new CCharString; s->PStringData = new CCharStringData; strcpy(s->PStringData->PData, t);
    s->PStringData->StringLength = (unsigned long)strlen(t); s->PStringData->NoRefs = 1; return s;
}
int main()
{
    CCharString* a = Make(" \t\r\nabc ");
    a->RemoveWhiteSpaceBegin();
    if (strcmp(a->GetText(), "abc ") != 0 || a->GetLength() != 4 || a->PStringData->NoRefs != 1) FAIL;
    CCharString* b = Make("xyz");
    CCharStringData* before = b->PStringData; b->RemoveWhiteSpaceBegin();
    if (b->PStringData != before) FAIL;
    CCharString* c = Make("   ");
    before = c->PStringData; c->RemoveWhiteSpaceBegin();
    if (c->PStringData != before || c->GetLength() != 3) FAIL;
    CCharString e; e.RemoveWhiteSpaceBegin(); if (e.PStringData != 0) FAIL;
    delete a; delete b; delete c;
    if (g_CCharStringInstanceCount != 1) FAIL;
    std::printf("0099f450_TEST PASS\n");
    return 0;
}