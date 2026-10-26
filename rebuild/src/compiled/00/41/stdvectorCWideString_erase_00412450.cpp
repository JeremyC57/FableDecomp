// std::vector<CWideString>::erase(CWideString* first, CWideString* last) -- STLport range erase: assign the tail down over [first, last), destroy the vacated strings, shrink _M_finish, return first
// Leaf: ?erase@?$CRetailVector@VCWideString@@@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00412450, 104 bytes (+8 int3 pad). Callees:
// CWideString::operator=(const CWideString&) 0x0099B7D0, CWideString::~CWideString 0x0099B510.
//
// Retail bytes:  push ebx ; push ebp ; push esi ; mov ebp,ecx ; mov eax,[ebp+4] ; mov ecx,[first] ;
//   push edi ; mov edi,[last] ; sub eax,edi ; sar eax,2 ; test eax,eax ; mov esi,ecx ; jle D ;
//   sub edi,ecx ; mov ebx,eax ; L: lea eax,[edi+esi] ; push eax ; mov ecx,esi ; call operator= ;
//   add esi,4 ; dec ebx ; jne L ; mov ecx,[first] ; D: mov ebx,[ebp+4] ; cmp esi,ebx ; mov edi,esi ;
//   je E ; K: mov ecx,edi ; call ~CWideString ; add edi,4 ; cmp edi,ebx ; jne K ;
//   mov eax,[first] ; pop edi ; mov [ebp+4],esi ; ... ret 8 ; E: pop edi ; mov [ebp+4],esi ; ...
//   mov eax,ecx ; ret 8
// STLport: `iterator erase(iterator first, iterator last) { pointer i = copy(last, _M_finish,
// first); _Destroy(i, _M_finish); _M_finish = i; return first; }` with the random-access __copy
// (count `_M_finish - last` once, element operator= out of line; VC re-expresses the source as
// destination + (last - first)) and the _Destroy loop inlined. The two helpers must be
// __forceinline: as plain `inline` templates VC7.1 keeps the copy loop out of line (a 67-byte
// erase calling it), retail has both expanded. The manifest name
// Std_Vector_Erase_Range is descriptive; landed callers (CFrontendProfileList::Refresh 0x00495830,
// CFrontendGameComponent::GetWorldNames 0x00495890) already call this as vector<CWideString>::erase.
class CWideStringData;

class CWideString {
public:
    ~CWideString();                               // 0x0099B510
    CWideString& operator=(const CWideString& other);   // 0x0099B7D0
private:
    CWideStringData* PStringData;
};

template <class T>
__forceinline T* RetailCopy(T* first, T* last, T* result)    // STLport __copy, random-access form
{
    for (int n = int(last - first); n > 0; --n) {
        *result = *first;
        ++first;
        ++result;
    }
    return result;
}

template <class T>
__forceinline void RetailDestroy(T* first, T* last)          // STLport _Destroy range
{
    for (; first != last; ++first)
        first->~T();
}

template <class T>
class CRetailVector {                             // STLport std::vector<T> storage
public:
    T* _M_start;                                  // +0
    T* _M_finish;                                 // +4
    T* _M_end_of_storage;                         // +8
    T* erase(T* first, T* last);
};

template <class T>
T* CRetailVector<T>::erase(T* first, T* last)
{
    T* i = RetailCopy(last, _M_finish, first);
    RetailDestroy(i, _M_finish);
    _M_finish = i;
    return first;
}

template class CRetailVector<CWideString>;