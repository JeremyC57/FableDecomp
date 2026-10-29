// NGameText::ConstructString(CWideString& output, const vector<CWideString>& replacements) -- replace %1..%N of a game-text string with the strings of a vector; false if any placeholder is missing
// Leaf: ?ConstructString@NGameText@@YI_NAAVCWideString@@ABV?$vector@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009C7C20, 211 bytes (+ int3 padding), __fastcall (output in ecx, the
// vector in edx, plain `ret`).
// Name: FableWin.pdb `?ConstructString@NGameText@@YI_NAAVCWideString@@ABV?$vector@VCWideString@@...@Z`.
// Callees: CWideString::CWideString() @ 0x0099AED0, CWideString::Sprintf(const unsigned short*, ...) @ 0x0099BA70
// (__cdecl, L"%d" @ 0x0122DE44), CWideString::CWideString(const CWideString&) @ 0x0099B720,
// CWideString::~CWideString @ 0x0099B510, operator+(const unsigned short*, const CWideString&) @ 0x0099BFF0
// (__fastcall, hidden result in ecx, L"%" @ 0x0129B470), CWideString::Contains @ 0x0099AF20,
// CWideString::Replace @ 0x0099C1E0.
// Shape: the key is built as in the landed ConstructStringInternal (0x009C7A60, same TU): L"%" + FormatIndex(i + 1),
// where the inline FormatIndex formats into a named local and returns it by value (no NRVO: default ctor, Format,
// copy into the temporary, destroy). `i != size()` gives the `test eax,0FFFFFFFCh` entry test and the `jne`
// bottom test; i + 1 is computed once into ebp and becomes the next i. A missing placeholder only sets the
// `failed` byte (frame +3) and the loop goes on; `operator[]` is STLport's `*(begin() + n)`.

class CWideString {
public:
    CWideString();                                                              // 0x0099AED0
    CWideString(const CWideString& other);                                      // 0x0099B720
    ~CWideString();                                                             // 0x0099B510
    void __cdecl Sprintf(const unsigned short* format, ...);                     // 0x0099BA70
    bool Contains(const CWideString& text) const;                               // 0x0099AF20
    void Replace(const CWideString& search, const CWideString& replacement);    // 0x0099C1E0
private:
    void* PData;
};

CWideString __fastcall operator+(const unsigned short* left, const CWideString& right);   // 0x0099BFF0

namespace std {

template <class _Tp> class allocator { };

template <class _Tp, class _Alloc = allocator<_Tp> > class vector {
public:
    typedef unsigned int size_type;
    typedef const _Tp* const_iterator;
    const_iterator begin() const { return _M_start; }
    const_iterator end() const { return _M_finish; }
    size_type size() const { return size_type(end() - begin()); }
    const _Tp& operator[](size_type __n) const { return *(begin() + __n); }
protected:
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
};

}

namespace NGameText {

extern const unsigned short g_PlaceholderPrefix[];     // L"%"  @ 0x0129B470
extern const unsigned short g_NumberFormat[];          // L"%d" @ 0x0122DE44

__forceinline CWideString FormatIndex(int index)
{
    CWideString number;
    number.Sprintf(g_NumberFormat, index);
    return number;
}

bool __fastcall ConstructString(CWideString& output, const std::vector<CWideString>& replacements)
{
    bool failed = false;
    for (unsigned int i = 0; i != replacements.size(); ++i) {
        CWideString key = g_PlaceholderPrefix + FormatIndex(i + 1);
        if (output.Contains(key))
            output.Replace(key, replacements[i]);
        else
            failed = true;
    }
    if (failed)
        return false;
    return true;
}

}