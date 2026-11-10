// behaviour test for 0x005D8978 CPersistContext::TransferMap<long,unsigned long> (body copied from the row source; self-contained stubs)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <map>
class CCharString {
public:
    std::string S;
    CCharString(const char* t = "") : S(t) { }
    CCharString& operator+=(const char* t) { S += t; return *this; }
    bool operator==(const char* t) const { return S == t; }
};
static const char* g_tokens[32]; static int g_pos; static int g_errors; static std::string g_lastError;
static int g_found; static int g_cleaned; static int g_binIn; static int g_binOut;
class CStringParser {
public:
    bool SkipPastWholeString(const char* name)
    { while (g_tokens[g_pos]) { if (strcmp(g_tokens[g_pos++], name) == 0) return true; } return false; }
    long GetStringPos() const { return g_pos * 10; }
    CCharString ReadAsSymbol() { const char* t = g_tokens[g_pos]; if (t) ++g_pos; return CCharString(t ? t : ""); }
};
class CDataInputStream { }; class CDataOutputStream { };
static void GFSerialiseMapBinaryIn(CDataInputStream&, std::map<unsigned long,long>&) { ++g_binIn; }
static void GFSerialiseMapBinaryOut(CDataOutputStream&, const std::map<unsigned long,long>&) { ++g_binOut; }
enum EPersistMode { MODE_LOAD_TEXT, MODE_SAVE_TEXT, MODE_LOAD_BINARY, MODE_SAVE_BINARY, MODE_CLEAR };
class CPersistContext;
template <class T> class CPersistTraits {
public:
    void TransferOut(CPersistContext&, CCharString& out, const T& value) { char b[32]; sprintf(b, "%ld", (long)value); out += b; }
    void TransferIn(CPersistContext&, CStringParser&, T& value) { const char* t = g_tokens[g_pos]; if (t) ++g_pos; value = (T)atol(t ? t : "0"); }
};
class CPersistContext {
public:
    EPersistMode Mode; bool Finished; CCharString* PStringToSaveTo; CStringParser* PParser;
    CDataInputStream* PLoadStream; CDataOutputStream* PSaveStream;
    void CheckCRC(const char*) { }
    void SetAsFoundItem() { ++g_found; }
    void CleanOutLine(long, long) { ++g_cleaned; }
    void PersistError(const CStringParser&, const char* message) const { ++g_errors; g_lastError = message; }
    void TransferMap(const char* name, std::map<unsigned long,long>& map, const long& defaultValue);
};
void CPersistContext::TransferMap(const char* name, std::map<unsigned long,long>& map, const long& defaultValue)
{
    CheckCRC(name);
    switch (Mode) {
    case MODE_SAVE_TEXT:
        for (std::map<unsigned long,long>::iterator it = map.begin(); it != map.end(); ++it) {
            CPersistTraits<unsigned long> keyTraits;
            CPersistTraits<long> valueTraits;
            *PStringToSaveTo += name;
            *PStringToSaveTo += "[";
            keyTraits.TransferOut(*this, *PStringToSaveTo, it->first);
            *PStringToSaveTo += "] ";
            valueTraits.TransferOut(*this, *PStringToSaveTo, it->second);
            *PStringToSaveTo += ";\r\n";
        }
        break;
    case MODE_LOAD_BINARY:
        GFSerialiseMapBinaryIn(*PLoadStream, map);
        break;
    case MODE_SAVE_BINARY:
        GFSerialiseMapBinaryOut(*PSaveStream, map);
        break;
    case MODE_CLEAR:
        map.clear();
        break;
    default: {
        CStringParser parser(*PParser);
        long nameLength = strlen(name);
        while (!Finished && parser.SkipPastWholeString(name)) {
            long start = parser.GetStringPos() - nameLength;
            if (parser.ReadAsSymbol() == "[") {
                unsigned long key;
                CPersistTraits<unsigned long>().TransferIn(*this, parser, key);
                if (parser.ReadAsSymbol() == "]") {
                    long value;
                    CPersistTraits<long>().TransferIn(*this, parser, value);
                    SetAsFoundItem();
                    map.erase(key);
                    map.insert(std::map<unsigned long,long>::value_type(key, value));
                    if (parser.ReadAsSymbol() == ";")
                        CleanOutLine(start, parser.GetStringPos());
                    else
                        PersistError(parser, "Unable to find terminating semicolon.");
                } else
                    PersistError(parser, "Unable to find terminating bracket.");
            } else
                PersistError(parser, "Unable to find opening bracket.");
        }
        break;
    }
    }
}
int main()
{
    std::map<unsigned long,long> m; m[3] = -7; m[1] = 42;
    CCharString out; CStringParser parser; CDataInputStream in; CDataOutputStream os;
    CPersistContext c; c.Finished = false; c.PStringToSaveTo = &out; c.PParser = &parser; c.PLoadStream = &in; c.PSaveStream = &os;
    long def = 0; bool ok = true;
    c.Mode = MODE_SAVE_TEXT; c.TransferMap("Ids", m, def);
    ok = ok && out.S == "Ids[1] 42;\r\nIds[3] -7;\r\n";
    const char* script[] = { "x", "Ids", "[", "3", "]", "9", ";", "Ids", "[", "5", "]", "11", "!", "Ids", "(", 0 };
    for (int k = 0; k < 16; ++k) g_tokens[k] = script[k];
    c.Mode = MODE_LOAD_TEXT; c.TransferMap("Ids", m, def);
    ok = ok && m.size() == 3 && m[3] == 9 && m[5] == 11 && m[1] == 42 && g_found == 2 && g_cleaned == 1 && g_errors == 2
        && g_lastError == "Unable to find opening bracket.";
    c.Mode = MODE_LOAD_BINARY; c.TransferMap("Ids", m, def); c.Mode = MODE_SAVE_BINARY; c.TransferMap("Ids", m, def);
    ok = ok && g_binIn == 1 && g_binOut == 1;
    c.Mode = MODE_CLEAR; c.TransferMap("Ids", m, def); ok = ok && m.empty();
    printf(ok ? "005d8978_TEST PASS\n" : "005d8978_TEST FAIL\n");
    return ok ? 0 : 1;
}