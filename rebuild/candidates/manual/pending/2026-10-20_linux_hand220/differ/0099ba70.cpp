// CWideString::Format(CWideString* result, const wchar_t* format, ...) -- wide printf into a stack buffer of 8000 characters (or a heap vector of wcslen(format) + 4000 when that is larger); on success re-assigns the result from the text (left unassigned when empty)
// Grade: DIFFER(265v265) -- retail stores the vector's three zero words BEFORE the wcslen call yet still pushes the known-zero insert position and skips size(); ours either stores them after the call (vector declared after wcslen, this file) or reloads _M_finish/_M_start for resize (vector declared first); plus the va-arg push order (eax/edx vs ecx/eax) follows
// Flags: /GS
// Leaf: Format
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0099BA70, length 265 bytes (`__cdecl` variadic, plain `ret`; 0x3E94-byte
// frame through __chkstk 0x00BFEA30, /GS cookie 0x0139C8A8 checked by 0x00BFE9F9).
// Identity: the manifest label "CCharString::SetFromFormatV" is wrong: the body works on 16-bit characters and the
// 16-byte CWideStringData (+0xc NoRefs) and assigns through CWideString::AllocStringData 0x0099B3C0; it is the wide twin
// of the landed CCharString::Format_vsnprintf 0x0099F1F0. 54 callers pass (&local CWideString, L"...", args).
// Callees: wcslen 0x00BFED94, vector<wchar_t>::_M_fill_insert 0x005149F0 (manifest "vector_insert_erase"),
// _vsnwprintf 0x00BFED9A, free 0x00BFEA14, operator delete 0x00BFE9BC, CWideString::AllocStringData 0x0099B3C0.
// Shape: STLport vector (_Vector_base + _STLP_alloc_proxy) resized through insert(end(), n, wchar_t()); the
// release is UnassignString inline (data destructor frees the basic_string buffer).
#include <stdarg.h>
#include <string.h>    // wcslen 0x00BFED94
#include <stdio.h>     // _vsnwprintf 0x00BFED9A
#include <stdlib.h>    // free 0x00BFEA14
void __cdecl operator delete(void* block);                                                             // 0x00BFE9BC

template <class T> struct allocator { allocator() { } };
template <class T> struct _STLP_alloc_proxy : allocator<T> {
    T* _M_data;
    _STLP_alloc_proxy(const allocator<T>& a, T* p) : allocator<T>(a), _M_data(p) { }
};
template <class T> struct _Vector_base {
    T* _M_start;
    T* _M_finish;
    _STLP_alloc_proxy<T> _M_end_of_storage;
    _Vector_base(const allocator<T>& a) : _M_start(0), _M_finish(0), _M_end_of_storage(a, 0) { }
    ~_Vector_base() { if (_M_start) free(_M_start); }
};
template <class T> struct vector : _Vector_base<T> {
    vector() : _Vector_base<T>(allocator<T>()) { }
    unsigned int size() const { return (unsigned int)(this->_M_finish - this->_M_start); }
    void _M_fill_insert(T* pos, unsigned int n, const T& x);       // 0x005149F0
    void insert(T* pos, unsigned int n, const T& x) { _M_fill_insert(pos, n, x); }
    void resize(unsigned int new_size, T x = T()) { insert(this->_M_finish, new_size - size(), x); }
};

struct CWideStringData {                      // 16 bytes: STLport basic_string<wchar_t> + owner count
    wchar_t* _M_start;
    wchar_t* _M_finish;
    wchar_t* _M_end_of_storage;
    long            NoRefs;                   // +0x0c
    ~CWideStringData() { if (_M_start) free(_M_start); }
};

class CWideString {
public:
    CWideStringData* PStringData;
    CWideStringData* AllocStringData(const wchar_t* text, long length);   // 0x0099B3C0
    void UnassignString()
    {
        CWideStringData* data = PStringData;
        if (data) {
            if (--data->NoRefs <= 0) {
                CWideStringData* p = PStringData;
                if (p)
                    delete p;
            }
            PStringData = 0;
        }
    }
    static void __cdecl Format(CWideString* result, const wchar_t* format, ...);
};

void __cdecl CWideString::Format(CWideString* result, const wchar_t* format, ...)
{
    wchar_t localBuffer[8000];
    long size = wcslen(format);
    vector<wchar_t> heapBuffer;
    wchar_t* buffer;
    if (size > 4000) {
        heapBuffer.resize(size + 4000);
        buffer = heapBuffer._M_start;
    } else
        buffer = localBuffer;
    size += 4000;
    va_list args;
    va_start(args, format);
    int count = _vsnwprintf(buffer, size - 1, format, args);
    buffer[size - 1] = 0;
    if (count <= size) {
        result->UnassignString();
        if (buffer && *buffer)
            result->PStringData = result->AllocStringData(buffer, -1);
    }
}
