// Behaviour test for 009cd140: src/009cd140.cpp copied verbatim (shared headers expanded inline), callees stubbed below so calls
// and arguments are observable. CBankFile::FindIndexByPathname(const CCharString& pathname, long unused) const
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CBankFile::FindIndexByPathname(const CCharString& pathname, long unused) const -- look an entry up by its full path: split off the file name + extension, get the candidate indices with that file name, and return the first whose stored pathname matches case-insensitively (0 when none)
// Leaf: ?FindIndexByPathname@CBankFile
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009CD140, length 268 bytes (`ret 8`).
// Callees: CCharString() @ 0x0099E4B0 (x2), SplitPath @ 0x009976D0 (__fastcall: path, drive 0, directory 0, &filename,
//          &extension), CCharString::operator+=(const CCharString&) @ 0x0099F0A0, CBankFile::FindIndicesByFilename(const
//          CCharString&, long, bool) @ 0x009CCEF0 (returns CArray<unsigned long> by value), _stricmp @ 0x00BFEBA8,
//          free @ 0x00BFEA14 (the vector's storage), ~CCharString @ 0x0099EAE0.
// Layout (local): the entry-pointer array at +0x2c, each entry's pathname (CCharString*) at +0xc.
// Shape: Ego_r.pdb `?FindIndexByPathname@CBankFile@@QBEKABVCCharString@@J@Z` (const, the unused second parameter is a
// long). FindIndicesByFilename is in retail's TU, so VC7.1 sees that the returned CArray does not escape: its _M_start
// stays in ebp across the _stricmp calls and is reused for the final free, `this` keeps ebx and the count goes to a
// 4th frame word. The same-TU stand-in below builds the result in place from scalars (plain noinline, NOT inside an
// auto_inline(off) region, which would outline the vector destructor it instantiates). The entry's pathname is bound
// to a const reference before the compare, so its loads come before the argument's conversion.
#include <string.h>
extern "C" void __cdecl free(void* block);    // 0x00BFEA14

struct CCharStringData { char* PData; };
struct CCharString {
    CCharStringData* PStringData;
    CCharString();                             // 0x0099E4B0
    ~CCharString();                            // 0x0099EAE0
    CCharString& operator+=(const CCharString& other);   // 0x0099F0A0
    operator const char*() const { return PStringData == 0 ? "" : PStringData->PData; }
};
void __fastcall SplitPath(const CCharString& path, CCharString* drive, CCharString* directory, CCharString* filename, CCharString* extension);   // 0x009976D0

template <class T> struct allocator { allocator() { } };
template <class T> struct _STLP_alloc_proxy : allocator<T> { T* _M_data; };
template <class T> struct _Vector_base {
    T* _M_start;
    T* _M_finish;
    _STLP_alloc_proxy<T> _M_end_of_storage;
    ~_Vector_base() { if (_M_start) free(_M_start); }
};
template <class T> struct CArray : _Vector_base<T> {
    CArray(T* start, T* finish) { this->_M_start = start; this->_M_finish = finish; this->_M_end_of_storage._M_data = finish; }
    unsigned int size() const { return (unsigned int)(this->_M_finish - this->_M_start); }
    T& operator[](unsigned int n) { return *(this->_M_start + n); }
};

struct CBankFileEntry {
    unsigned char _pad[0xc];
    CCharString*  PPathname;                  // +0x0c
};

class CBankFile {
public:
    virtual void v00();
    unsigned char    _pad04[0x28];
    CBankFileEntry** Entries;                 // +0x2c
    CArray<unsigned long> FindIndicesByFilename(const CCharString& filename, long maxCount, bool exact) const;   // 0x009CCEF0
    unsigned long FindIndexByPathname(const CCharString& pathname, long unused) const;
};
unsigned long* FindIndices_009ccef0(const CBankFile* bank, const CCharString& filename, long maxCount, bool exact, unsigned long* count);
// Stand-in for CBankFile::FindIndicesByFilename @ 0x009CCEF0 (same TU in retail): the result is built in place from
// scalars, so the caller's result vector does not escape.
__declspec(noinline) CArray<unsigned long> CBankFile::FindIndicesByFilename(const CCharString& filename, long maxCount, bool exact) const
{
    unsigned long count;
    unsigned long* start = FindIndices_009ccef0(this, filename, maxCount, exact, &count);
    return CArray<unsigned long>(start, start + count);
}

