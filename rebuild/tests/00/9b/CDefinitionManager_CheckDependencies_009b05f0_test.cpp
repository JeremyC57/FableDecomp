// Behaviour test for CDefinitionManager::CheckDependencies (0x009B05F0): model and body as src/009b05f0.cpp (the inline vector-map
// Add / push_back / Sort, the same-TU insert-grow stand-in and the inline CRC loop included), with the string, path-split, file-scan and
// checksum-file callees given small in-memory bodies and the CRC table built as the standard reflected CRC-32 table. Checks the .cks
// pathname, that the CRC equals an independent reference over the checksums sorted by lower-case name, and that the disabled flag
// leaves the CRC 0 without opening the checksum file.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <new>
#ifndef _MSC_VER
#define __fastcall
#define __forceinline inline
#define __int64 long long
#endif
void* GrowArrayStorage(unsigned long bytes) { return malloc(bytes); }
struct CCharString {
    char m_Text[128];
    CCharString(const char* text) { strcpy(m_Text, text); }
    CCharString(const CCharString& other) { strcpy(m_Text, other.m_Text); }
    ~CCharString() {}
    CCharString& operator=(const CCharString& other) { strcpy(m_Text, other.m_Text); return *this; }
    CCharString ToLower() const
    {
        CCharString r(m_Text);
        for (char* p = r.m_Text; *p; ++p)
            if (*p >= 'A' && *p <= 'Z') *p = (char)(*p - 'A' + 'a');
        return r;
    }
};
struct CWideString {
    char m_Text[128];
    CWideString() { m_Text[0] = 0; }
    CWideString(const char* text) { strcpy(m_Text, text); }
    CWideString(const CWideString& other) { strcpy(m_Text, other.m_Text); }
    ~CWideString() {}
    CWideString& operator=(const CWideString& other) { strcpy(m_Text, other.m_Text); return *this; }
    CCharString ConvertToCharString() const { return CCharString(m_Text); }
};
CWideString __fastcall operator+(const CWideString& a, const CWideString& b)
{
    CWideString r(a);
    strcat(r.m_Text, b.m_Text);
    return r;
}
CWideString __fastcall operator+(const CWideString& a, const wchar_t* b)
{
    CWideString r(a);
    size_t n = strlen(r.m_Text);
    while (*b) r.m_Text[n++] = (char)*b++;
    r.m_Text[n] = 0;
    return r;
}
// "c:\dir\sub\name.ext" -> drive "c:", directory "\dir\sub\", name "name".
void __fastcall SplitPath(const CWideString& path, CWideString* drive, CWideString* directory, CWideString* name, CWideString* extension)
{
    const char* text = path.m_Text;
    const char* rest = text;
    if (text[0] && text[1] == ':') rest = text + 2;
    const char* slash = strrchr(rest, '\\');
    const char* base = slash ? slash + 1 : rest;
    const char* dot = strrchr(base, '.');
    if (!dot) dot = base + strlen(base);
    if (drive) { memcpy(drive->m_Text, text, rest - text); drive->m_Text[rest - text] = 0; }
    if (directory) { memcpy(directory->m_Text, rest, base - rest); directory->m_Text[base - rest] = 0; }
    if (name) { memcpy(name->m_Text, base, dot - base); name->m_Text[dot - base] = 0; }
    if (extension) strcpy(extension->m_Text, dot);
}
struct SFakeFile { const char* Dir; const char* Name; unsigned int Checksum; };
static const SFakeFile g_Files[] = {
    { "defs\\*.def", "Zeta.DEF",  0x11223344u },
    { "defs\\*.def", "alpha.def", 0xdeadbeefu },
    { "defs\\*.def", "Mid.def",   0x00000001u },
    { "more\\*.def", "Beta.def",  0x80000000u },
};
static const int g_FileCount = sizeof(g_Files) / sizeof(g_Files[0]);
struct CFileScanState { long _body[2]; };
static bool ScanFrom(const CWideString& directory, CFileScanState& state, CWideString& fileName)
{
    for (int i = (int)state._body[0]; i < g_FileCount; ++i)
        if (strcmp(g_Files[i].Dir, directory.m_Text) == 0) {
            fileName = CWideString(g_Files[i].Name);
            state._body[0] = i + 1;
            return true;
        }
    state._body[0] = g_FileCount;
    return false;
}
bool __fastcall ScanFirstFile(const CWideString& directory, CFileScanState& state, CWideString& fileName)
{
    state._body[0] = 0;
    return ScanFrom(directory, state, fileName);
}
bool __fastcall ScanNextFile(const CWideString& directory, CFileScanState& state, CWideString& fileName)
{
    return ScanFrom(directory, state, fileName);
}
static char g_OpenedPath[128];
static int g_ChecksumFiles = 0;
class CChecksumFile {
public:
    __int64       Stamp;
    unsigned char _body[0x10];
    CChecksumFile(long bufferSize) : Stamp(bufferSize) { ++g_ChecksumFiles; }
    ~CChecksumFile() { --g_ChecksumFiles; }
    void Open(const CWideString& path) { strcpy(g_OpenedPath, path.m_Text); }
    unsigned long GetChecksum(const CWideString& fileName)
    {
        for (int i = 0; i < g_FileCount; ++i)
            if (strcmp(g_Files[i].Name, fileName.m_Text) == 0)
                return g_Files[i].Checksum;
        return 0;
    }
};
unsigned long g_CRCTable[256];
struct SScalarTag { SScalarTag() { } };
struct CFileChecksum {
    CCharString   first;
    unsigned long second;
    CFileChecksum(const CCharString& name, const unsigned long& checksum) : first(name), second(checksum) {}
};
struct CKeyPairCompareLess { unsigned char _empty; };
void __fastcall SortFileChecksums(CFileChecksum* first, CFileChecksum* last, CKeyPairCompareLess compare)
{
    (void)compare;
    for (CFileChecksum* i = first; i != last; ++i)
        for (CFileChecksum* j = i + 1; j != last; ++j)
            if (strcmp(j->first.m_Text, i->first.m_Text) < 0) {
                CFileChecksum t(*i);
                i->~CFileChecksum(); new (i) CFileChecksum(*j);
                j->~CFileChecksum(); new (j) CFileChecksum(t);
            }
}
struct CFileChecksumMap {                               // CVectorMap<CCharString, unsigned long>
    CFileChecksum*      _M_start;
    CFileChecksum*      _M_finish;
    CFileChecksum*      _M_end_of_storage;
    CKeyPairCompareLess Compare;
    bool                NeedsSort;
    CFileChecksumMap() : _M_start(0), _M_finish(0), _M_end_of_storage(0), NeedsSort(false) {}
    ~CFileChecksumMap()
    {
        for (CFileChecksum* p = _M_start; p != _M_finish; ++p)
            p->~CFileChecksum();
        if (_M_start)
            free(_M_start);
    }
    unsigned long size() const { return (unsigned long)(_M_finish - _M_start); }
    CFileChecksum& operator[](unsigned long n) { return *(_M_start + n); }
    void InsertGrow(CFileChecksum* where, const CFileChecksum& value, const SScalarTag& tag, unsigned long count, bool atEnd);   // 0x009B71B0
    __forceinline void push_back(const CFileChecksum& value)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) CFileChecksum(value);
            ++_M_finish;
        }
        else
            InsertGrow(_M_finish, value, SScalarTag(), 1, true);
    }
    __forceinline void Add(const CCharString& key, const unsigned long& value)
    {
        push_back(CFileChecksum(key, value));
        NeedsSort = true;
    }
    void Sort()
    {
        if (NeedsSort) {
            SortFileChecksums(_M_start, _M_finish, Compare);
            NeedsSort = false;
        }
    }
};
void CFileChecksumMap::InsertGrow(CFileChecksum* where, const CFileChecksum& value, const SScalarTag& tag, unsigned long count, bool atEnd)
{
    unsigned long oldSize = size();
    unsigned long newCapacity = oldSize + (oldSize > count ? oldSize : count);
    CFileChecksum* p = (CFileChecksum*)GrowArrayStorage(newCapacity * sizeof(CFileChecksum));
    CFileChecksum* out = p;
    CFileChecksum* it;
    for (it = _M_start; it != where; ++it, ++out)
        new (out) CFileChecksum(*it);
    for (unsigned long i = 0; i < count; ++i, ++out)
        new (out) CFileChecksum(value);
    for (it = _M_start; it != _M_finish; ++it)
        it->~CFileChecksum();
    if (_M_start)
        free(_M_start);
    _M_start = p;
    _M_finish = out;
    _M_end_of_storage = p + newCapacity;
}
inline unsigned long UpdateCRC(unsigned long crc, const void* data, unsigned long length)
{
    const unsigned char* bytes = (const unsigned char*)data;
    for (unsigned long i = 0; i < length; ++i)
        crc = (crc >> 8) ^ g_CRCTable[bytes[i] ^ (crc & 0xff)];
    return crc;
}
struct CWideStringVector {
    CWideString* _M_start;
    CWideString* _M_finish;
    CWideString* _M_end_of_storage;
    const CWideString* begin() const { return _M_start; }
    const CWideString* end() const { return _M_finish; }
};
bool g_bCheckDefDependencies = true;

