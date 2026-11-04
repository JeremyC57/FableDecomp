// Behaviour test for CStringParser::ReadAsFloat (0x009BA650): a model of the row -- a float item returns its value, an
// integer item converts, anything else (or no item) reports "Error parsing float" once and returns 0.
#include <stdio.h>
#include <string.h>
enum EParsedItemType { PARSED_ITEM_NULL, PARSED_ITEM_SYMBOL, PARSED_ITEM_STRING, PARSED_ITEM_INTEGER, PARSED_ITEM_FLOAT };
struct CParsedItem {
    EParsedItemType Type; union { long Integer; float Float; };
    bool IsFloat() const { return Type == PARSED_ITEM_FLOAT; }
    bool IsInteger() const { return Type == PARSED_ITEM_INTEGER; }
    float GetFloat() const { return Float; }
    long GetInteger() const { return Integer; }
};
struct CStringParser {
    bool HasItem; CParsedItem Next; int Errors; const char* LastError;
    bool ReadNextItem(CParsedItem& item) { if (!HasItem) return false; item = Next; return true; }
    void Error(const char* message) { ++Errors; LastError = message; }
    float ReadAsFloat()
    {
        CParsedItem item;
        if (!ReadNextItem(item) || (!item.IsFloat() && !item.IsInteger())) {
            Error("Error parsing float");
            return 0.0f;
        }
        if (item.IsFloat())
            return item.GetFloat();
        return item.GetInteger();
    }
};
int main()
{
    CStringParser p; p.Errors = 0; p.LastError = 0;
    p.HasItem = true; p.Next.Type = PARSED_ITEM_FLOAT; p.Next.Float = 2.5f;
    int ok = p.ReadAsFloat() == 2.5f;
    p.Next.Type = PARSED_ITEM_INTEGER; p.Next.Integer = -7;
    ok = ok && p.ReadAsFloat() == -7.0f && p.Errors == 0;
    p.Next.Type = PARSED_ITEM_SYMBOL;
    ok = ok && p.ReadAsFloat() == 0.0f && p.Errors == 1 && strcmp(p.LastError, "Error parsing float") == 0;
    p.HasItem = false;
    ok = ok && p.ReadAsFloat() == 0.0f && p.Errors == 2;
    printf(ok ? "009ba650_TEST PASS\n" : "009ba650_TEST FAIL\n");
    return ok ? 0 : 1;
}