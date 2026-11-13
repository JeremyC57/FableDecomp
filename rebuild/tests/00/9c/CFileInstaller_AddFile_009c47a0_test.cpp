// Behaviour test for 0x009C47A0 CFileInstaller::AddFile(const CCharString&, const CCharString&, const CArray<CCharString>*, bool):
// a host model of the row's logic over a small fake file system (full paths, upper-cased filter set, wildcard scans,
// directory / explicit destinations, compressed-name mapping, size accounting, the Completed flag and the wake-up event).
#include <cstdio>
#include <cstring>
#include <cctype>

static const int MAXS = 128;
struct Str { char s[MAXS]; };
static Str MakeStr(const char* t) { Str r; std::strncpy(r.s, t, MAXS - 1); r.s[MAXS - 1] = 0; return r; }
static Str Cat(const Str& a, const Str& b) { Str r = a; std::strncat(r.s, b.s, MAXS - 1 - std::strlen(r.s)); return r; }
static Str Upper(const Str& a) { Str r = a; for (char* p = r.s; *p; ++p) if ((unsigned char)*p < 0x80) *p = (char)std::toupper(*p); return r; }
static long FindChar(const Str& a, char c) { const char* p = std::strchr(a.s, c); return p ? (long)(p - a.s) : -1; }
static bool Eq(const Str& a, const Str& b) { return std::strcmp(a.s, b.s) == 0; }

// fake disk: full paths and lengths
struct FakeFile { const char* path; long length; };
static FakeFile g_Disk[] = {
    { "C:\\SRC\\A.BIG", 100 }, { "C:\\SRC\\B.TXT", 20 }, { "C:\\SRC\\C.BIZ", 7 }, { "C:\\SRC\\D.TXT", 3 },
    { "C:\\SKIP\\B.TXT", 0 }, { "C:\\ONE\\X.BIZ", 50 },
};
static const int DISK = sizeof(g_Disk) / sizeof(g_Disk[0]);

// CAFile::ConvertPathToFullPath: relative paths are under C:\ .
static Str FullPath(const Str& p) { if (p.s[0] && p.s[1] == ':') return p; return Cat(MakeStr("C:\\"), p); }
// CAFile::GetFilenameFromPathname: the part after the last backslash.
static Str FilenameOf(const Str& p) { const char* q = std::strrchr(p.s, '\\'); return MakeStr(q ? q + 1 : p.s); }
static Str DirOf(const Str& p) { Str r = p; char* q = std::strrchr(r.s, '\\'); if (q) q[1] = 0; else r.s[0] = 0; return r; }
static bool Match(const char* pat, const char* s)
{
    if (*pat == 0) return *s == 0;
    if (*pat == '*') { for (;; ++s) { if (Match(pat + 1, s)) return true; if (*s == 0) return false; } }
    return std::toupper(*pat) == std::toupper(*s) && Match(pat + 1, s + 1);
}
// CAFile::StartFileScan / ScanNextFile over the fake disk: full paths of the matches.
struct ScanInfo { int next; };
static bool ScanNext(const Str& pattern, ScanInfo& info, Str& out)
{
    for (; info.next < DISK; ++info.next)
        if (Match(pattern.s, g_Disk[info.next].path)) { out = MakeStr(g_Disk[info.next].path); ++info.next; return true; }
    return false;
}
static bool ScanFirst(const Str& pattern, ScanInfo& info, Str& out) { info.next = 0; return ScanNext(pattern, info, out); }
static long LengthOf(const Str& p) { for (int i = 0; i < DISK; ++i) if (Eq(MakeStr(g_Disk[i].path), p)) return g_Disk[i].length; return 0; }

