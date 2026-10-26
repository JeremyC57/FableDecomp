// CShaderDataBank::NotifyEntryAdded(unsigned long type, unsigned long index, const CArray<char>& data) -- grow the shader table to cover the new entry, create the entry for vertex/pixel shader types, then let the bank file record it
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A623D0, 174 bytes, `ret 0Ch` (protected virtual, PDB
//   `?NotifyEntryAdded@CShaderDataBank@@MAEXKKABV?$CArray@D@@@Z`).
// Leaf: ?NotifyEntryAdded@CShaderDataBank
// Callees: STLport vector<CShaderBankEntry*>::_M_insert_overflow @ 0x00A62740 (manifest "StdVector_InsertN_Shader"),
//   operator new @ 0x00BFEA1A, CShaderBankEntry::CShaderBankEntry(unsigned long type, unsigned long index,
//   CShaderDataBank* bank) @ 0x00A62120 (0x24 bytes in retail), CBankFile::NotifyEntryAdded @ 0x009CBC20
//   (qualified, non-virtual base call).
// Shape: `while (index >= Shaders.size()) Shaders.push_back(0);` with STLport's push_back inlined: the null
//   value is a conversion temporary bound to `const T&`, homed once in the dead `index` argument slot (index
//   itself lives in ebx), stored in place as an immediate when there is room (`add [esi+4],edi` with the
//   element size hoisted into edi), else handed with the `__true_type` tag to _M_insert_overflow. Only shader
//   types 0 and 1 get an entry; anything else stores a null pointer (the `xor eax,eax` before the type test).
// Retail layout: Shaders +0x110 (CBankFile base 0x110 bytes; engine/CShaderDataBank.h carries the PDB +0x174,
//   so a local model is used, as in the landed CShaderDataBank::CloseShaders 0x00A62050).
template <class T> class CArray;
class CShaderDataBank;

struct __true_type { __true_type() { } };

class CShaderBankEntry {
public:
    CShaderBankEntry(unsigned long type, unsigned long index, CShaderDataBank* bank);   // 0x00A62120
    unsigned char _body[0x24];
};

template <class T> class CArray { public:                          // retail CArray (STLport vector): 12 bytes
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    unsigned long size() const { return _M_finish - _M_start; }
    T* begin() { return _M_start; }
    T& operator[](unsigned long n) { return *(begin() + n); }
    void _M_insert_overflow(T* pos, const T& x, const __true_type& tag, unsigned long fill_len, bool atend);   // 0x00A62740
    void push_back(const T& x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) T(x);
            ++_M_finish;
        }
        else
            _M_insert_overflow(_M_finish, x, __true_type(), 1, true);
    }
};
void* GrowVectorStorage_s7(void* vector, unsigned long count);
#pragma auto_inline(off)
// Same-TU body of the vector growth template (_M_insert_overflow, instantiated in retail's TU): reads the value through its const
// reference into the new storage and never touches the empty tag (the landed push_back idiom).
template <class T>
__declspec(noinline) void CArray<T>::_M_insert_overflow(T* pos, const T& x, const __true_type& tag, unsigned long fill_len, bool atend)
{
    T* p = (T*)GrowVectorStorage_s7(this, fill_len);
    if (p)
        *p = x;
    
}
#pragma auto_inline(on)

inline void* operator new(unsigned int, void* p) { return p; }

class CBankFile {
protected:
    virtual void NotifyEntryAdded(unsigned long type, unsigned long index, const CArray<char>& data);   // 0x009CBC20
    unsigned char _body[0x10c];
};

class CShaderDataBank : public CBankFile {
protected:
    CArray<CShaderBankEntry*> Shaders;                      // +0x110
    virtual void NotifyEntryAdded(unsigned long type, unsigned long index, const CArray<char>& data);
};

void CShaderDataBank::NotifyEntryAdded(unsigned long type, unsigned long index, const CArray<char>& data)
{
    while (index >= Shaders.size())
        Shaders.push_back(0);
    CShaderBankEntry* entry = 0;
    if (type == 0 || type == 1)
        entry = new CShaderBankEntry(type, index, this);
    Shaders[index] = entry;
    CBankFile::NotifyEntryAdded(type, index, data);
}