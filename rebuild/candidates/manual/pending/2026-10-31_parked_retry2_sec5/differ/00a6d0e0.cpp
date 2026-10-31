// CPhysicsMesh::GetHelperDataFromFileData(const C3DMeshFile& file) -- read the mesh file's helper points and helper dummies into the two name-keyed maps, then sort both
// Grade: DIFFER(930v984) -- root chunk held in a local for HasHelpers/GetHelpersChunk (381 diff lines v 439); left:
//   frame 0xfc vs 0xf4, push_back temporaries
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A6D0E0, 984 bytes, `ret 4`.
// Callees: CCharString 0x0099EBF0 / 0x0099EC30 / 0x0099EAE0, GetSubString 0x0099EC70, GFCharStringToInt 0x0099E7F0, operator delete 0x00BFE9BC, GetRootChunk 0x00A32620, HasHelpers 0x00A9B2C0, GetHelpersChunk 0x00A9B3D0, GetFirst/NextHelperPoint 0x00AAA270 / 0x00AAA300, GetFirst/NextHelperDummy 0x00AAA550 / 0x00AAA5E0, C3DHelperPoint 0x00A2E470, C3DHelperDummy 0x00A2E490, _M_insert_overflow 0x00A71FC0.
// Layout: CPhysicsMesh (retail, from the disassembly: `add edi,0x10` then [edi+4]/[edi+8] and flag [edi+0xd]): helper-point map +0x10 (sorted flag +0x1d), helper-dummy map +0x20 (sorted flag +0x2d). Local struct: no shared CPhysicsMesh header models these STLport map offsets.
// Shape: two chunk loops (points then dummies), each building a name-keyed entry and appending with the inline push_back; both vectors re-sorted at the end.
extern "C" int __cdecl strncmp(const char* a, const char* b, unsigned int n);
inline void* operator new(unsigned int, void* p) { return p; }
void operator delete(void* p);                         // 0x00BFE9BC
struct CStringData {
    char*         PData;
    unsigned long StringLength;
};
struct CCharString {
    CStringData* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    CCharString(const CCharString& other);             // 0x0099EC30
    ~CCharString();                                    // 0x0099EAE0
    CCharString GetSubString(long start, long end) const;   // 0x0099EC70
    const char* GetString() const { return PStringData ? PStringData->PData : ""; }
    unsigned long Length() const { return PStringData ? PStringData->StringLength : 0; }
};
long __fastcall GFCharStringToInt(const CCharString& text);   // 0x0099E7F0
struct CCPPointerInfo {
    long RefCount;
    void (__fastcall* DeleteFunc)(void* data);
    void* Data;
};
template <class T> struct CCountedPointer {
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer() : Data(0), Info(0) {}
    CCountedPointer(const CCountedPointer& o) : Data(o.Data), Info(o.Info) { if (Info) ++Info->RefCount; }
    ~CCountedPointer()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
        Data = 0;
        Info = 0;
    }
    CCountedPointer& operator=(const CCountedPointer& o)
    {
        if (Info != o.Info) {
            if (Info) {
                --Info->RefCount;
                if (Info->RefCount == 0) {
                    Info->DeleteFunc(Info->Data);
                    delete Info;
                }
            }
            Data = o.Data;
            Info = o.Info;
            if (Info) ++Info->RefCount;
        }
        return *this;
    }
};
template <class T> struct CIVCountedPointer {
    T*              Data;
    CCPPointerInfo* Info;
    ~CIVCountedPointer()
    {
        if (Info && --Info->RefCount == 0) {
            Info->DeleteFunc(Info->Data);
            delete Info;
        }
    }
};
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
};
struct CMatrix3x4 { float M[12]; };
struct C3DMeshFileHelperPointChunk {
    unsigned char _pad00[0x2c];
    C3DVector     Position;                            // +0x2c
    unsigned char _pad38[4];
    long          Bone;                                // +0x3c
    CCharString   Name;                                // +0x40
};
struct C3DMeshFileHelperDummyChunk {
    unsigned char _pad00[0x3c];
    long          Bone;                                // +0x3c
    CCharString   Name;                                // +0x40
    unsigned char _pad44[0xc];
    CMatrix3x4    Matrix;                              // +0x50
};
struct C3DMeshFileHelpersChunk {
    CCountedPointer<C3DMeshFileHelperPointChunk> GetFirstHelperPoint() const;   // 0x00AAA270
    CCountedPointer<C3DMeshFileHelperPointChunk> GetNextHelperPoint(const CCountedPointer<C3DMeshFileHelperPointChunk>& current) const;   // 0x00AAA300
    CCountedPointer<C3DMeshFileHelperDummyChunk> GetFirstHelperDummy() const;   // 0x00AAA550
    CCountedPointer<C3DMeshFileHelperDummyChunk> GetNextHelperDummy(const CCountedPointer<C3DMeshFileHelperDummyChunk>& current) const;   // 0x00AAA5E0
};
struct C3DMeshFileRootChunk {
    bool HasHelpers() const;                           // 0x00A9B2C0
    CCountedPointer<C3DMeshFileHelpersChunk> GetHelpersChunk() const;   // 0x00A9B3D0
};
struct C3DMeshFile {
    CIVCountedPointer<C3DMeshFileRootChunk> GetRootChunk() const;   // 0x00A32620
};
struct C3DHelperPoint {
    C3DVector Position;
    long      Bone;
    C3DHelperPoint(const C3DVector& position, long bone);   // 0x00A2E470
};
struct C3DHelperDummy {
    CMatrix3x4 Matrix;
    long       Bone;
    C3DHelperDummy(const CMatrix3x4& matrix, long bone);    // 0x00A2E490
};
struct CHelperPointEntry {
    CCharString    first;
    C3DHelperPoint second;
    CHelperPointEntry(const CCharString& f, const C3DHelperPoint& s) : first(f), second(s) {}
};
struct CHelperDummyEntry {
    CCharString    first;
    C3DHelperDummy second;
    CHelperDummyEntry(const CCharString& f, const C3DHelperDummy& s) : first(f), second(s) {}
};
struct __false_type {};
template <class E, int Overflow> struct CNameMap;
struct CHelperPointMap {
    CHelperPointEntry* _M_start;
    CHelperPointEntry* _M_finish;
    CHelperPointEntry* _M_end_of_storage;
    bool               Compare;                        // +0x0c
    bool               NeedsSort;                      // +0x0d
    void _M_insert_overflow(CHelperPointEntry* pos, const CHelperPointEntry& x, const __false_type&, unsigned int n, bool atEnd);   // 0x00A71FC0
    void push_back(const CHelperPointEntry& x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) CHelperPointEntry(x);
            ++_M_finish;
        } else
            _M_insert_overflow(_M_finish, x, __false_type(), 1, true);
    }
};
struct CHelperDummyMap {
    CHelperDummyEntry* _M_start;
    CHelperDummyEntry* _M_finish;
    CHelperDummyEntry* _M_end_of_storage;
    bool               Compare;                        // +0x0c
    bool               NeedsSort;                      // +0x0d
    void _M_insert_overflow(CHelperDummyEntry* pos, const CHelperDummyEntry& x, const __false_type&, unsigned int n, bool atEnd);   // 0x00A720E0
    void push_back(const CHelperDummyEntry& x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) CHelperDummyEntry(x);
            ++_M_finish;
        } else
            _M_insert_overflow(_M_finish, x, __false_type(), 1, true);
    }
};
void __fastcall SortHelperPoints(CHelperPointEntry* first, CHelperPointEntry* last, bool compare);   // 0x00A735A0
void __fastcall SortHelperDummies(CHelperDummyEntry* first, CHelperDummyEntry* last, bool compare);  // 0x00A73600

