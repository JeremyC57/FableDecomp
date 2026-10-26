// CWideString::operator<(const CWideString& other) const -- wide-string ordering: wcscmp of the two texts is negative (unassigned strings read as the shared empty string)
// Leaf: ??MCWideString@@QBE_NABV0@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0099AF70, 56 bytes (+ int3 padding), thiscall `ret 4`.
// Identity: the landed STLport map<CWideString, ...> walks (_M_lower_bound 0x00A609C0, _M_find 0x00A608F0, _M_insert
//   0x00A61270) call this address as CWideString::operator< (less<CWideString> inline); it sits in the CWideString TU
//   after Contains 0x0099AF20. The catalog label "std::_Iterator_base12::_Getcont" is propagated and wrong.
// Callee: wcscmp @ 0x00BFE94C (MSVCR71 thunk, cdecl).
// Shape: `wcscmp(GetAsCharArray(), other.GetAsCharArray()) < 0` with both accessors inline (null storage -> the empty
//   wide literal 0x0129A8E0), the arguments evaluated right to left; the bool is `setl cl ; mov al,cl`.
#include <stddef.h>
extern "C" int __cdecl wcscmp(const wchar_t* left, const wchar_t* right);   // 0x00BFE94C

class CWideStringData { public:
    wchar_t*      text;                         // +0x00
    unsigned long unknown04;
    unsigned long unknown08;
    long          owners;
};
extern wchar_t g_FableEmptyWideString_0129A8E0[1];   // 0x0129A8E0

class CWideString {
public:
    const wchar_t* GetAsCharArray() const
    {
        if (storage_ == 0)
            return g_FableEmptyWideString_0129A8E0;
        return storage_->text;
    }
    bool operator<(const CWideString& other) const;
private:
    CWideStringData* storage_;                  // +0x00
};

bool CWideString::operator<(const CWideString& other) const
{
    return wcscmp(GetAsCharArray(), other.GetAsCharArray()) < 0;
}