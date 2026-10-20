// Behaviour test for 00a623d0: src/00a623d0.cpp copied verbatim (the function body and its same-TU stand-ins), the remaining
// declared callees stubbed below so calls and arguments are observable. CShaderDataBank::NotifyEntryAdded(unsigned long type, unsigned long index, const CArray<char>& data)
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
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
template <class T> struct CArray;
struct CShaderDataBank;

struct __true_type { __true_type() { } };

class CShaderBankEntry {
public:
    CShaderBankEntry(unsigned long type, unsigned long index, CShaderDataBank* bank);   // 0x00A62120
    unsigned char _body[0x24];
};

template <class T> struct CArray {                          // retail CArray (STLport vector): 12 bytes
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

// ---- stubs and driver ----
static CShaderBankEntry* g_store[8]; static int g_grow = 0, g_ctor = 0, g_base = 0;
static unsigned long g_ctorType = 99, g_ctorIndex = 99, g_baseType = 99, g_baseIndex = 99; static CShaderDataBank* g_ctorBank = 0; static const CArray<char>* g_baseData = 0;
void* GrowVectorStorage_s7(void* vector, unsigned long count)
{
    ++g_grow; CArray<CShaderBankEntry*>* v = (CArray<CShaderBankEntry*>*)vector; CShaderBankEntry** p = v->_M_finish; v->_M_finish += count; if (v->_M_end_of_storage < v->_M_finish) v->_M_end_of_storage = v->_M_finish; return p;
}
CShaderBankEntry::CShaderBankEntry(unsigned long type, unsigned long index, CShaderDataBank* bank) { ++g_ctor; g_ctorType = type; g_ctorIndex = index; g_ctorBank = bank; _body[0] = (unsigned char)(0x40 + index); }
void CBankFile::NotifyEntryAdded(unsigned long type, unsigned long index, const CArray<char>& data) { ++g_base; g_baseType = type; g_baseIndex = index; g_baseData = &data; }
struct TBank : CShaderDataBank {
    void Call(unsigned long type, unsigned long index, const CArray<char>& data) { CShaderDataBank::NotifyEntryAdded(type, index, data); }
    CArray<CShaderBankEntry*>& V() { return Shaders; }
};
int main()
{
    TBank b; b.V()._M_start = g_store; b.V()._M_finish = g_store; b.V()._M_end_of_storage = g_store + 2;
    CArray<char> data; data._M_start = data._M_finish = data._M_end_of_storage = 0;
    g_store[2] = (CShaderBankEntry*)1;
    b.Call(0, 2, data);    // grows 0..2 (third push_back through the overflow path)
    CHECK(b.V().size() == 3); CHECK(g_grow == 1); CHECK(g_store[0] == 0 && g_store[1] == 0);
    CHECK(g_ctor == 1 && g_ctorType == 0 && g_ctorIndex == 2 && g_ctorBank == (CShaderDataBank*)&b); CHECK(g_store[2] != 0 && g_store[2]->_body[0] == 0x42);
    CHECK(g_base == 1 && g_baseType == 0 && g_baseIndex == 2 && g_baseData == &data);
    b.Call(1, 0, data);    // in range, pixel shader: entry created
    CHECK(b.V().size() == 3 && g_ctor == 2 && g_ctorType == 1 && g_store[0] != 0 && g_store[0]->_body[0] == 0x40 && g_base == 2 && g_baseType == 1);
    b.Call(5, 2, data);    // other type: the slot is cleared
    CHECK(g_ctor == 2 && g_store[2] == 0 && g_base == 3 && g_baseType == 5 && g_baseIndex == 2);
    b.V()._M_end_of_storage = g_store + 8;
    b.Call(7, 3, data);    // one push without growth
    CHECK(b.V().size() == 4 && g_grow == 1 && g_store[3] == 0 && g_ctor == 2 && g_base == 4);
    if (fails == 0) printf("00a623d0_TEST PASS\n"); else printf("00a623d0_TEST FAIL\n");
    return fails;
}