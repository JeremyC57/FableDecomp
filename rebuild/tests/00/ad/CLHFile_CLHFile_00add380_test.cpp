// Test for CLHFile::CLHFile (0x00ADD380): the constructor zeroes +0xC, creates the threaded file, sets m_bIsWMA when the
// file name's extension (SplitPath: from the last '.' after the last separator, dot included), upper-cased, equals ".WMA",
// and opens the file read-only under the same name.
#include <cstdio>
typedef char Ch;
struct File { bool opened; bool readOnly; Ch name[64]; };
struct CLHFile { File* m_pFile; unsigned long m_unknown0C; bool m_bIsWMA; };
static void SplitExt(const Ch* path, Ch* ext)
{
    int last = -1, i = 0;
    for (; path[i]; ++i) {
        if (path[i] == '.') last = i;
        else if (path[i] == '\\' || path[i] == '/' || path[i] == ':') last = -1;
    }
    int n = 0;
    if (last >= 0) for (int j = last; path[j]; ++j) ext[n++] = path[j];
    ext[n] = 0;
}
static bool EqualsWMA(const Ch* ext)
{
    const char* w = ".WMA"; int i = 0;
    for (; ext[i] && w[i]; ++i) { Ch c = ext[i]; if (c < 0x80 && c >= 'a' && c <= 'z') c = (Ch)(c - 32); if (c != (Ch)w[i]) return false; }
    return ext[i] == 0 && w[i] == 0;
}
static void Construct(CLHFile* f, File* storage, const Ch* fileName)
{
    f->m_unknown0C = 0;
    f->m_pFile = storage; storage->opened = false;
    f->m_bIsWMA = false;
    Ch ext[64]; SplitExt(fileName, ext);
    if (EqualsWMA(ext)) f->m_bIsWMA = true;
    int i = 0; for (; fileName[i]; ++i) storage->name[i] = fileName[i]; storage->name[i] = 0;
    storage->opened = true; storage->readOnly = true;
}
static void Widen(const char* s, Ch* d) { int i = 0; for (; s[i]; ++i) d[i] = (Ch)s[i]; d[i] = 0; }
int main()
{
    struct { const char* name; bool wma; } cases[] = { {"music\\track.wma", true}, {"a.WmA", true}, {"speech.wav", false},
        {"dir.wma\\file", false}, {"noext", false}, {"x.wmav", false} };
    for (int k = 0; k < 6; ++k) {
        Ch name[64]; Widen(cases[k].name, name);
        CLHFile f; File storage; f.m_unknown0C = 7;
        Construct(&f, &storage, name);
        if (f.m_bIsWMA != cases[k].wma || f.m_unknown0C != 0 || !storage.opened || !storage.readOnly || storage.name[0] != name[0]) {
            std::printf("00add380_TEST FAIL\n"); return 1;
        }
    }
    std::printf("00add380_TEST PASS\n");
    return 0;
}