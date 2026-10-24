// Behaviour test for 0x0045C8D3 CPersistContext::Transfer<CSpecialEffectsStringMap>: CRC check first, a default traits map
// alive across the switch, mode 2 reads the containment map from the load stream, mode 3 writes it, other modes do nothing
#include <cstdio>
#include <cstring>
static char g_log[64];
static void Log(const char* s) { std::strcat(g_log, s); }
struct CDataOutputStream { int id; };
struct CMemoryDataInputStream { int id; };
struct CULongLongMap { int entries; CULongLongMap() : entries(0) { } ~CULongLongMap() { Log("D"); } };
static void GFSerialiseMapBinaryOut_ULongLong(CDataOutputStream* s, const CULongLongMap* m) { if (s->id == 7 && m->entries == 3) Log("O"); }
static void GFSerialiseMapBinaryIn_ULongLong(CMemoryDataInputStream* s, CULongLongMap* m) { if (s->id == 6) { m->entries = 9; Log("I"); } }
struct CSpecialEffectsStringMap { CULongLongMap ContainmentMap; CSpecialEffectsStringMap() { Log("C"); } };
template <class T> struct CPersistTraits;
template <> struct CPersistTraits<CSpecialEffectsStringMap> {
    CSpecialEffectsStringMap DefaultVal;
    void TransferBinaryOut(CDataOutputStream* s, const CSpecialEffectsStringMap* v) { GFSerialiseMapBinaryOut_ULongLong(s, &v->ContainmentMap); }
    void TransferBinaryIn(CMemoryDataInputStream* s, CSpecialEffectsStringMap* v) { GFSerialiseMapBinaryIn_ULongLong(s, &v->ContainmentMap); }
};
struct CPersistContext {
    long Mode; CMemoryDataInputStream* PLoadStream; CDataOutputStream* PSaveStream;
    void CheckCRC(const char* name) { if (name[0] == 0) Log("K"); }
    template <class T> void Transfer(T& value)
    {
        CheckCRC("");
        CPersistTraits<T> traits;
        switch (Mode) {
        case 2: traits.TransferBinaryIn(PLoadStream, &value); break;
        case 3: traits.TransferBinaryOut(PSaveStream, &value); break;
        }
    }
};
int main()
{
    CMemoryDataInputStream in = { 6 };
    CDataOutputStream out = { 7 };
    CPersistContext ctx; ctx.PLoadStream = &in; ctx.PSaveStream = &out;
    CSpecialEffectsStringMap value; value.ContainmentMap.entries = 3;
    bool ok = true;
    g_log[0] = 0; ctx.Mode = 3; ctx.Transfer(value); ok = ok && std::strcmp(g_log, "KCOD") == 0;
    g_log[0] = 0; ctx.Mode = 2; ctx.Transfer(value); ok = ok && std::strcmp(g_log, "KCID") == 0 && value.ContainmentMap.entries == 9;
    g_log[0] = 0; ctx.Mode = 4; ctx.Transfer(value); ok = ok && std::strcmp(g_log, "KCD") == 0;
    if (!ok) { std::printf("0045c8d3_TEST FAIL\n"); return 1; }
    std::printf("0045c8d3_TEST PASS\n");
    return 0;
}