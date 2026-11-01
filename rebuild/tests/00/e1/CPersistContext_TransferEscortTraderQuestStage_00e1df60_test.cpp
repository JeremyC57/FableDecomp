// Behaviour test for CPersistContext::Transfer<EscortTraderQuestStage> (00e1df60): the five persist modes on a 4-byte enum member
#include <cstdio>
enum EscortTraderQuestStage { E0 = 0, E1 = 1, E2 = 2, E7 = 7 };
static long g_written = -1;
static int g_lastText = -1;          // 0 load, 1 save
static const char* g_lastName = 0;
static int g_crcChecks = 0;
class CDataOutputStream { public: void WriteSLONG(long v) { g_written = v; } };
class CMemoryDataInputStream { public: long next; };
template <class T> struct CPersistTraits {
    T DefaultVal;
    CPersistTraits() : DefaultVal((T)0) {}
    void TransferBinaryIn(CMemoryDataInputStream& s, T& v) { v = (T)s.next; }
    void TransferBinaryOut(CDataOutputStream& s, T v) { s.WriteSLONG(v); }
};
struct Component { EscortTraderQuestStage* Value; Component(EscortTraderQuestStage& v) : Value(&v) {} };
class CPersistContext { public:
    bool UseDefaults;
    long Mode;
    CMemoryDataInputStream* PLoadStream;
    CDataOutputStream* PSaveStream;
    void CheckCRC(const char*) { ++g_crcChecks; }
    void TransferObjectSaveText(const char* name, Component& c) { g_lastText = 1; g_lastName = name; (void)c; }
    void TransferObjectLoadText(const char* name, Component& c) { g_lastText = 0; g_lastName = name; *c.Value = E2; }
    void Transfer(const char* name, EscortTraderQuestStage& value, const EscortTraderQuestStage& defaultValue)
    {
        CheckCRC(name);
        CPersistTraits<EscortTraderQuestStage> traits;
        switch (Mode) {
        case 4: value = defaultValue; break;
        case 3: traits.TransferBinaryOut(*PSaveStream, value); break;
        case 2: traits.TransferBinaryIn(*PLoadStream, value); break;
        case 1: { Component item(value); TransferObjectSaveText(name, item); break; }
        case 0: { if (UseDefaults) value = defaultValue; Component item(value); TransferObjectLoadText(name, item); break; }
        }
    }
};
int main()
{
    CDataOutputStream out; CMemoryDataInputStream in; in.next = 7;
    CPersistContext c; c.UseDefaults = false; c.PLoadStream = &in; c.PSaveStream = &out;
    EscortTraderQuestStage v = E1; const EscortTraderQuestStage def = E0;
    c.Mode = 4; c.Transfer("k", v, def); if (v != E0) { std::printf("00e1df60_TEST FAIL\n"); return 1; }
    v = E1; c.Mode = 3; c.Transfer("k", v, def); if (g_written != 1) { std::printf("00e1df60_TEST FAIL\n"); return 1; }
    c.Mode = 2; c.Transfer("k", v, def); if (v != E7) { std::printf("00e1df60_TEST FAIL\n"); return 1; }
    c.Mode = 1; c.Transfer("key1", v, def); if (g_lastText != 1 || g_lastName[3] != '1' || v != E7) { std::printf("00e1df60_TEST FAIL\n"); return 1; }
    c.Mode = 0; c.Transfer("key0", v, def); if (g_lastText != 0 || v != E2) { std::printf("00e1df60_TEST FAIL\n"); return 1; }
    if (g_crcChecks != 5) { std::printf("00e1df60_TEST FAIL\n"); return 1; }
    std::printf("00e1df60_TEST PASS\n");
    return 0;
}