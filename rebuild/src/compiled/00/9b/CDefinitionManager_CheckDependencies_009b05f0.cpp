// CDefinitionManager::CheckDependencies(const CWideStringVector&) -- when dependency checking is on, open the <compiled file>.cks checksum file beside the compiled definitions, look up the stored checksum of every source file the path list matches (keyed by lower-cased name), sort by name and CRC the checksums into DependencyCheckCRC (left 0 when checking is off)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009B05F0, length 705 bytes (`ret 4`).
// Callees: CWideString() 0x0099AED0 / ~CWideString 0x0099B510 / ConvertToCharString 0x0099B2C0 / operator+ 0x0099BE70 / operator+(const wchar_t*)
//   0x0099BF30 (__fastcall), SplitPath 0x00997620 (__fastcall), CCharString copy ctor 0x0099EC30 / ~CCharString 0x0099EAE0 / ToLower 0x0099ED10,
//   checksum file ctor 0x00A60C90 / dtor 0x00A60CE0 / Open 0x00A60D40 / GetChecksum 0x00A60FB0, ScanFirstFile 0x00999C50 / ScanNextFile
//   0x00999CB0 (__fastcall), CVectorMap insert-grow 0x009B71B0 (same-TU stand-in body below), sort 0x009B8EF0 (__fastcall), g_CRCTable
//   0x0129A168, g_bCheckDefDependencies 0x0138E189, CRT free 0x00BFEA14.
// Layout: retail CDefinitionManager (PDB minus 0x10 from the 12-byte vectors ahead): CompiledFileName +0xb8, DependencyCheckCRC +0xbc.
//   The checksum file carries an __int64 (aligned `and esp,-8` frame). Modelled locally.
// Shape: the vector-map Add (push_back + NeedsSort) and push_back are __forceinline, otherwise Add goes out of line; the insert-grow is a
//   same-TU noinline stand-in that reads Start/Finish before writing. The CRC is the table loop crc = (crc >> 8) ^ table[byte ^ (crc & 0xff)]
//   inlined over a local copy of each checksum.
// Leaf: CheckDependencies
#include <new>
extern "C" void __cdecl free(void* block);                     // 0x00BFEA14
void* GrowArrayStorage(unsigned long bytes);
struct CCharString {
    void* m_pData;
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
    CCharString ToLower() const;                        // 0x0099ED10
};
struct CWideString {
    void* PStringData;
    CWideString();                                      // 0x0099AED0
    ~CWideString();                                     // 0x0099B510
    CCharString ConvertToCharString() const;            // 0x0099B2C0
};
CWideString __fastcall operator+(const CWideString& a, const CWideString& b);   // 0x0099BE70
CWideString __fastcall operator+(const CWideString& a, const wchar_t* b);       // 0x0099BF30
void __fastcall SplitPath(const CWideString& path, CWideString* drive, CWideString* directory, CWideString* name, CWideString* extension);   // 0x00997620
struct CFileScanState { long _body[2]; };
bool __fastcall ScanFirstFile(const CWideString& directory, CFileScanState& state, CWideString& fileName);   // 0x00999C50
bool __fastcall ScanNextFile(const CWideString& directory, CFileScanState& state, CWideString& fileName);    // 0x00999CB0
class CChecksumFile {
public:
    __int64       Stamp;
    unsigned char _body[0x10];
    CChecksumFile(long bufferSize);                     // 0x00A60C90
    ~CChecksumFile();                                   // 0x00A60CE0
    void Open(const CWideString& path);                 // 0x00A60D40
    unsigned long GetChecksum(const CWideString& fileName);   // 0x00A60FB0
};
extern const unsigned long g_CRCTable[256];             // 0x0129A168
struct SScalarTag { SScalarTag() { } };
struct CFileChecksum {
    CCharString   first;
    unsigned long second;
    CFileChecksum(const CCharString& name, const unsigned long& checksum) : first(name), second(checksum) {}
};
struct CKeyPairCompareLess { unsigned char _empty; };
void __fastcall SortFileChecksums(CFileChecksum* first, CFileChecksum* last, CKeyPairCompareLess compare);   // 0x009B8EF0
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
#pragma auto_inline(off)
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
#pragma auto_inline(on)
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
extern bool g_bCheckDefDependencies;                    // 0x0138E189

class CDefinitionManager {
public:
    unsigned char _pad000[0xb8];
    CWideString   CompiledFileName;                     // +0xb8
    unsigned long DependencyCheckCRC;                   // +0xbc
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