// Grade: DIFFER(908v875) -- first-pass model: frame 0x18 larger than retail (the resize default CRegion temporary and the stream share slots differently), register roles ebp/ebx swapped (retail keeps 0 in ebp and this in a frame slot); the inline singleton Get and the inline STLport char-vector fill are modelled
// CWorldMap::LoadFromFile(const CWideString& filename) -- read the whole world-map file into memory, then stream in the map infos and the regions (entry 0 of each stays the default), place every map and register it with the engine manager
// Leaf: ?LoadFromFile@CWorldMap@@QAEXABVCWideString@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00507650, 873 bytes (+ int3 padding), ret 4.
// Callees: CDiskFileWin32(const CWideString&, FileAccessType, ulong) 0x0099AD80 / Read 0x0099A960 / Close 0x0099A920,
//   ~CWideString 0x0099B510, ~CBase 0x0099A300, malloc 0x00BFEA0E / free 0x00BFEA14, CMemoryDataInputStream(void*, ulong)
//   0x00994930 / dtor 0x00993B10, ReadWithSrcChunkOverflow 0x00993CA0, CArray<CMapInfo>::resize 0x0051BE20,
//   CMapInfo::LoadBinary 0x004FB4F0, CRegion() 0x006BC410 / ~CRegion 0x0051A110 / LoadBinary 0x006BC510,
//   vector<CRegion>::erase 0x0051CC80 / _M_fill_insert 0x0051D0A0, SetMapPlacement 0x004FC9C0, operator new 0x00BFEA1A,
//   operator delete 0x00BFE9BC, CEngineManager::AddMap 0x006403A0; CTBaseSingleton<CEngineManager> instance 0x013BA854
//   (counted pointer, info 0x013BA858, deleter 0x004361E0), CEngineManager vtable 0x01231584.
#include <new>
#include <stdlib.h>
#include <string.h>
#pragma intrinsic(memset, memcpy)
void* operator new(unsigned int size);                 // 0x00BFEA1A
void operator delete(void* p);                         // 0x00BFE9BC

