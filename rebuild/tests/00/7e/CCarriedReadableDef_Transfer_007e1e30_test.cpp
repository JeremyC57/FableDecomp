#include <cstdio>
struct CMemoryDataInputStream { long values[4]; int pos; };
struct CDataOutputStream { long written[4]; int n; void WriteLong(long v) { written[n++] = v; } };
struct CDefString { long TablePos; void ReadBinary(CMemoryDataInputStream* s) { TablePos = s->values[s->pos++]; } };
static int g_crc = 0;
struct CPersistContext {
    long Mode; CMemoryDataInputStream* LoadStream; CDataOutputStream* SaveStream;
    void CheckCRC(const char*) { ++g_crc; }
    void Transfer(CDefString& value)
    {
        CheckCRC("");
        switch (Mode) {
        case 2: value.ReadBinary(LoadStream); break;
        case 3: SaveStream->WriteLong(value.TablePos); break;
        }
    }
};
struct CCarriedReadableDef {
    long TitleTextTag_TablePos, BodyTextTag_TablePos;
    void Transfer(CPersistContext& context)
    {
        context.Transfer(*reinterpret_cast<CDefString*>(&TitleTextTag_TablePos));
        context.Transfer(*reinterpret_cast<CDefString*>(&BodyTextTag_TablePos));
    }
};
int main()
{
    CMemoryDataInputStream in; in.values[0] = 7; in.values[1] = 9; in.pos = 0;
    CDataOutputStream out; out.n = 0;
    CPersistContext ctx; ctx.LoadStream = &in; ctx.SaveStream = &out;
    CCarriedReadableDef d; d.TitleTextTag_TablePos = 0; d.BodyTextTag_TablePos = 0;
    ctx.Mode = 2; d.Transfer(ctx);
    if (d.TitleTextTag_TablePos != 7 || d.BodyTextTag_TablePos != 9 || g_crc != 2) { std::printf("007e1e30_TEST FAIL\n"); return 1; }
    ctx.Mode = 3; d.Transfer(ctx);
    if (out.n != 2 || out.written[0] != 7 || out.written[1] != 9 || g_crc != 4) { std::printf("007e1e30_TEST FAIL\n"); return 1; }
    ctx.Mode = 1; d.Transfer(ctx);
    if (out.n != 2 || in.pos != 2 || g_crc != 6) { std::printf("007e1e30_TEST FAIL\n"); return 1; }
    std::printf("007e1e30_TEST PASS\n");
    return 0;
}