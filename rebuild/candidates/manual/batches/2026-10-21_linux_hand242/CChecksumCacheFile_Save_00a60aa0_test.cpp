// Behaviour test for CChecksumCacheFile::Save (0x00A60AA0): only when NeedsSaving; a placeholder header, the
// existing entries (name, date, checksum) in map order, then the header patched to {4, count}; file closed.
#include <cstdio>
#include <cstring>

static unsigned long g_out[64]; static unsigned long g_pos = 0, g_len = 0;
static int g_opens = 0, g_closes = 0;
struct CCharString { char Buf[32]; };
struct CWideString { const char* Text; bool Exists; };
static bool PathExists(const CWideString& p) { return p.Exists; }
struct CDiskFileWin32 {
    CDiskFileWin32() {}
    ~CDiskFileWin32() { Close(); }
    void Open(const CWideString&, int, unsigned long) { ++g_opens; g_pos = g_len = 0; }
    void Close() { ++g_closes; }
};
struct CDateAndTime { unsigned long Stamp; };
struct CFileDataOutputStream {
    CFileDataOutputStream(CDiskFileWin32*) {}
    unsigned long GetPosition() const { return g_pos; }
    void SetPosition(unsigned long p) { g_pos = p; }
    void Put(unsigned long v) { g_out[g_pos++] = v; if (g_pos > g_len) g_len = g_pos; }
    void WriteULONG(unsigned long v) { Put(v); }
    void WriteSLONG(long v) { Put((unsigned long)v); }
    void WriteNullTerminatedString(const CCharString& t) { Put((unsigned long)t.Buf[0]); }
    void WriteDateAndTime(const CDateAndTime& d) { Put(d.Stamp); }
};
struct CEntry { CDateAndTime Date; long Checksum; };
struct SEntry { CWideString first; CEntry second; };
static CCharString Convert(const CWideString& w) { CCharString c; std::strcpy(c.Buf, w.Text); return c; }
struct CChecksumCacheFile {
    CWideString Filename; SEntry* Entries; int Count; bool NeedsSaving;
    void Save();
};

void CChecksumCacheFile::Save()
{
    if (NeedsSaving) {
        CDiskFileWin32 file;
        file.Open(Filename, 4, 2);
        CFileDataOutputStream stream(&file);
        unsigned long start = stream.GetPosition();
        stream.WriteULONG(0);
        stream.WriteULONG(0);
        unsigned long count = 0;
        for (SEntry* it = Entries; it != Entries + Count; ++it) {
            if (PathExists(it->first)) {
                CWideString name(it->first);
                stream.WriteNullTerminatedString(Convert(name));
                stream.WriteDateAndTime(it->second.Date);
                stream.WriteSLONG(it->second.Checksum);
                ++count;
            }
        }
        stream.SetPosition(start);
        stream.WriteULONG(4);
        stream.WriteULONG(count);
        file.Close();
        NeedsSaving = false;
    }
}

int main()
{
    SEntry e[3] = { { { "a", true }, { { 11 }, -5 } }, { { "b", false }, { { 22 }, 6 } }, { { "c", true }, { { 33 }, 7 } } };
    CChecksumCacheFile f; f.Filename.Text = "cache"; f.Entries = e; f.Count = 3; f.NeedsSaving = false;
    f.Save();
    bool ok = g_opens == 0;
    f.NeedsSaving = true; f.Save();
    unsigned long want[8] = { 4, 2, 'a', 11, (unsigned long)-5, 'c', 33, 7 };
    ok = ok && g_opens == 1 && g_closes == 2 && !f.NeedsSaving && g_len == 8 && std::memcmp(g_out, want, sizeof(want)) == 0;
    if (!ok) { std::printf("00a60aa0_TEST FAIL\n"); return 1; }
    std::printf("00a60aa0_TEST PASS\n");
    return 0;
}
