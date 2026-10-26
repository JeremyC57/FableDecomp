// C3DMeshStats::UnpackNames(std::vector<CCharString>&, bool, const CCharString&, bool) const -- append every packed helper-point (or dummy-object) name that starts with the prefix, once per object stored under it
// Leaf: UnpackNames
// Retail @ 0x00A2ED10, 468 bytes (ret 10h).
// Callees: strncmp 0x00BFEAF8, CCharString(const char*, long) 0x0099EBF0 / copy ctor 0x0099EC30 / ~CCharString 0x0099EAE0,
//   CRC Calc 0x004014A0 (__fastcall seed, data, length), CPreAllocVectorMap<ulong,C3DDummyObject>::UpperBound 0x00A317E0 /
//   LowerBound 0x00A31790, CPreAllocVectorMap<ulong,C3DHelperPoint>::UpperBound 0x00A31700 / LowerBound 0x00A316B0,
//   vector<CCharString>::_M_insert_overflow 0x00433530.
// Shape: PackedNames starts with the ushort offset of the dummy-object names (helper names follow the word); the
// prefix length is taken once; each name is a CCharString temporary (in the dead names-argument slot) pushed
// `count` times -- 1 when every name is wanted once, else the number of map entries under the name's CRC
// (UpperBound - LowerBound over 56- or 20-byte pairs). push_back is the inline STLport form with the __false_type
// tag over the dead bool slot.
// Layout: C3DMeshStats retail offsets as in the landed SaveBinary 0x00A2EAA0 / GetHelperPoint 0x00A2E8C0:
// HelperPoints +0x38, DummyObjects +0x48, PackedNames +0x74.
extern "C" unsigned int __cdecl strlen(const char* text);
#pragma intrinsic(strlen)
extern "C" int __cdecl strncmp(const char* a, const char* b, unsigned int count);   // 0x00BFEAF8
inline void* operator new(unsigned int, void* where) { return where; }

unsigned long __fastcall Calc_004014a0(unsigned long seed, const void* data, unsigned long length);   // 0x004014A0: CRC

class CCharStringData { public: char* Text; long Length; };
class CCharString { public:
    CCharStringData* Data;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
    const char* GetText() const { return Data == 0 ? "" : Data->Text; }
    long GetLength() const { return Data == 0 ? 0 : Data->Length; }
};

struct __false_type { __false_type() {} };
struct CCharStringVector {                              // STLport vector<CCharString>
    CCharString* _M_start;
    CCharString* _M_finish;
    CCharString* _M_end_of_storage;
    void _M_insert_overflow(CCharString* position, const CCharString& x, const __false_type&,
                            unsigned long fill_len, bool atend);   // 0x00433530
    void push_back(const CCharString& x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) CCharString(x);
            ++_M_finish;
        }
        else
            _M_insert_overflow(_M_finish, x, __false_type(), 1, true);
    }
};

extern "C" void* __cdecl malloc(unsigned int size);
extern "C" void __cdecl free(void* p);
// Same-TU stand-in for the out-of-line STLport growth path (retail 0x00433530): it reads the value only through
// its const reference and never touches the empty tag, so VC7.1 may home the caller's temporaries in dead slots.
#pragma auto_inline(off)
void CCharStringVector::_M_insert_overflow(CCharString* position, const CCharString& x, const __false_type&,
                                           unsigned long fill_len, bool atend)
{
    unsigned long oldSize = (unsigned long)(_M_finish - _M_start);
    unsigned long len = oldSize + (oldSize > fill_len ? oldSize : fill_len);
    CCharString* start = (CCharString*)malloc(len * sizeof(CCharString));
    CCharString* finish = start;
    for (CCharString* p = _M_start; p != position; ++p, ++finish)
        new (finish) CCharString(*p);
    for (unsigned long n = 0; n < fill_len; ++n, ++finish)
        new (finish) CCharString(x);
    if (!atend)
        for (CCharString* q = position; q != _M_finish; ++q, ++finish)
            new (finish) CCharString(*q);
    for (CCharString* d = _M_start; d != _M_finish; ++d)
        d->~CCharString();
    free(_M_start);
    _M_start = start;
    _M_finish = finish;
    _M_end_of_storage = start + len;
}
#pragma auto_inline(on)

