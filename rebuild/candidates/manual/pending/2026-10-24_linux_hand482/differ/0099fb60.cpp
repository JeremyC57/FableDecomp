// GFIntVectorToCharString(const std::vector<long>& values) -- join a list of integers for display: "1", "1 and 2", "1, 2 and 3"
// Grade: DIFFER(390v428) -- retail inlines UnassignString only into the result's final destructor (keeping result's block in edi); this source calls it everywhere, the visible-body variant inlines it everywhere (620v428), same under QFE 4035
// Leaf: ?GFIntVectorToCharString@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x0099FB60, 428 bytes (incl. padding). FableWin.pdb
//   `?GFIntVectorToCharString@@YI?AVCCharString@@ABV?$vector@JV?$allocator@J@std@@@std@@@Z`: a `__fastcall` free function
//   returning CCharString (hidden result in ecx, the vector in edx), plain `ret`. The catalog's "GFIntToCharString_JoinList"
//   is descriptive.
// Callees: GFIntToCharString 0x0099F830, CCharString::UnassignString 0x0099E9B0, CCharString::operator+=(const
//   CCharString&) 0x0099F0A0, CCharString::operator+=(const char*) 0x0099F100, operator delete[] 0x00BFEB1C, operator
//   delete 0x00BFE9BC. Globals: g_CCharStringInstanceCount 0x013BD800. Literals ", " 0x012397CC, " and " 0x0129AB1C.
// Shape: the CCharString translation unit, so the class's bodies are visible and VC7.1 /O2 auto-inlines them: the
//   default constructor (null block, counter++), operator= and the copy constructor (self test, add a reference),
//   ~CCharString = UnassignString() + counter--; the temporaries' UnassignString stays a call while the result's
//   destructor inlines it (block release: delete[] the text, clear the length and the 31-bit capacity, delete the block).
//   This source declares UnassignString only (every site a call); with its body visible (probe 0099fb60_g1) VC7.1
//   inlines it at all seven sites (620 bytes), so retail's one-site inline decision is not reproduced yet.
extern long g_CCharStringInstanceCount;                 // 0x013BD800

#pragma pack(push, 1)
class CBasicStringChar                                  // CBasicString<char>, 13 bytes
{
public:
    char*         PData;                                // +0x00
    unsigned long StringLength;                         // +0x04
    unsigned long DataLength    : 31;                   // +0x08 bits 0..30
    unsigned long m_reserved    : 1;                    // +0x08 bit 31
    unsigned char UseFastExtend : 1;                    // +0x0c bit 0
    ~CBasicStringChar()
    {
        if (PData != 0) {
            delete[] PData;
            PData = 0;
        }
        StringLength = 0;
        DataLength = 0;
    }
};
class CCharStringData                                   // 17 bytes
{
public:
    CBasicStringChar Data;                              // +0x00
    long             NoRefs;                            // +0x0d
};
#pragma pack(pop)

class CCharString
{
public:
    CCharStringData* PStringData;                       // +0x00
    CCharString() : PStringData(0) { ++g_CCharStringInstanceCount; }
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString() { UnassignString(); --g_CCharStringInstanceCount; }
    CCharString& operator=(const CCharString& other);  // 0x0099EFB0
    CCharString& operator+=(const CCharString& other); // 0x0099F0A0
    CCharString& operator+=(const char* text);          // 0x0099F100
    void UnassignString();                              // 0x0099E9B0
};

CCharString::CCharString(const CCharString& other)
    : PStringData(0)
{
    ++g_CCharStringInstanceCount;
    *this = other;
}

CCharString& CCharString::operator=(const CCharString& other)
{
    if (&other != this) {
        if (PStringData != 0)
            UnassignString();
        if (other.PStringData != 0) {
            PStringData = other.PStringData;
            ++PStringData->NoRefs;
        }
    }
    return *this;
}

CCharString __fastcall GFIntToCharString(long value);  // 0x0099F830

namespace std {
template <class _Tp> class allocator { public: allocator() { } };
template <class _Tp, class _Alloc = allocator<_Tp> > class vector
{
public:
    _Tp* _M_start;                                      // +0x00
    _Tp* _M_finish;                                     // +0x04
    _Tp* _M_end_of_storage;                             // +0x08
    typedef unsigned int size_type;
    const _Tp* begin() const { return _M_start; }
    const _Tp* end() const { return _M_finish; }
    size_type size() const { return size_type(end() - begin()); }
    bool empty() const { return begin() == end(); }
    const _Tp& operator[](size_type __n) const { return *(begin() + __n); }
};
} // namespace std

CCharString __fastcall GFIntVectorToCharString(const std::vector<long>& values)
{
    CCharString result;
    if (!values.empty()) {
        if (values.size() == 1)
            result = GFIntToCharString(values[0]);
        else {
            for (long i = values.size(); i > 2; --i) {
                result += GFIntToCharString(values[values.size() - i]);
                result += ", ";
            }
            result += GFIntToCharString(values[values.size() - 2]);
            result += " and ";
            result += GFIntToCharString(values[values.size() - 1]);
        }
    }
    return result;
}
