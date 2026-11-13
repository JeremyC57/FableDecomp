// Behaviour test for 0x0049F4C0 CWorld::SaveGameStateInternal: a model of the save flow (header section transfers, the body sections,
// the "FableSav" file assembly with the size slot, zero padding to 0x4B000 and the trailing CRC) checked on two worlds.
#include <cstdio>
#include <cstring>

static unsigned char g_buf[0x50000];
struct MemStream {
    unsigned char* data; unsigned long pos, len;
    MemStream(unsigned char* d) : data(d), pos(0), len(0) {}
    void Write(const void* p, unsigned long n) { std::memcpy(data + pos, p, n); pos += n; if (pos > len) len = pos; }
    void WriteULONG(unsigned long v) { Write(&v, 4); }
    void WriteString(const char* s) { Write(s, (unsigned long)std::strlen(s)); }
    void WriteRepeatedByte(unsigned char b, long n) { for (long i = 0; i < n; ++i) Write(&b, 1); }
    void WriteZlibCompressed(const void* p, long n, long) { WriteULONG((unsigned long)n); Write(p, (unsigned long)n); }
    unsigned long GetPosition() const { return pos; }
    void SetPosition(unsigned long p) { pos = p; }
    unsigned long GetLength() const { return len; }
    const void* PeekData() const { return data; }
    void Clear() { pos = len = 0; }
};
static unsigned long Crc(unsigned long seed, const void* p, unsigned long n)
{
    const unsigned char* b = (const unsigned char*)p;
    for (unsigned long i = 0; i < n; ++i) { seed ^= b[i]; for (int k = 0; k < 8; ++k) seed = (seed >> 1) ^ (0xEDB88320UL & (0UL - (seed & 1))); }
    return seed;
}
struct Context {
    MemStream* out; int count; char names[64][40];
    void Init(MemStream& s) { out = &s; }
    void Section(const char* name) { Transfer(name, 0); }
    void Transfer(const char* name, unsigned long v) { std::strcpy(names[count++], name); out->WriteString(name); out->WriteULONG(v); }
};
struct World {
    bool SerialisingHeroState; double StartTime, TimePlayed, Paused; int started, stopped; bool dontStart, dontStop;
    bool Save(MemStream& file, bool b, Context& ctx, unsigned long& total_out)
    {
        if (b && !dontStart) ++started;
        static unsigned char h[0x2000], body[0x2000];
        MemStream save_stream(g_buf), header(h), bodys(body);
        ctx.count = 0; ctx.Init(header);
        ctx.Section("HEADER"); ctx.Transfer("WorldName", 1); ctx.Transfer("WorldFrame", 2);
        if (!SerialisingHeroState) {
            ctx.Transfer("TeleportingEnabled", 1);
            double start_time = StartTime;
            double total = 5.0 - start_time + TimePlayed;   // GFGetTime() == 5.0 in the model
            TimePlayed = total;
            total -= Paused;
            if (total < 0.0) total = 0.0;
            total_out = (unsigned long)total;
            ctx.Transfer("TotalTimePlayed", total_out);
        }
        ctx.Init(bodys);
        ctx.Section("ENTITIES");
        if (!SerialisingHeroState) { ctx.Section("PLAYER"); ctx.Section("QUESTS"); ctx.Section("REGIONS"); ctx.Section("FACTIONS"); }
        {
            unsigned long hl = header.GetLength(), bl = bodys.GetLength();
            save_stream.WriteString("FableSav"); save_stream.WriteULONG(0x2165);
            unsigned long size_position = save_stream.GetPosition();
            save_stream.WriteULONG(0); save_stream.WriteULONG(hl); save_stream.WriteULONG(bl);
            save_stream.WriteZlibCompressed(header.PeekData(), hl, 0x1000);
            save_stream.WriteZlibCompressed(bodys.PeekData(), bl, 0x1000);
            unsigned long data_size = save_stream.GetPosition();
            bodys.Clear();
            save_stream.SetPosition(size_position); save_stream.WriteULONG(data_size); save_stream.SetPosition(data_size);
            if (data_size < 0x4b000) save_stream.WriteRepeatedByte(0, 0x4b000 - data_size);
            unsigned long crc = Crc(0, save_stream.PeekData(), data_size);
            save_stream.SetPosition(data_size); save_stream.Write(&crc, 4);
            file.Write(save_stream.PeekData(), save_stream.GetLength());
        }
        if (b && !dontStop) ++stopped;
        return true;
    }
};
static unsigned char g_file[0x50000];
static int check(World& w, bool b, int expect_names, const char* last)
{
    MemStream file(g_file); Context ctx; unsigned long total = 99;
    if (!w.Save(file, b, ctx, total)) return 0;
    if (ctx.count != expect_names || std::strcmp(ctx.names[ctx.count - 1], last) != 0) return 0;
    if (std::memcmp(g_file, "FableSav", 8) != 0) return 0;
    unsigned long v, size; std::memcpy(&v, g_file + 8, 4); std::memcpy(&size, g_file + 12, 4);
    if (v != 0x2165 || size >= 0x4b000) return 0;
    unsigned long crc; std::memcpy(&crc, g_file + size, 4);
    if (crc != Crc(0, g_file, size)) return 0;
    if (file.GetLength() != 0x4b000 || g_file[0x4b000 - 1] != 0) return 0;
    return 1;
}
int main()
{
    World a; a.SerialisingHeroState = false; a.StartTime = 1.0; a.TimePlayed = 10.0; a.Paused = 4.0; a.started = a.stopped = 0;
    a.dontStart = false; a.dontStop = true;
    World h = a; h.SerialisingHeroState = true; h.dontStart = true; h.dontStop = false;
    if (!check(a, true, 10, "FACTIONS") || a.TimePlayed != 14.0 || a.started != 1 || a.stopped != 0) { std::printf("0049f4c0_TEST FAIL\n"); return 1; }
    if (!check(h, true, 4, "ENTITIES") || h.started != 0 || h.stopped != 1) { std::printf("0049f4c0_TEST FAIL\n"); return 1; }
    World z = a; z.Paused = 100.0; z.TimePlayed = 0.0; MemStream f(g_file); Context c; unsigned long t = 7;
    z.Save(f, false, c, t);
    if (t != 0 || z.started != 1) { std::printf("0049f4c0_TEST FAIL\n"); return 1; }
    std::printf("0049f4c0_TEST PASS\n");
    return 0;
}