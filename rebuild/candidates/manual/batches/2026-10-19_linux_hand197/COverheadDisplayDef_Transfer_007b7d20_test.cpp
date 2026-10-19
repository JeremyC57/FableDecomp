#include <cstdio>
#include <cstring>
// Behaviour test for 007b7d20 COverheadDisplayDef::Transfer: three CRC checks and, by mode, the three flags written in order
// (save) or read in order (load); other modes only check.
static char g_log[16]; static int g_n = 0, g_crc = 0, g_inPos = 0; static bool g_in[3] = { true, false, true };
struct COverheadDisplayDef {
    void* vtbl;
    unsigned char pad[0x21];
    bool DisplayDamage, DisplayRepair, DisplayRenown;
};
struct CMemoryDataInputStream;
struct CDataOutputStream {
    void WriteBool(bool value) { g_log[g_n++] = value ? 'T' : 'F'; }
};
template<class T> struct CPersistTraits;
template<> struct CPersistTraits<bool> {
    CPersistTraits() {}
    void TransferBinaryIn(CMemoryDataInputStream* stream, bool* value) { *value = g_in[g_inPos++]; }
    static void TransferBinaryOut(CDataOutputStream* stream, const bool& value) { stream->WriteBool(value); }
};
struct CPersistContext {
    unsigned char           _pad00[0x18];
    long                    m_mode;                     // +0x18 (2 = load, 3 = save)
    unsigned char           _pad1c[0x08];
    CMemoryDataInputStream* m_pLoadStream;              // +0x24
    CDataOutputStream*      m_pSaveStream;              // +0x28
    void CheckCRC(const char* name) { if (name[0] == 0) ++g_crc; }
    template<class T> void Transfer(const char* name, T& value)
    {
        CheckCRC(name);
        switch (m_mode) {
        case 2:
            CPersistTraits<T>().TransferBinaryIn(m_pLoadStream, &value);
            break;
        case 3:
            CPersistTraits<T>::TransferBinaryOut(m_pSaveStream, value);
            break;
        }
    }
};
struct COverheadDisplayDef_Transfer : COverheadDisplayDef {
    void Transfer(CPersistContext& context);
};

void COverheadDisplayDef_Transfer::Transfer(CPersistContext& context)
{
    context.Transfer("", DisplayDamage);
    context.Transfer("", DisplayRepair);
    context.Transfer("", DisplayRenown);
}

int main()
{
    static COverheadDisplayDef_Transfer def;
    static CPersistContext ctx;
    def.DisplayDamage = true; def.DisplayRepair = false; def.DisplayRenown = true;
    ctx.m_mode = 3;
    def.Transfer(ctx);
    g_log[g_n] = 0;
    bool ok = std::strcmp(g_log, "TFT") == 0 && g_crc == 3;
    def.DisplayDamage = false; def.DisplayRepair = true; def.DisplayRenown = false;
    ctx.m_mode = 2;
    def.Transfer(ctx);
    ok = ok && def.DisplayDamage && !def.DisplayRepair && def.DisplayRenown && g_crc == 6 && g_inPos == 3;
    ctx.m_mode = 1; g_n = 0;
    def.Transfer(ctx);
    ok = ok && g_n == 0 && g_inPos == 3 && g_crc == 9;
    if (!ok) { std::printf("007b7d20_TEST FAIL\n"); return 1; }
    std::printf("007b7d20_TEST PASS\n");
    return 0;
}
