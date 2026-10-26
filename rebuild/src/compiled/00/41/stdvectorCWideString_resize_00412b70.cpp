// std::vector<CWideString>::resize(unsigned int newSize) -- STLport resize with a default value: erase the excess tail or fill-insert copies of an empty CWideString at the end
// Leaf: ?resize@?$CRetailVector@VCWideString@@@@QAEXI@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00412B70, 99 bytes. Callees: CWideString::CWideString()
// 0x0099AED0, vector<CWideString>::erase(first, last) 0x00412450, vector<CWideString>::insert(pos, n,
// const T&) 0x004126E0 (STLport _M_fill_insert; manifest label vector_resize_with_fill),
// CWideString::~CWideString 0x0099B510.
//
// Retail bytes:  push ecx ; push ebx ; push esi ; mov esi,ecx ; push edi ; lea ecx,[tmp] ;
//   call CWideString() ; mov edi,[esi+4] ; mov edx,[esi] ; mov ecx,[newSize] ; mov ebx,edi ;
//   sub ebx,edx ; sar ebx,2 ; cmp ecx,ebx ; jae G ; lea eax,[edx+ecx*4] ; push edi ; push eax ;
//   mov ecx,esi ; call erase ; ~tmp ; ... ret 4 ; G: mov edi,[esi+4] ; push eax (&tmp, the ctor's
//   returned this) ; mov eax,edi ; sub eax,edx ; sar eax,2 ; sub ecx,eax ; push ecx ; push edi ;
//   mov ecx,esi ; call insert ; ~tmp ; ... ret 4
// STLport: `void resize(size_type n) { resize(n, _Tp()); }` over the inline `resize(n, x) { if (n <
// size()) erase(begin() + n, end()); else insert(end(), n - size(), x); }`. The default value is a
// temporary built first and destroyed in both arms. The manifest name Std_Vector_Resize is
// descriptive; the element is fixed by the CWideString default ctor/dtor and the landed erase.
class CWideStringData;

class CWideString {
public:
    CWideString();                                // 0x0099AED0
    ~CWideString();                               // 0x0099B510
private:
    CWideStringData* PStringData;
};

template <class T>
class CRetailVector {                             // STLport std::vector<T> storage
public:
    T* _M_start;                                  // +0
    T* _M_finish;                                 // +4
    T* _M_end_of_storage;                         // +8
    T* begin() { return _M_start; }
    T* end() { return _M_finish; }
    unsigned int size() const { return unsigned int(_M_finish - _M_start); }
    T* erase(T* first, T* last);                                  // 0x00412450
    void insert(T* position, unsigned int n, const T& value);     // 0x004126E0
    void resize(unsigned int newSize, const T& value)
    {
        if (newSize < size())
            erase(begin() + newSize, end());
        else
            insert(end(), newSize - size(), value);
    }
    void resize(unsigned int newSize);
};

template <class T>
void CRetailVector<T>::resize(unsigned int newSize)
{
    resize(newSize, T());
}

template void CRetailVector<CWideString>::resize(unsigned int newSize);