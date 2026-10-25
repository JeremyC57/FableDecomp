// CMemoryDataOutputStream::Clear() -- empty the byte buffer and release its storage (only when it has any capacity), then rewind the write position to 0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00996B60, length 83 bytes (padded to 96).
// Leaf: ?Clear@CMemoryDataOutputStream@@QAEXXZ
// Label correction: the catalog "CMemoryDataOutputStream::Reset" and the manifest "_Dest_val<..., std::pair<std::pair<long,long>,
// CEngineTextureDecal>>" are wrong: the PDBs name no Reset on this class.
// Ego_r.pdb lists ?Clear@CMemoryDataOutputStream@@QAEXXZ; the body works on Data (retail CArray<char>, three words at
// +8/+0xc/+0x10) and CurrentPosition (+0x14) -- the retail layout the landed constructor 0x00996AF0 stores and
// engine/CMemoryDataOutputStream.h already cites this row for. Callers: CWorld::SaveGameStateInternal 0x0049FDB6,
// 0x009AE592, 0x00ADFF95, 0x00AE0005, 0x00AE3409, 0x00AE3C91, and the thunk 0x00996BD0 (`mov [ecx+18h],0 ; jmp`).
// Callees: memmove @ 0x00BFEAE6, free @ 0x00BFEA14 (statically linked CRT).
// Layout: engine/CMemoryDataOutputStream.h (FABLE_RETAIL_MEMORY_OUTPUT_STREAM, 24 bytes) declares no member functions, and a
// method-only carrier struct mangles the body as ?Clear@CMemoryDataOutputStream_Methods@@, so the class is modelled locally
// with the header's retail offsets (Data +0x08 as the STLport vector model, CurrentPosition +0x14).
// Shape: CArray::Clear = `if (capacity()) { clear(); swap with an empty array; }`: the STLport `erase(begin(), end())`
// keeps __copy_trivial's unfolded `last == first` test (`cmp ecx,ecx ; je`, else memmove of 0 bytes), the swap with
// the empty temporary zeroes the three words through one zero register, and the temporary's `_Vector_base`
// destructor frees the old block on a null test. The zero-capacity path is a separate `mov [esi+14h],0 ; ret` exit.
extern "C" void __cdecl free(void* p);                                           // 0x00BFEA14
extern "C" void* __cdecl memmove(void* dst, const void* src, unsigned int n);   // 0x00BFEAE6
namespace std {
inline void* __copy_trivial(const void* first, const void* last, void* result)
{
    return (last == first) ? result
        : ((char*)memmove(result, first, ((const char*)last - (const char*)first))) + ((const char*)last - (const char*)first);
}
template <class T> inline void swap(T& a, T& b) { T tmp = a; a = b; b = tmp; }
template <class T> struct allocator {
    void deallocate(T* p, unsigned int) { free(p); }
};
template <class T, class A> struct _STLP_alloc_proxy : A {
    T _M_data;
};
template <class T> struct _Vector_base {
    T* _M_start;
    T* _M_finish;
    _STLP_alloc_proxy<T*, allocator<T> > _M_end_of_storage;
    _Vector_base() : _M_start(0), _M_finish(0) { _M_end_of_storage._M_data = 0; }
    ~_Vector_base()
    {
        if (_M_start != 0)
            _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start);
    }
};
template <class T> struct vector : _Vector_base<T> {
    unsigned int capacity() const { return this->_M_end_of_storage._M_data - this->_M_start; }
    T* erase(T* first, T* last)
    {
        T* i = (T*)__copy_trivial(last, this->_M_finish, first);
        this->_M_finish = i;
        return first;
    }
    void clear() { erase(this->_M_start, this->_M_finish); }
    void swap(vector& x)
    {
        std::swap(this->_M_start, x._M_start);
        std::swap(this->_M_finish, x._M_finish);
        std::swap(this->_M_end_of_storage._M_data, x._M_end_of_storage._M_data);
    }
};
}
template <class T> struct CArray : std::vector<T> {
    void Clear()
    {
        if (this->capacity() != 0) {
            this->clear();
            CArray<T> empty;
            this->swap(empty);
        }
    }
};
class CMemoryDataOutputStream {
public:
    void*         _vftable;                     // +0x00 (CDataOutputStream vptr)
    void*         NewLine;                      // +0x04 CDataOutputStream::NewLine (CCharString)
    CArray<char>  Data;                         // +0x08
    unsigned long CurrentPosition;              // +0x14
    void Clear();
};
void CMemoryDataOutputStream::Clear()
{
    Data.Clear();
    CurrentPosition = 0;
}