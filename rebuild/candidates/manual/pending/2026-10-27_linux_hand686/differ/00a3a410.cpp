// CWADBank::AddFile(const CWideString& fileName) -- add (or refresh) the WAD bank entry for a file: a new entry is created when the symbol is unknown; an existing one is rewritten only when the size, the last-write time or the bytes changed
// Grade: DIFFER(918v920) -- frame-slot order only: retail lays out fileData/if-block temporaries before fullPath and the empty filename before the file object (ours: fullPath first, filename after file); file.Length is loaded into edi before GetEntryDataSize in retail; vector::resize needs __forceinline here (VC declines to expand it at two sites with the shared CCountedPointer header, 857v920 without it). Next step: slot-order grid search over the declaration/scoping of fullPath, filename and the two CByteVectors (R&D rule: refs descending, sibling scopes overlay), then drop the __forceinline once the layout matches.
// Leaf: ?AddFile@CWADBank@@QAEXABVCWideString@@@Z
// Identity (proven): Ego_r.pdb ?AddFile@CWADBank@@QAEXABVCWideString@@@Z (the only CWideString-taking member of the WAD bank);
// the state block it creates is CWADBankStateBlock (vtable 0x0129CFD0, RTTI); the only caller 0x00502170 (landed
// CWorldMap::CreateWorldLevelWad) calls it on its local CWADBank for each level/thing file, binding this address as an invented
// CBankFileAsync::UpdateEntryFromFile (NAME_FIXES). The manifest's "CreateWorldLevelWad" label is propagated.
// Callees: CWideString::ConvertToCharString 0x0099B2C0, CBankFile::FindIndexBySymbol 0x009CCDC0, ~CCharString 0x0099EAE0,
// operator new 0x00BFEA1A, CCountedPointer<CBankStateBlock>(CBankStateBlock*) 0x00A3A380, CCharString(const char*, long) 0x0099EBF0,
// CBankFile::AddEntry 0x009CD620, operator delete 0x00BFE9BC, CAFile::ConvertPathToFullPath 0x00999110, CDiskFileWin32 ctor 0x0099AD80,
// CBankFile::GetEntryDataSize 0x009CC240, CDateAndTime() 0x009E9AB0, CBankFileEntryUpdateData::ReadInfo 0x009CBE90,
// CDiskFileWin32::GetFileInformation 0x0099A5A0, CDateAndTime::operator== 0x009E9B20, vector<char>::_M_fill_insert 0x004110D0,
// CBankFile::ReadEntryData 0x009CC2A0, CDiskFileWin32::Read 0x0099A960 / SetPosition 0x0099ABF0, free 0x00BFEA14,
// CFileDataInputStream ctor 0x00994700 / dtor 0x00994780, CMemoryDataOutputStream ctor 0x00996AF0 / Write 0x00996CC0 /
// GetLength 0x00994B40 / dtor 0x00996B30, CMemoryDataInputStream(const CMemoryDataOutputStream&) 0x00995EE0, ~CDataStream 0x00993B10,
// CBankFile::UpdateEntry 0x009D1320, CDiskFileWin32::Close 0x0099A920, ~CWideString 0x0099B510, ~CBaseClassNonCopyable 0x0099A300.
// Models: the landed CBankFile (retail layout, 00/9c), CDiskFileWin32 / CFileInformation (00/99), CCountedPointer (00/a3).
#include <string.h>
#pragma intrinsic(memcmp)
extern "C" void* __cdecl malloc(unsigned int size);                     // 0x00BFEA0E
extern "C" void __cdecl free(void* block);                              // 0x00BFEA14
void* __cdecl operator new(unsigned int size);                          // 0x00BFEA1A
void __cdecl operator delete(void* block);                              // 0x00BFE9BC
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);                    // 0x0099EBF0
    ~CCharString();                                                     // 0x0099EAE0
};
class CWideString { public:
    void* PStringData;
    CCharString ConvertToCharString() const;                            // 0x0099B2C0
    ~CWideString();                                                     // 0x0099B510
};
class CBaseClassNonCopyable { public:
    virtual ~CBaseClassNonCopyable();                                   // 0x0099A300
};
struct _FILETIME;
class CDateAndTime { public:
    long Year;
    unsigned long Month, Day, Hour, Minute, Second, Millisecond;
    CDateAndTime();                                                     // 0x009E9AB0
    bool operator==(const CDateAndTime& other) const;                   // 0x009E9B20
};
class CFileInformation { public:
    CDateAndTime CreationTime;                                          // +0x00
    CDateAndTime LastAccessTime;                                        // +0x1c
    CDateAndTime LastWriteTime;                                         // +0x38
    bool ReadOnly;                                                      // +0x54
};
enum FileAccessType { FILE_ACCESS_READ = 1 };
class CASuspendableProcess;
class CAFile : public CBaseClassNonCopyable { public:
    static CWideString __fastcall ConvertPathToFullPath(const CWideString& path);   // 0x00999110
};
class CDiskFileWin32 : public CAFile { public:                          // vtable 0x0122D06C
    long        AccessType;                                             // +0x04
    long        Position;                                               // +0x08
    long        Length;                                                 // +0x0c
    void*       WinFileHandle;                                          // +0x10
    bool        Opened;                                                 // +0x14
    CWideString Pathname;                                               // +0x18
    CDiskFileWin32(const CWideString& pathname, FileAccessType access, unsigned long flags);   // 0x0099AD80
    virtual ~CDiskFileWin32() { Close(); }
    virtual void Close();                                               // 0x0099A920
    virtual void Read(void* buffer, long size, CASuspendableProcess* process);   // 0x0099A960
    virtual void SetPosition(long position);                            // 0x0099ABF0
    virtual CFileInformation GetFileInformation();                      // 0x0099A5A0
};
class CDataStream { public:
    virtual ~CDataStream();                                             // 0x00993B10
};
class CDataInputStream : public CDataStream { public:
    unsigned char _body[0x14];                                          // +0x04 .. 0x18
};
class CDataOutputStream : public CDataStream { public:
    unsigned char _body[4];
};
class CMemoryDataOutputStream : public CDataOutputStream { public:      // 0x18 bytes
    unsigned char _data[0x0c];                                          // +0x08 CArray<char> Data
    unsigned long CurrentPosition;                                      // +0x14
    CMemoryDataOutputStream();                                          // 0x00996AF0
    virtual ~CMemoryDataOutputStream();                                 // 0x00996B30
    void Write(const void* data, unsigned long size);                   // 0x00996CC0
    unsigned long GetLength() const;                                    // 0x00994B40
};
class CMemoryDataInputStream : public CDataInputStream { public:        // 0x1c bytes
    unsigned char* StreamSource;                                        // +0x18
    CMemoryDataInputStream(const CMemoryDataOutputStream& source);      // 0x00995EE0
};
class CFileDataInputStream : public CDataInputStream { public:          // 0x24 bytes
    CAFile*        File;                                                // +0x18
    unsigned char* Buffer;                                              // +0x1c
    long           BufferSize;                                          // +0x20
    CFileDataInputStream(CAFile& file, unsigned long bufferSize);       // 0x00994700
    virtual ~CFileDataInputStream();                                    // 0x00994780
};
class CBankStateBlock { public:
    virtual ~CBankStateBlock() { }
};
class CStateBlock { public: long State; };
class CWADBankStateBlock : public CBankStateBlock { public:             // vtable 0x0129CFD0, 8 bytes
    CStateBlock State;                                                  // +0x04
    CWADBankStateBlock() { }
};
#include "engine/CCountedPointer.h"                                     // the shared retail model (landed 0x00B2C4E0 AddCustomMorphTexture uses it with AddEntry)
namespace std {
inline void* __copy_trivial(const void* __first, const void* __last, void* __result)
{
    return (__last == __first) ? __result :
        ((char*)memmove(__result, __first, ((const char*)__last - (const char*)__first))) + ((const char*)__last - (const char*)__first);
}
inline char* copy(const char* __first, const char* __last, char* __result)
{
    return (char*)__copy_trivial(__first, __last, __result);
}
inline void _Destroy(char*, char*) { }                                  // __destroy_aux(..., __true_type)
template <class _Tp> class allocator { public:
    allocator() { }
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc { public:
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
    _Value _M_data;
};
template <class _Tp, class _Alloc> class _Vector_base { public:
    _Vector_base(const _Alloc& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, 0) { }
    ~_Vector_base() { if (_M_start != 0) _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start); }
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> { public:
    typedef _Tp* iterator;
    typedef unsigned int size_type;
    vector() : _Vector_base<_Tp, _Alloc>(_Alloc()) { }
    iterator begin() { return this->_M_start; }
    iterator end() { return this->_M_finish; }
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    void _M_fill_insert(iterator __pos, size_type __n, const _Tp& __x);   // 0x004110D0
    void insert(iterator __pos, size_type __n, const _Tp& __x) { _M_fill_insert(__pos, __n, __x); }
    iterator erase(iterator __first, iterator __last)
    {
        iterator __i = copy(__last, this->_M_finish, __first);
        _Destroy(__i, this->_M_finish);
        this->_M_finish = __i;
        return __first;
    }
    __forceinline void resize(size_type __new_size, const _Tp& __x = _Tp())
    {
        if (__new_size < size())
            erase(begin() + __new_size, end());
        else
            insert(end(), __new_size - size(), __x);
    }
};
}
template <class T> class CStdVector : public std::vector<T> { public:   // CArray<T>-style helpers on the STLport vector
    long GetCount() const { return (long)this->size(); }
    T* GetData() { return this->_M_finish - this->_M_start != 0 ? this->_M_start : (T*)this; }
};
typedef CStdVector<char> CByteVector;
struct CRuntimeData { unsigned long DataOffset, DataSize; unsigned char DataType; bool Valid; unsigned char _pad[2]; };
struct CPackedUIntArray { unsigned long m_bitsPerValue; unsigned long* m_pData; unsigned long m_count, m_capacity; };
template <class T> class CArray { public:                               // retail CArray: 12 bytes
    T* m_pFirst;
    T* m_pLast;
    T* m_pEnd;
    T& operator[](unsigned long index) const { return m_pFirst[index]; }
};
class CBankFileEntryUpdateData { public:
    void ReadInfo(unsigned long offset, void* dest, unsigned long size) const;   // 0x009CBE90
};
class CBankFile {                                                       // the retail layout (landed 00/9c CBankFile_Retail)
public:
    virtual ~CBankFile();
    unsigned long             Size;                                     // +0x04
    unsigned char             Symbols[0x0c];                            // +0x08
    unsigned char             Checksums[0x0c];                          // +0x14
    CArray<CRuntimeData>      RuntimeData;                              // +0x20
    CArray<CBankFileEntryUpdateData*> UpdateData;                       // +0x2c
    CPackedUIntArray          PackedDataOffset;                         // +0x38
    CPackedUIntArray          PackedDataSize;                           // +0x48
    CPackedUIntArray          PackedDataType;                           // +0x58
    CPackedUIntArray          PackedValid;                              // +0x68
    unsigned long             OpenFlags;                                // +0x78
    unsigned char             _body7c[0x14];                            // +0x7c .. 0x90
    unsigned long FindIndexBySymbol(const CCharString& symbol) const;   // 0x009CCDC0
    void AddEntry(const CCharString& symbol, unsigned long type, const CCharString& filename, CCountedPointer<CBankStateBlock> state);   // 0x009CD620
    unsigned long GetEntryDataSize(unsigned long index) const;          // 0x009CC240
    void ReadEntryData(unsigned long index, unsigned long offset, void* buffer, unsigned long size);   // 0x009CC2A0
    void UpdateEntry(unsigned long index, CDataInputStream& data, unsigned long dataSize, CDataInputStream& info, unsigned long infoSize, bool computeChecksum);   // 0x009D1320
};
class CBankFileAsync : public CBankFile { public:
    unsigned char _async[0x164 - 0x90];
};
class CWADBank : public CBankFileAsync { public:
    void AddFile(const CWideString& fileName);
};

void CWADBank::AddFile(const CWideString& fileName)
{
    unsigned long index = FindIndexBySymbol(fileName.ConvertToCharString());
    bool entryExists = true;
    if (index == 0) {
        CCountedPointer<CBankStateBlock> state(new CWADBankStateBlock);
        {
            CCharString filename("");
            AddEntry(fileName.ConvertToCharString(), 0, filename, state);
        }
        index = FindIndexBySymbol(fileName.ConvertToCharString());
        entryExists = false;
    }
    CWideString fullPath = CAFile::ConvertPathToFullPath(fileName);
    CDiskFileWin32 file(fullPath, FILE_ACCESS_READ, 2);
    if (entryExists) {
        entryExists = false;                                            // from here: "the entry differs from the file"
        if (GetEntryDataSize(index) != (unsigned long)file.Length)
            entryExists = true;
        else {
            CFileInformation storedInfo;
            UpdateData[index]->ReadInfo(0, &storedInfo, sizeof(CFileInformation));
            if (!(file.GetFileInformation().LastWriteTime == storedInfo.LastWriteTime))
                return;
            CByteVector entryData;
            CByteVector fileData;
            entryData.resize(GetEntryDataSize(index));
            ReadEntryData(index, 0, entryData.GetData(), GetEntryDataSize(index));
            fileData.resize(file.Length);
            file.Read(fileData.GetData(), file.Length, 0);
            file.SetPosition(0);
            if (memcmp(entryData.GetData(), fileData.GetData(), entryData.GetCount()) != 0)
                entryExists = true;
        }
        if (!entryExists)
            return;
    }
    CFileDataInputStream data(file, 0x4000);
    CMemoryDataOutputStream infoOut;
    CFileInformation info = file.GetFileInformation();
    infoOut.Write(&info, sizeof(CFileInformation));
    CMemoryDataInputStream infoIn(infoOut);
    unsigned long dataSize = file.Length;
    UpdateEntry(index, data, dataSize, infoIn, infoOut.GetLength(), false);
}
