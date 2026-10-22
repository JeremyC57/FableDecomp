// Grade: DIFFER(610v609) -- structure right (two inline appends, an out-of-line operator+= for the tail, inline assignment and destructors with the instance counter), register/frame allocation off: retail keeps result.PStringData in ebp and the prefix data in ebx, homes the search text in the dead `what` argument slot, and gives AllocStringData an empty-object `this` in the `with` slot (ours homes the prefix and passes its address)
// CWideString::Replace(const CWideString& what, const CWideString& with) -- replace every occurrence of `what` with `with`
// Leaf: ?Replace@CWideString@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x0099C1E0, 609 bytes, ret 8; PDB ?Replace@CWideString@@QAEXABV1@0@Z. LABEL: the catalogue
//   row says CCharString::ReplaceSubstring; the body is the wide-string Replace (wcsstr/wcslen, CWideStringData NoRefs +0xc).
// Callees: wcsstr 0x00BFEB2E, wcslen 0x00BFED94, CWideString AllocStringData 0x0099B3C0 / MakeOwnCopyOfData 0x0099B560 /
//   (const wchar_t*) ctor 0x0099B6B0 / operator+= 0x0099B8D0 (landed) / UnassignString 0x0099B4D0, basic_string<wchar_t>::_M_append
//   0x0099C7E0, free 0x00BFEA14, operator delete 0x00BFE9BC; instance counter 0x013BCA20, empty text 0x0129A8E0.
// Next: model the substring constructor so AllocStringData is not called through the prefix's own address; loop order is
//   prefix, found += wcslen(what), two appends, source = found, wcsstr(found, what).
#include <stddef.h>
extern "C" wchar_t* __cdecl wcsstr(const wchar_t* s, const wchar_t* sub);   // 0x00BFEB2E
extern "C" size_t __cdecl wcslen(const wchar_t* s);                          // 0x00BFED94
#pragma function(wcslen)
extern "C" void __cdecl free(void* p);                                       // 0x00BFEA14
void __cdecl operator delete(void* p);                                       // 0x00BFE9BC
extern wchar_t g_FableEmptyWideString[];   // 0x0129A8E0
extern long g_CWideStringInstanceCount;    // 0x013BCA20
struct forward_iterator_tag { forward_iterator_tag() {} };
static inline size_t WideLength(const wchar_t* s) { size_t n = 0; while (s[n] != 0) ++n; return n; }
struct CWideBasicString {
    wchar_t* _M_start;
    wchar_t* _M_finish;
    wchar_t* _M_end_of_storage;
    CWideBasicString& _M_append(const wchar_t* first, const wchar_t* last, const forward_iterator_tag& tag);   // 0x0099C7E0
    CWideBasicString& append(const wchar_t* first, const wchar_t* last) { return _M_append(first, last, forward_iterator_tag()); }
    CWideBasicString& append(const wchar_t* s) { return append(s, s + WideLength(s)); }
};
struct CWideStringData {
    CWideBasicString Data;
    long NoRefs;
};
struct CWideString {
    CWideStringData* PStringData;
    CWideString() { ++g_CWideStringInstanceCount; PStringData = 0; }
    CWideString(const wchar_t* text);                  // 0x0099B6B0
    CWideString(const wchar_t* text, long length)
    {
        ++g_CWideStringInstanceCount;
        PStringData = 0;
        if (text != 0 && *text != 0)
            PStringData = AllocStringData(text, length);
    }
    ~CWideString()
    {
        if (PStringData != 0) {
            if (--PStringData->NoRefs <= 0) {
                if (PStringData->Data._M_start)
                    free(PStringData->Data._M_start);
                delete PStringData;
            }
        }
        --g_CWideStringInstanceCount;
    }
    void MakeOwnCopyOfData();                          // 0x0099B560
    void UnassignString();                             // 0x0099B4D0
    CWideStringData* AllocStringData(const wchar_t* text, long length);   // 0x0099B3C0
    const wchar_t* GetText() const { return PStringData == 0 ? g_FableEmptyWideString : PStringData->Data._M_start; }
    const CWideString& operator+=(const CWideString& other);   // 0x0099B8D0
    __forceinline void Append(const CWideString& other)
    {
        if (PStringData != 0) {
            MakeOwnCopyOfData();
            PStringData->Data.append(other.GetText());
        } else if (&other != this && other.PStringData != 0) {
            PStringData = other.PStringData;
            ++PStringData->NoRefs;
        }
    }
    CWideString& operator=(const CWideString& other)
    {
        if (&other != this && PStringData != other.PStringData) {
            if (PStringData != 0)
                UnassignString();
            if (other.PStringData != 0) {
                PStringData = other.PStringData;
                ++PStringData->NoRefs;
            }
        }
        return *this;
    }
    void Replace(const CWideString& what, const CWideString& with);
};

void CWideString::Replace(const CWideString& what, const CWideString& with)
{
    CWideString result;
    const wchar_t* source = GetText();
    const wchar_t* whatText = what.GetText();
    const wchar_t* found = wcsstr(source, whatText);
    while (found != 0) {
        CWideString prefix(source, found - source);
        source = found + wcslen(whatText);
        result.Append(prefix);
        result.Append(with);
        found = wcsstr(source, whatText);
    }
    result += CWideString(source);
    *this = result;
}
