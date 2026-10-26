// Behaviour test for 00a0d200: src/00a0d200.cpp copied verbatim; the CWideString members and CWStringParser::ReadNextItem
// are stubbed so the symbol return, the wrong-type throw and the failed-read throw are observable. CWStringParser::ReadAsSymbol()
#ifndef _MSC_VER
#define __declspec(x)
#endif
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CWStringParser::ReadAsSymbol() -- reads the next item and returns its text when it is a symbol item (type 1); otherwise throws CGenericException
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A0D200, 96 bytes (the int3 VC7.1 emits after the final noreturn call included), `ret 4` (hidden CWideString result). Callees:
// CWideString::CWideString() 0x0099AED0, CWStringParser::ReadNextItem(CWParsedItem*) 0x00A0CFC0,
// CWideString::CWideString(const CWideString&) 0x0099B720, ~CWideString 0x0099B510,
// _CxxThrowException 0x00BFEB84 with ThrowInfo 0x013692F8 (.?AVCGenericException@@ : CExceptionBase).
// The wide twin of the landed CStringParser::ReadAsSymbol 0x009BA790, which reports an error instead of throwing.
// Shape: a CWParsedItem local (Type +0, value +4, String +8 -- only the string has a constructor); `return
// item.String;` copies into the hidden result before the item's destructor; the throw path is placed last,
// the empty exception object is copied byte-wise from its dead slot (the hidden-result argument slot),
// and there is no EH frame (the TU is built without /EHsc, so the item is not unwound).
#include <stddef.h>   // wchar_t = unsigned short under VC7.1 without /Zc:wchar_t

class CWideStringData;
class CWideString { public:
    CWideStringData* PStringData;
    CWideString();                                   // 0x0099AED0
    CWideString(const CWideString& other);           // 0x0099B720
    ~CWideString();                                  // 0x0099B510
};

class CWParsedItem { public:
    long        Type;      // +0x00 EParsedItemType (1 = symbol, 2 = string)
    long        Integer;   // +0x04
    CWideString String;    // +0x08
};

class CExceptionBase { };
class CGenericException : public CExceptionBase {
public:
    CGenericException() { }
};

struct CWStringParser {
    void*          __vftable;  // +0x00
    wchar_t*       String;     // +0x04
    unsigned long  StringPos;  // +0x08
    unsigned long  Line;       // +0x0C
    bool ReadNextItem(CWParsedItem* item);           // 0x00A0CFC0
    CWideString ReadAsSymbol();
};

CWideString CWStringParser::ReadAsSymbol()
{
    CWParsedItem item;
    if (ReadNextItem(&item) && item.Type == 1)
        return item.String;
    throw CGenericException();
}

// ---- stubs ----
static int g_ctor = 0, g_copy = 0, g_dtor = 0;
CWideString::CWideString() : PStringData(0) { ++g_ctor; }
CWideString::CWideString(const CWideString& other) : PStringData(other.PStringData) { ++g_copy; }
CWideString::~CWideString() { ++g_dtor; }
static bool g_ok = true; static long g_type = 1; static CWideStringData* g_text = (CWideStringData*)0x1234; static int g_reads = 0;
static CWStringParser* g_this = 0;
bool CWStringParser::ReadNextItem(CWParsedItem* item)
{
    ++g_reads; g_this = this;
    item->Type = g_type; item->Integer = 7; item->String.PStringData = g_text;
    return g_ok;
}
static void Reset() { g_ctor = g_copy = g_dtor = 0; g_reads = 0; g_this = 0; }
int main()
{
    CWStringParser p; memset(&p, 0, sizeof(p));
    // a symbol item (type 1): its text is returned, the item string is constructed once and destroyed once
    Reset(); g_ok = true; g_type = 1;
    {
        CWideString r = p.ReadAsSymbol();
        CHECK(r.PStringData == g_text); CHECK(g_reads == 1 && g_this == &p);
        CHECK(g_ctor == 1 && g_copy == 1 && g_dtor == 1);
    }
    CHECK(g_dtor == 2);
    // a string item (type 2) is the wrong type: throws CGenericException
    Reset(); g_ok = true; g_type = 2; bool caught = false;
    try { CWideString r = p.ReadAsSymbol(); CHECK(!"no throw"); }
    catch (CGenericException&) { caught = true; }
    CHECK(caught); CHECK(g_reads == 1 && g_copy == 0);
    // the read itself fails (even with a symbol-typed item): throws, the type is not consulted
    Reset(); g_ok = false; g_type = 1; caught = false;
    try { CWideString r = p.ReadAsSymbol(); CHECK(!"no throw"); }
    catch (CGenericException&) { caught = true; }
    CHECK(caught); CHECK(g_reads == 1 && g_copy == 0);
    if (fails == 0) printf("00a0d200_TEST PASS\n"); else printf("00a0d200_TEST FAIL\n");
    return fails;
}