struct CWideString {
    void* PStringData;
    ~CWideString();                                    // 0x0099B510
};
struct CBase {
    CBase();                                           // 0x0099A2F0
    virtual ~CBase();                                  // 0x0099A300
};
enum FileAccessType { FILE_ACCESS_READ = 1 };
struct CASuspendableProcess;
class CDiskFileWin32 : public CBase {
public:
    CDiskFileWin32(const CWideString& pathname, FileAccessType access, unsigned long flags);   // 0x0099AD80
    virtual ~CDiskFileWin32() { Close(); }
    virtual void Read(void* buffer, long size, CASuspendableProcess* process = 0);           // 0x0099A960
    virtual void Close();                                                                     // 0x0099A920
    unsigned char _pad04[0x14];
    CWideString   Pathname;                            // +0x18
    long          Size;                                // +0x1C
};
struct CDataInputStream {
    void* __vftable;
    unsigned long StreamPos;                           // +0x04
    long StreamSize;                                   // +0x08
    const unsigned char* CurrentSourcePtr;             // +0x0c
    unsigned long SourceChunkStreamPos;                // +0x10
    long SourceChunkBytesRemaining;                    // +0x14
    void ReadWithSrcChunkOverflow(unsigned char* buffer, long size);   // 0x00993CA0
    static __forceinline bool IsValidPosition(__int64 p) { return p >= 0 && p <= 0x7fffffff; }
    __forceinline void Read(void* dst, long size) {
        __int64 np = (__int64)(StreamPos + size);
        if (!IsValidPosition(np)) return;
        if (SourceChunkBytesRemaining >= size) {
            memcpy(dst, CurrentSourcePtr, size); CurrentSourcePtr += size; SourceChunkBytesRemaining -= size; StreamPos += size;
        } else ReadWithSrcChunkOverflow((unsigned char*)dst, size);
    }
};
struct CMemoryDataInputStream : CDataInputStream {
    CMemoryDataInputStream(void* data, unsigned long size);   // 0x00994930
    ~CMemoryDataInputStream();                                 // 0x00993B10
    unsigned char _pad18[0x14];
};
struct CCharBuffer {
    char* _M_start;
    char* _M_finish;
    char* _M_end_of_storage;
    explicit CCharBuffer(unsigned long n)
    {
        _M_start = 0; _M_finish = 0; _M_end_of_storage = 0;
        _M_start = n ? (char*)malloc(n) : 0;
        _M_end_of_storage = _M_start + n;
        _M_finish = _M_start;
        memset(_M_start, 0, n);
        _M_finish = _M_start + n;
    }
    ~CCharBuffer() { if (_M_start) free(_M_start); }
    long size() const { return _M_finish - _M_start; }
    char* Data() { return size() ? _M_start : (char*)this; }
};
struct C2DBoxI { long TLX, TLY, BRX, BRY; };
struct CMapInfo {
    C2DBoxI Area;
    unsigned char _body[0x38];
    void LoadBinary(CDataInputStream* stream);         // 0x004FB4F0
};
struct CMapInfoArray {
    CMapInfo* _M_start; CMapInfo* _M_finish; CMapInfo* _M_end_of_storage;
    void resize(long n);                               // 0x0051BE20
};
struct CRegion {
    CRegion();                                         // 0x006BC410
    ~CRegion();                                        // 0x0051A110
    void LoadBinary(CDataInputStream* stream);         // 0x006BC510
    unsigned char _body[0x58];
};
struct CRegionVector {
    CRegion* _M_start; CRegion* _M_finish; CRegion* _M_end_of_storage;
    unsigned long size() const { return _M_finish - _M_start; }
    CRegion* erase(CRegion* first, CRegion* last);     // 0x0051CC80
    void insert(CRegion* position, unsigned long n, const CRegion& value);   // 0x0051D0A0
    void resize(unsigned long n, const CRegion& value = CRegion())
    {
        if (n < size())
            erase(_M_start + n, _M_finish);
        else
            insert(_M_finish, n - size(), value);
    }
};
struct CCPPointerInfo { long RefCount; void (__fastcall* DeleteFunc)(void* data); void* Data; };
class CEngineManager {
public:
    CEngineManager() : f8(0), fC(0), f10(0)
    {
        ListNode* node = (ListNode*)malloc(sizeof(ListNode));
        node->next = node; node->prev = node;
        List = node;
    }
    virtual ~CEngineManager();
    struct ListNode { ListNode* next; ListNode* prev; long data; };
    ListNode* List;                                    // +0x04
    long f8, fC, f10;
    void AddMap(long map);                             // 0x006403A0
};
void __fastcall DeleteEngineManager(void* object);     // 0x004361E0
template <class T> struct CTBaseSingleton {
    static T*              s_Data;                     // 0x013BA854
    static CCPPointerInfo* s_Info;                     // 0x013BA858
    static __forceinline T& Get()
    {
        if (!s_Data) {
            T* object = new T;
            if (s_Info) {
                if (--s_Info->RefCount == 0) {
                    s_Info->DeleteFunc(s_Info->Data);
                    delete s_Info;
                }
            }
            s_Info = 0;
            s_Data = object;
            if (object) {
                CCPPointerInfo* info = (CCPPointerInfo*)operator new(sizeof(CCPPointerInfo));
                if (info) {
                    info->RefCount = 1;
                    info->DeleteFunc = DeleteEngineManager;
                    info->Data = s_Data;
                    s_Info = info;
                }
                else
                    s_Info = 0;
            }
        }
        return *s_Data;
    }
};

class CWorldMap {
public:
    unsigned char  _pad00[0x20];
    CMapInfoArray  Maps;                               // +0x20
    CRegionVector  Regions;                            // +0x2C
    void SetMapPlacement(const C2DBoxI& area, long map);   // 0x004FC9C0
    void LoadFromFile(const CWideString& filename);
};

void CWorldMap::LoadFromFile(const CWideString& filename)
{
    CDiskFileWin32 file(filename, FILE_ACCESS_READ, 2);
    CCharBuffer buffer(file.Size + 1);
    file.Read(buffer.Data(), buffer.size());
    CMemoryDataInputStream stream(buffer.Data(), buffer.size());
    long mapCount;
    stream.Read(&mapCount, sizeof(mapCount));
    Maps.resize(mapCount);
    for (long i = 1; i < mapCount; ++i)
        Maps._M_start[i].LoadBinary(&stream);
    long regionCount;
    stream.Read(&regionCount, sizeof(regionCount));
    Regions.resize(regionCount);
    for (long r = 1; r < regionCount; ++r)
        Regions._M_start[r].LoadBinary(&stream);
    for (long m = 1; m < mapCount; ++m)
    {
        SetMapPlacement(Maps._M_start[m].Area, m);
        CTBaseSingleton<CEngineManager>::Get().AddMap(m);
    }
}