unsigned long CBankFile::FindIndexByPathname(const CCharString& pathname, long) const
{
    CCharString filename;
    CCharString extension;
    SplitPath(pathname, 0, 0, &filename, &extension);
    filename += extension;
    CArray<unsigned long> indices = FindIndicesByFilename(filename, -1, false);
    unsigned long count = indices.size();
    for (unsigned long i = 0; i < count; ++i) {
        unsigned long index = indices[i];
        const CCharString& entryPath = *Entries[index]->PPathname;
        if (_stricmp(entryPath, pathname) == 0)
            return index;
    }
    return 0;
}

// ---- stubs and driver ----
#include <cstdlib>
static int g_live = 0, g_finds = 0; static char g_lastName[64]; static long g_lastMax = 0; static bool g_lastExact = true;
static void SetText(CCharString& s, const char* text, size_t n)
{
    if (!s.PStringData) { s.PStringData = (CCharStringData*)malloc(sizeof(CCharStringData)); s.PStringData->PData = 0; }
    free(s.PStringData->PData);
    s.PStringData->PData = (char*)malloc(n + 1); memcpy(s.PStringData->PData, text, n); s.PStringData->PData[n] = 0;
}
CCharString::CCharString() : PStringData(0) { ++g_live; }
CCharString::~CCharString() { --g_live; if (PStringData) { free(PStringData->PData); free(PStringData); } }
CCharString& CCharString::operator+=(const CCharString& other)
{
    const char* a = *this; const char* b = other;
    char buf[256]; strcpy(buf, a); strcat(buf, b); SetText(*this, buf, strlen(buf)); return *this;
}
void __fastcall SplitPath(const CCharString& path, CCharString* drive, CCharString* directory, CCharString* filename, CCharString* extension)
{
    CHECK(drive == 0 && directory == 0);
    const char* p = path; const char* slash = strrchr(p, '\\'); const char* base = slash ? slash + 1 : p;
    const char* dot = strrchr(base, '.');
    size_t n = dot ? (size_t)(dot - base) : strlen(base);
    SetText(*filename, base, n);
    if (dot) SetText(*extension, dot, strlen(dot));
}
static unsigned long g_result[4]; static unsigned long g_resultCount = 0;
unsigned long* FindIndices_009ccef0(const CBankFile* bank, const CCharString& filename, long maxCount, bool exact, unsigned long* count)
{
    ++g_finds; strcpy(g_lastName, filename); g_lastMax = maxCount; g_lastExact = exact;
    *count = g_resultCount;
    if (g_resultCount == 0) return 0;
    unsigned long* p = (unsigned long*)malloc(g_resultCount * sizeof(unsigned long));
    for (unsigned long i = 0; i < g_resultCount; ++i) p[i] = g_result[i];
    return p;
}
void CBankFile::v00() {}
static CCharString* MakeString(const char* text) { CCharString* s = new CCharString; SetText(*s, text, strlen(text)); return s; }
int main()
{
    CBankFile* bank = new CBankFile;
    CBankFileEntry entries[5]; CBankFileEntry* table[5];
    const char* paths[5] = { "a\\x.wav", "b\\hero.wav", "c\\HERO.WAV", "d\\hero.wav", "e\\y.wav" };
    for (int i = 0; i < 5; ++i) { entries[i].PPathname = MakeString(paths[i]); table[i] = &entries[i]; }
    bank->Entries = table;
    CCharString* query = MakeString("D\\Hero.Wav");
    int base = g_live;
    g_result[0] = 1; g_result[1] = 2; g_result[2] = 3; g_resultCount = 3;
    CHECK(bank->FindIndexByPathname(*query, 0) == 3);
    CHECK(g_finds == 1 && strcmp(g_lastName, "Hero.Wav") == 0 && g_lastMax == -1 && !g_lastExact);
    CHECK(g_live == base);
    CCharString* q2 = MakeString("c\\hero.wav");
    base = g_live;
    CHECK(bank->FindIndexByPathname(*q2, 0) == 2);       // case-insensitive, first match in candidate order
    g_resultCount = 0;
    CHECK(bank->FindIndexByPathname(*q2, 0) == 0);       // no candidates
    g_result[0] = 4; g_resultCount = 1;
    CHECK(bank->FindIndexByPathname(*q2, 0) == 0);       // candidate does not match
    CHECK(g_live == base && g_finds == 4);
    if (fails == 0) printf("009cd140_TEST PASS\n");
    return fails;
}