struct CPhysicsMesh {
    unsigned char   _pad00[0x10];
    CHelperPointMap HelperPoints;                      // +0x10
    CHelperDummyMap HelperDummies;                     // +0x20
    void GetHelperDataFromFileData(const C3DMeshFile& file);
};

void CPhysicsMesh::GetHelperDataFromFileData(const C3DMeshFile& file)
{
    CIVCountedPointer<C3DMeshFileRootChunk> root = file.GetRootChunk();
    C3DMeshFileRootChunk* rootChunk = root.Data;
    if (rootChunk->HasHelpers()) {
        CCountedPointer<C3DMeshFileHelpersChunk> chunk = rootChunk->GetHelpersChunk();
        for (CCountedPointer<C3DMeshFileHelperPointChunk> point = chunk.Data->GetFirstHelperPoint(); point.Data; point = chunk.Data->GetNextHelperPoint(point)) {
            HelperPoints.push_back(CHelperPointEntry(point.Data->Name, C3DHelperPoint(C3DVector(point.Data->Position), point.Data->Bone)));
            HelperPoints.NeedsSort = true;
        }
        for (CCountedPointer<C3DMeshFileHelperDummyChunk> dummy = chunk.Data->GetFirstHelperDummy(); dummy.Data; dummy = chunk.Data->GetNextHelperDummy(dummy)) {
            HelperDummies.push_back(CHelperDummyEntry(dummy.Data->Name, C3DHelperDummy(CMatrix3x4(dummy.Data->Matrix), dummy.Data->Bone)));
            HelperDummies.NeedsSort = true;
        }
    }
    SortHelperPoints(HelperPoints._M_start, HelperPoints._M_finish, HelperPoints.Compare);
    HelperPoints.NeedsSort = false;
    SortHelperDummies(HelperDummies._M_start, HelperDummies._M_finish, HelperDummies.Compare);
    HelperDummies.NeedsSort = false;
}