class C3DVector { public: float X, Y, Z; };
class CMatrix3x4 { public: float m_row[3][4]; };
struct SHelperPointEntry { unsigned long first; C3DVector Position; long BoneIndex; };        // 20 bytes
struct SDummyObjectEntry { unsigned long first; CMatrix3x4 Transform; long BoneIndex; };      // 56 bytes
struct CHelperPointMap {                   // CPreAllocVectorMap<unsigned long, C3DHelperPoint>
    bool Compare; SHelperPointEntry* Elements; unsigned long Size; bool Dirty;
    SHelperPointEntry* LowerBound(const unsigned long& key);   // 0x00A316B0
    SHelperPointEntry* UpperBound(const unsigned long& key);   // 0x00A31700
};
struct CDummyObjectMap {                   // CPreAllocVectorMap<unsigned long, C3DDummyObject>
    bool Compare; SDummyObjectEntry* Elements; unsigned long Size; bool Dirty;
    SDummyObjectEntry* LowerBound(const unsigned long& key);   // 0x00A31790
    SDummyObjectEntry* UpperBound(const unsigned long& key);   // 0x00A317E0
};

// Same-TU stand-ins for the out-of-line bound searches (they read the key only through the reference).
#pragma auto_inline(off)
SHelperPointEntry* CHelperPointMap::LowerBound(const unsigned long& key)
{
    SHelperPointEntry* first = Elements;
    unsigned long count = Size;
    while (count > 0) {
        unsigned long half = count >> 1;
        if (first[half].first < key) { first += half + 1; count -= half + 1; }
        else count = half;
    }
    return first;
}
SHelperPointEntry* CHelperPointMap::UpperBound(const unsigned long& key)
{
    SHelperPointEntry* first = Elements;
    unsigned long count = Size;
    while (count > 0) {
        unsigned long half = count >> 1;
        if (!(key < first[half].first)) { first += half + 1; count -= half + 1; }
        else count = half;
    }
    return first;
}
SDummyObjectEntry* CDummyObjectMap::LowerBound(const unsigned long& key)
{
    SDummyObjectEntry* first = Elements;
    unsigned long count = Size;
    while (count > 0) {
        unsigned long half = count >> 1;
        if (first[half].first < key) { first += half + 1; count -= half + 1; }
        else count = half;
    }
    return first;
}
SDummyObjectEntry* CDummyObjectMap::UpperBound(const unsigned long& key)
{
    SDummyObjectEntry* first = Elements;
    unsigned long count = Size;
    while (count > 0) {
        unsigned long half = count >> 1;
        if (!(key < first[half].first)) { first += half + 1; count -= half + 1; }
        else count = half;
    }
    return first;
}
#pragma auto_inline(on)

class C3DMeshStats { public:
    virtual void v00();
    unsigned char _pad_0x04[0x38 - 0x04];
    CHelperPointMap HelperPoints;          // +0x38
    CDummyObjectMap DummyObjects;          // +0x48
    unsigned char _pad_0x58[0x74 - 0x58];
    char* PackedNames;                     // +0x74
    unsigned short PackedNamesSize;        // +0x78
    bool AnimatedFlag;                     // +0x7A
    __forceinline long GetNoHelperPoints(const CCharString& name)
    {
        unsigned long crc = Calc_004014a0(0, name.GetText(), name.GetLength());
        return HelperPoints.UpperBound(crc) - HelperPoints.LowerBound(crc);
    }
    __forceinline long GetNoDummyObjects(const CCharString& name)
    {
        unsigned long crc = Calc_004014a0(0, name.GetText(), name.GetLength());
        return DummyObjects.UpperBound(crc) - DummyObjects.LowerBound(crc);
    }
    void UnpackNames(CCharStringVector& names, bool dummyObjects, const CCharString& prefix, bool once);
};

void C3DMeshStats::UnpackNames(CCharStringVector& names, bool dummyObjects, const CCharString& prefix, bool once)
{
    if (!PackedNames)
        return;
    const char* text = PackedNames;
    unsigned long dummyNamesOffset = *(const unsigned short*)text;
    text += sizeof(unsigned short);
    if (dummyObjects)
        text = PackedNames + dummyNamesOffset;
    long prefixLength = prefix.GetLength();
    unsigned long length;
    while ((length = strlen(text)) != 0) {
        if (prefixLength == 0 || strncmp(text, prefix.GetText(), prefixLength) == 0) {
            CCharString name(text);
            long count = 1;
            if (!once) {
                if (dummyObjects)
                    count = GetNoDummyObjects(name);
                else
                    count = GetNoHelperPoints(name);
            }
            for (long i = 0; i < count; ++i)
                names.push_back(name);
        }
        text += length + 1;
    }
}