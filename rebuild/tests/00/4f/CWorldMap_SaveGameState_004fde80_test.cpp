// 004fde80 behaviour test: CWorldMap::SaveGameState (match.cpp body, binary and text paths)
#include <stdio.h>
#include <string>
#include <vector>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { printf("FAIL line %d: %s\n", __LINE__, #c); g_fail = 1; } } while (0)
static std::vector<std::string> g_log;
struct CCharString {
    std::string S;
    CCharString(const char* text, long = -1) : S(text) {}
    CCharString(const CCharString& o) : S(o.S) {}
    ~CCharString() {}
};
CCharString operator+(const char* l, const CCharString& r) { return CCharString((std::string(l) + r.S).c_str()); }
CCharString operator+(const CCharString& l, const char* r) { return CCharString((l.S + r).c_str()); }
static int g_progress = 0;
void DisplayProgress(const CCharString& text, float, bool, bool) { ++g_progress; g_log.push_back("progress:" + text.S); }
struct CDataOutputStream { std::vector<long> Out; void WriteLong(long v) { Out.push_back(v); } };
struct CMemoryDataInputStream { long Next; };
template <class T> struct CPersistTraits {
    T DefaultVal;
    CPersistTraits() {}
    void TransferBinaryIn(CMemoryDataInputStream* stream, T* value) { *value = (T)stream->Next; }
};
struct CPersistContext {
    long Mode;
    CMemoryDataInputStream* PLoadStream;
    CDataOutputStream* PSaveStream;
    int Crc;
    void CheckCRC(const char*) { ++Crc; }
    void Transfer(CCharString& v) { g_log.push_back("transfer:" + v.S); }
    void WriteText(const CCharString& t) { g_log.push_back("text:" + t.S); }
    bool IsBinary() const { return Mode == 2 || Mode == 3; }
    void TransferBinary(unsigned long& value)
    {
        CheckCRC("");
        CPersistTraits<unsigned long> traits;
        switch (Mode) {
        case 3: PSaveStream->WriteLong(value); break;
        case 2: traits.TransferBinaryIn(PLoadStream, &value); break;
        }
    }
};
struct CRegion {
    CCharString Name;
    int Transferred;
    CRegion(const char* n) : Name(n), Transferred(0) {}
    void TransferGameState(CPersistContext&) { ++Transferred; g_log.push_back("region:" + Name.S); }
};
struct CWorldMap {
    std::vector<CRegion> Regions;
    void SaveGameState(CPersistContext& context);
};
void CWorldMap::SaveGameState(CPersistContext& context)
{
    unsigned long count = Regions.size();
    if (context.IsBinary()) {
        context.TransferBinary(count);
        for (unsigned long i = 1; i < count; ++i) {
            CCharString name(Regions[i].Name);
            context.Transfer(name);
            Regions[i].TransferGameState(context);
            { CCharString text(""); DisplayProgress(text, -1.0f, false, false); }
        }
    } else {
        for (unsigned long i = 1; i < count; ++i) {
            context.WriteText("REGION \"" + Regions[i].Name + "\"\r\n");
            Regions[i].TransferGameState(context);
            { CCharString end("END_REGION\r\n\r\n"); context.WriteText(end); }
            { CCharString text(""); DisplayProgress(text, -1.0f, false, false); }
        }
    }
}
int main()
{
    CWorldMap m; m.Regions.push_back(CRegion("R0")); m.Regions.push_back(CRegion("R1")); m.Regions.push_back(CRegion("R2"));
    // save (mode 3): count written, regions 1..2 transferred, region 0 skipped
    CDataOutputStream out; CMemoryDataInputStream in; in.Next = 2;
    CPersistContext save; save.Mode = 3; save.PSaveStream = &out; save.PLoadStream = &in; save.Crc = 0;
    g_log.clear(); g_progress = 0;
    m.SaveGameState(save);
    CHECK(out.Out.size() == 1 && out.Out[0] == 3);
    CHECK(save.Crc == 1);
    CHECK(m.Regions[0].Transferred == 0 && m.Regions[1].Transferred == 1 && m.Regions[2].Transferred == 1);
    CHECK(g_progress == 2);
    CHECK(g_log.size() == 6 && g_log[0] == "transfer:R1" && g_log[1] == "region:R1" && g_log[3] == "transfer:R2");
    // load (mode 2): the loaded count (2) bounds the loop
    CPersistContext load = save; load.Mode = 2; out.Out.clear(); g_log.clear(); g_progress = 0;
    m.SaveGameState(load);
    CHECK(out.Out.empty());
    CHECK(m.Regions[1].Transferred == 2 && m.Regions[2].Transferred == 1);
    CHECK(g_progress == 1);
    // text mode: header, region, END_REGION, progress per region
    CPersistContext text = save; text.Mode = 0; text.Crc = 0; g_log.clear(); g_progress = 0;
    m.SaveGameState(text);
    CHECK(text.Crc == 0);
    CHECK(g_log.size() == 8);
    CHECK(g_log[0] == "text:REGION \"R1\"\r\n" && g_log[1] == "region:R1" && g_log[2] == "text:END_REGION\r\n\r\n" && g_log[3] == "progress:");
    CHECK(g_log[4] == "text:REGION \"R2\"\r\n");
    if (!g_fail) printf("004fde80_TEST PASS\n");
    return g_fail;
}