struct FileInfo { Str src, dst; long size; bool started, decompress, marker; };
struct Installer {
    FileInfo files[32]; int nfiles; long total; bool completed; int events;
    // CFileInstaller::IsCompressedFilename: ".BIZ" names install as ".BIG"
    bool IsCompressed(const Str& name, Str& out) const
    {
        Str base = FilenameOf(name); size_t n = std::strlen(base.s);
        if (n > 4 && std::strcmp(Upper(base).s + n - 4, ".BIZ") == 0) { base.s[n - 4] = 0; out = Cat(base, MakeStr(".big")); return true; }
        out = base; return false;
    }
    void Push(const Str& s, const Str& d, long size, bool dec, bool marker)
    {
        FileInfo& f = files[nfiles++]; f.src = s; f.dst = d; f.size = size; f.started = false; f.decompress = dec; f.marker = marker;
    }
    void AddFile(const Str& source, const Str& destination, const Str* filters, int nfilters, bool marker)
    {
        Str src = FullPath(source), dst = FullPath(destination);
        Str filtered[32]; int nf = 0;
        if (filters && nfilters) {
            for (int i = 0; i < nfilters; ++i) {
                Str f = Upper(filters[i]);
                if (FindChar(f, '*') >= 0) {
                    Str name; ScanInfo info;
                    if (ScanFirst(f, info, name)) do { filtered[nf++] = Upper(name); } while (ScanNext(f, info, name));
                } else filtered[nf++] = Upper(FullPath(f));
            }
        }
        if (FindChar(src, '*') >= 0) {
            Str name; ScanInfo info;
            if (ScanFirst(src, info, name)) do {
                bool skip = false; for (int i = 0; i < nf; ++i) if (Eq(filtered[i], Upper(name))) skip = true;
                if (!skip) {
                    Str out; bool dec = IsCompressed(name, out);
                    long size = LengthOf(name); total += size;
                    Push(name, Cat(dst, out), size, dec, marker);
                }
            } while (ScanNext(src, info, name));
        } else {
            Str fname = FilenameOf(dst);
            bool skip = false; for (int i = 0; i < nf; ++i) if (Eq(filtered[i], Upper(src))) skip = true;
            Str out; bool dec = IsCompressed(src, out);
            long size = LengthOf(src); total += size;           // the length is counted even when the file is filtered
            if (!skip) Push(src, fname.s[0] == 0 ? Cat(dst, out) : dst, size, dec, marker);
        }
        if (nfiles != 0) completed = false;
        ++events;
    }
};

int main()
{
    static Installer inst; inst.nfiles = 0; inst.total = 0; inst.completed = true; inst.events = 0;
    bool ok = true;
    // 1. wildcard source with one plain filter and one wildcard filter
    Str filters[2] = { MakeStr("src\\b.txt"), MakeStr("c:\\src\\d*") };
    inst.AddFile(MakeStr("src\\*"), MakeStr("dest\\"), filters, 2, true);
    ok = ok && inst.nfiles == 2 && inst.total == 107 && !inst.completed && inst.events == 1;
    ok = ok && Eq(inst.files[0].dst, MakeStr("C:\\dest\\A.BIG")) && !inst.files[0].decompress && inst.files[0].marker;
    ok = ok && Eq(inst.files[1].dst, MakeStr("C:\\dest\\C.big")) && inst.files[1].decompress && inst.files[1].size == 7;
    // 2. single file into a directory (empty destination file name), compressed
    inst.AddFile(MakeStr("C:\\ONE\\X.BIZ"), MakeStr("D:\\OUT\\"), 0, 0, false);
    ok = ok && inst.nfiles == 3 && inst.total == 157 && Eq(inst.files[2].dst, MakeStr("D:\\OUT\\X.big")) && inst.files[2].decompress;
    // 3. single file to an explicit path keeps the destination; the compressed flag is still reported
    inst.AddFile(MakeStr("C:\\ONE\\X.BIZ"), MakeStr("D:\\OUT\\Y.DAT"), 0, 0, true);
    ok = ok && inst.nfiles == 4 && Eq(inst.files[3].dst, MakeStr("D:\\OUT\\Y.DAT")) && inst.files[3].decompress && !inst.files[3].started;
    // 4. a filtered single file is not queued but its length is still added
    Str f2[1] = { MakeStr("C:\\SRC\\A.BIG") };
    inst.AddFile(MakeStr("C:\\SRC\\A.BIG"), MakeStr("D:\\X.BIG"), f2, 1, false);
    ok = ok && inst.nfiles == 4 && inst.total == 307 && inst.events == 4;
    (void)DirOf;
    if (!ok) { std::printf("009c47a0_TEST FAIL\n"); return 1; }
    std::printf("009c47a0_TEST PASS\n");
    return 0;
}