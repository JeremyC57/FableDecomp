// GFLoadSkeletalMorphTargetFromFile(CAFile& file, C3DSkeletalMorphTarget& morph_target) -- read the whole file as text and parse the bone block written by GFSaveSkeletalMorphTargetToFile ("<bone>: x, y, z;" lines between the start and end markers); on success initialise the morph target with the file's base name and the parsed bone morphs
// Grade: DIFFER(970v971) -- exit layout only: retail inlines the implicit ~CStringParser at the start-marker return alone and calls it out of line (0x004047A0) everywhere else, so the four loop-failure returns (~bone_name) fall into the shared ~CArray/~CStringParser calls; ours also inlines it at the loop-failure returns, which then get their own cleanup block after the ret. Every instruction of the body otherwise lines up.
// Leaf: ?GFLoadSkeletalMorphTargetFromFile@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0088B0C0, 971 bytes, /Gr `__fastcall` (file in ecx, morph target in edx,
//   spilled to the frame), plain `ret`, no EH frame.
// Identity: Ego_r.pdb `?GFLoadSkeletalMorphTargetFromFile@@YIXAAVCAFile@@AAVC3DSkeletalMorphTarget@@@Z` (the catalog
//   agrees); the reader of GFSaveSkeletalMorphTargetToFile 0x0088AC90.
// Callees: CAFile slots 9 GetLength (+0x24), 5 SetPosition (+0x14), 3 Read (+0x0c), 11 GetPathname (+0x2c); malloc/free
//   (the STLport vector<char> buffer); CStringParser: CBaseClass ctor 0x0099A2D0 + vtable 0x0122D874, Init 0x009B9530,
//   NextItemIs 0x009B9BA0, SkipPastNextItemIfIs 0x009B9E00, ReadAsStringUntilString 0x009BA330, ReadAsFloat 0x009BA650,
//   PeekNextItemType 0x009BA900, comment list dtor 0x004046B0, out-of-line ~CStringParser 0x004047A0;
//   C3DSkeletalBoneMorph(const CCharString&, const C3DVector&) 0x00A44DF0, vector<C3DSkeletalBoneMorph>::_M_insert_overflow
//   0x0088B4F0, ~CArray<C3DSkeletalBoneMorph> 0x006C39A0, CWideString() 0x0099AED0 / ~CWideString 0x0099B510,
//   CAFile::SplitPath 0x00997620, CWideString::ToCharString 0x0099B2C0, C3DSkeletalMorphTarget::Initialise 0x00A45150.
// Strings: "" 0x0122D70E, "#Start_Bone_data\r\n" 0x012780AC, ": " 0x012780A8, ", " 0x012397CC, ";\r\n" 0x012353B8,
//   "#End_bone_data\r\n" 0x01278094.
// Shape: the buffer is an STLport vector<char>(length + 1) (allocate's zero test, the inline memset fill, the inline
//   `if (start) free(start)` dtor); the marker and separator strings are IMPLICIT conversions (each re-lea'd for the
//   push), the bool negated before the temporary's dtor (`neg/sbb/inc bl`); an explicit CCharString(x) temporary is
//   pushed from the ctor's eax instead.
extern "C" void* __cdecl malloc(unsigned int size);         // 0x00BFEA0E
extern "C" void __cdecl free(void* p);                      // 0x00BFEA14
extern "C" void* __cdecl memset(void* dst, int value, unsigned int size);
#pragma intrinsic(memset)
inline void* operator new(unsigned int, void* where) { return where; }

class CASuspendableProcess;
class CWideString {
public:
    void* PStringData;
    CWideString();                                          // 0x0099AED0
    ~CWideString();                                         // 0x0099B510
    class CCharString ToCharString() const;                 // 0x0099B2C0
};

class CCharString {
public:
    void* PStringData;                                      // +0x00
    CCharString();                                          // 0x0099E4B0
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    CCharString(const CCharString& other);                  // 0x0099EC30
    ~CCharString();                                         // 0x0099EAE0
};

class CAFile {
public:
    virtual ~CAFile();
    virtual void Open(const CWideString& name, long access, unsigned long flags) = 0;
    virtual void Close() = 0;                                                       // slot 2
    virtual void Read(void* dst, long size, CASuspendableProcess* process) = 0;     // slot 3 (+0x0c)
    virtual void Write(const void* src, long size, CASuspendableProcess* process) = 0;
    virtual void SetPosition(long pos) = 0;                                         // slot 5 (+0x14)
    virtual void MovePosition(long delta) = 0;
    virtual long GetPosition() const = 0;
    virtual void SetLength(long length) = 0;
    virtual long GetLength() const = 0;                                             // slot 9 (+0x24)
    virtual bool IsOpen() const = 0;
    virtual CWideString GetPathname() const = 0;                                    // slot 11 (+0x2c)
    static void __fastcall SplitPath(const CWideString& path, CWideString* drive, CWideString* directory,
                                     CWideString* filename, CWideString* extension);   // 0x00997620
};

class C3DVector {
public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
};

class C3DSkeletalBoneMorph {                                // Ego_r.pdb, 20 bytes
public:
    CCharString   BoneName;                                 // +0x00
    unsigned long BoneNameCRC;                              // +0x04
    C3DVector     Scaling;                                  // +0x08
    C3DSkeletalBoneMorph(const CCharString& bone_name, const C3DVector& scaling);   // 0x00A44DF0
};