class CDefinitionManager {
public:
    unsigned char _pad000[0xb8];
    CWideString   CompiledFileName;
    unsigned long DependencyCheckCRC;
    void CheckDependencies(const CWideStringVector& paths);
};

void CDefinitionManager::CheckDependencies(const CWideStringVector& paths)
{
    DependencyCheckCRC = 0;
    if (!g_bCheckDefDependencies)
        return;
    CWideString drive;
    CWideString directory;
    CWideString name;
    SplitPath(CompiledFileName, &drive, &directory, &name, 0);
    CWideString checksumPath = drive + directory + name + L".cks";
    CChecksumFile checksums(0x10000);
    checksums.Open(checksumPath);
    CFileChecksumMap files;
    for (const CWideString* path = paths.begin(); path != paths.end(); ++path) {
        CWideString fileName;
        CFileScanState scan;
        if (ScanFirstFile(*path, scan, fileName)) {
            do {
                files.Add(fileName.ConvertToCharString().ToLower(), checksums.GetChecksum(fileName));
            } while (ScanNextFile(*path, scan, fileName));
        }
    }
    files.Sort();
    DependencyCheckCRC = 0;
    for (unsigned long i = 0; i < files.size(); ++i) {
        unsigned long checksum = files[i].second;
        DependencyCheckCRC = UpdateCRC(DependencyCheckCRC, &checksum, 4);
    }
}