namespace std {

struct __true_type { };
struct __false_type { __false_type() { } };

template <class _T1, class _T2>
inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _Tp> class allocator {
public:
    allocator() { }
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};

inline void fill(char* __first, char* __last, const char& __val)
{
    char __tmp = __val;
    memset(__first, __tmp, __last - __first);
}
inline char* uninitialized_fill_n(char* __first, unsigned int __n, const char& __x)
{
    fill(__first, __first + __n, __x);
    return __first + __n;
}

template <class _Tp, class _Alloc>
class _Vector_base {
public:
    _Vector_base(const _Alloc& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, 0) { }
    _Vector_base(unsigned int __n, const _Alloc& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, 0)
    {
        _M_start = _M_end_of_storage.allocate(__n);
        _M_finish = _M_start;
        _M_end_of_storage._M_data = _M_start + __n;
    }
    ~_Vector_base() { _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start); }
    _Tp* _M_start;                                                  // +0x00
    _Tp* _M_finish;                                                 // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;         // +0x08
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* pointer;
    typedef unsigned int size_type;
    vector() : _Vector_base<_Tp, _Alloc>(_Alloc()) { }
    explicit vector(size_type __n) : _Vector_base<_Tp, _Alloc>(__n, _Alloc())
    {
        this->_M_finish = uninitialized_fill_n(this->_M_start, __n, _Tp());
    }
    _Tp& operator[](size_type __n) { return *(this->_M_start + __n); }
    void push_back(const _Tp& __x)
    {
        if (this->_M_finish != this->_M_end_of_storage._M_data) {
            _Construct(this->_M_finish, __x);
            ++this->_M_finish;
        } else
            _M_insert_overflow(this->_M_finish, __x, __false_type(), 1UL, true);
    }
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, size_type __fill_len, bool __atend);   // 0x0088B4F0
};


} // namespace std

template <class T> class CArray : public std::vector<T> {
public:
    CArray() { }
    ~CArray();                                              // 0x006C39A0 for C3DSkeletalBoneMorph (out of line)
};

class C3DSkeletalMorphTarget {
public:
    CCharString                   Name;                     // +0x00
    CArray<C3DSkeletalBoneMorph>  BoneMorphs;               // +0x04
    void Initialise(const CCharString& name, const CArray<C3DSkeletalBoneMorph>& bone_morphs);   // 0x00A45150
};

class CBaseClass {
public:
    CBaseClass();                                           // 0x0099A2D0
    virtual ~CBaseClass();                                  // 0x0099A2E0
};
struct SCommentNode {                                       // list<CStringParserCommentBoundaries> node
    SCommentNode* m_next;
    SCommentNode* m_prev;
    char*         PStart;
    char*         PEnd;
};
struct CCommentList {                                       // retail list: the sentinel pointer
    SCommentNode* m_head;
    CCommentList() : m_head(0)
    {
        SCommentNode* node = (SCommentNode*)malloc(sizeof(SCommentNode));
        node->m_next = node;
        node->m_prev = node;
        m_head = node;
    }
    ~CCommentList();                                        // 0x004046B0
};
struct CCursorPos { bool InQuotes; long StringPos; long Line; };
typedef void (__cdecl* TErrorCallback)(CCharString*);
enum EParsedItemType { PARSED_ITEM_NONE = 1 };
class CStringParser : public CBaseClass {                   // vtable 0x0122D874, retail 0x28 bytes
public:
    char*          String;                                  // +0x04
    unsigned long  BaseLineOffset;                          // +0x08
    CCharString    Filename;                                // +0x0c
    CCharString    TempCharString;                          // +0x10
    CCommentList   Comments;                                // +0x14
    TErrorCallback ErrorCallback;                           // +0x18
    CCursorPos     CursorPos;                               // +0x1c
    CStringParser(char* text, TErrorCallback callback, const CCharString& filename, long baseLine)
    {
        Init(text, callback, filename, baseLine);
    }
    void Init(char* text, TErrorCallback callback, const CCharString& filename, long baseLine);   // 0x009B9530
    bool NextItemIs(const CCharString& item);               // 0x009B9BA0
    bool SkipPastNextItemIfIs(const CCharString& item);     // 0x009B9E00
    CCharString ReadAsStringUntilString(const CCharString& end);   // 0x009BA330
    float ReadAsFloat();                                    // 0x009BA650
    EParsedItemType PeekNextItemType();                     // 0x009BA900
};

void __fastcall GFLoadSkeletalMorphTargetFromFile(CAFile& file, C3DSkeletalMorphTarget& morph_target);

void __fastcall GFLoadSkeletalMorphTargetFromFile(CAFile& file, C3DSkeletalMorphTarget& morph_target)
{
    std::vector<char> buffer(file.GetLength() + 1);
    file.SetPosition(0);
    file.Read(&buffer[0], file.GetLength(), 0);
    buffer[file.GetLength()] = 0;
    CStringParser parser(&buffer[0], 0, CCharString(""), 0);
    if (!parser.NextItemIs("#Start_Bone_data\r\n"))
        return;
    CArray<C3DSkeletalBoneMorph> bone_morphs;
    while (parser.PeekNextItemType() != PARSED_ITEM_NONE) {
        CCharString bone_name = parser.ReadAsStringUntilString(": ");
        if (!parser.SkipPastNextItemIfIs(": "))
            return;
        C3DVector scaling;
        scaling.X = parser.ReadAsFloat();
        if (!parser.SkipPastNextItemIfIs(", "))
            return;
        scaling.Y = parser.ReadAsFloat();
        if (!parser.SkipPastNextItemIfIs(", "))
            return;
        scaling.Z = parser.ReadAsFloat();
        if (!parser.SkipPastNextItemIfIs(";\r\n"))
            return;
        bone_morphs.push_back(C3DSkeletalBoneMorph(bone_name, scaling));
    }
    if (!parser.SkipPastNextItemIfIs("#End_bone_data\r\n"))
        return;
    CWideString filename;
    CAFile::SplitPath(file.GetPathname(), 0, 0, &filename, 0);
    morph_target.Initialise(filename.ToCharString(), bone_morphs);
}