// Independent reference: plain CRC-32 table walk (no pre/post inversion) over each checksum's four little-endian bytes.
static unsigned int ReferenceCRC(const unsigned int* values, int count)
{
    unsigned int crc = 0;
    for (int i = 0; i < count; ++i)
        for (int b = 0; b < 4; ++b)
            crc = (crc >> 8) ^ (unsigned int)g_CRCTable[((values[i] >> (8 * b)) ^ crc) & 0xff];
    return crc;
}

int main()
{
    int fails = 0;
    for (unsigned int n = 0; n < 256; ++n) {
        unsigned int c = n;
        for (int k = 0; k < 8; ++k)
            c = (c & 1) ? 0xedb88320u ^ (c >> 1) : c >> 1;
        g_CRCTable[n] = c;
    }
    if (g_CRCTable[1] != 0x77073096u) { printf("bad table\n"); ++fails; }

    CWideString dirs[3] = { CWideString("defs\\*.def"), CWideString("none\\*.def"), CWideString("more\\*.def") };
    CWideStringVector paths;
    paths._M_start = dirs;
    paths._M_finish = dirs + 3;
    paths._M_end_of_storage = dirs + 3;

    CDefinitionManager manager;
    manager.CompiledFileName = CWideString("c:\\data\\compiledefs\\game.bin");
    manager.DependencyCheckCRC = 0x12345678;
    manager.CheckDependencies(paths);
    if (strcmp(g_OpenedPath, "c:\\data\\compiledefs\\game.cks") != 0) { printf("opened [%s]\n", g_OpenedPath); ++fails; }
    // Sorted by lower-case name: alpha.def, beta.def, mid.def, zeta.def.
    unsigned int sorted[4] = { 0xdeadbeefu, 0x80000000u, 0x00000001u, 0x11223344u };
    unsigned int expected = ReferenceCRC(sorted, 4);
    if ((unsigned int)manager.DependencyCheckCRC != expected) {
        printf("crc %08x expected %08x\n", (unsigned int)manager.DependencyCheckCRC, expected); ++fails;
    }
    unsigned int unsorted[4] = { 0x11223344u, 0xdeadbeefu, 0x00000001u, 0x80000000u };
    if (ReferenceCRC(unsorted, 4) == expected) { printf("order-insensitive reference\n"); ++fails; }
    if (g_ChecksumFiles != 0) { printf("checksum file leaked\n"); ++fails; }

    // Dependency checking off: CRC cleared, checksum file never opened.
    g_bCheckDefDependencies = false;
    g_OpenedPath[0] = 0;
    manager.DependencyCheckCRC = 0x55;
    manager.CheckDependencies(paths);
    if (manager.DependencyCheckCRC != 0 || g_OpenedPath[0] != 0) { printf("disabled: crc %lx opened [%s]\n", (unsigned long)manager.DependencyCheckCRC, g_OpenedPath); ++fails; }

    // No matching files: CRC stays 0.
    g_bCheckDefDependencies = true;
    paths._M_start = dirs + 1;
    paths._M_finish = dirs + 2;
    manager.DependencyCheckCRC = 0x55;
    manager.CheckDependencies(paths);
    if (manager.DependencyCheckCRC != 0) { printf("no files: crc %lx\n", (unsigned long)manager.DependencyCheckCRC); ++fails; }

    if (fails == 0)
        printf("009b05f0_TEST PASS\n");
    return fails;
}