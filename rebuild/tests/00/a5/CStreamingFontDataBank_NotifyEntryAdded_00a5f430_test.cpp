// Behaviour test for 00a5f430: src/00a5f430.cpp copied verbatim (the function body and its same-TU stand-ins), the remaining
// declared callees stubbed below so calls and arguments are observable. CStreamingFontDataBank::NotifyEntryAdded(unsigned long type, unsigned long index, const CArray<char>& data)
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CStreamingFontDataBank::NotifyEntryAdded(unsigned long type, unsigned long index, const CArray<char>& data) -- grow the font table to cover the new entry, create a streaming font entry for font data types 0..2, then let the bank file record it
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A5F430, 213 bytes, `ret 0Ch` (protected virtual).
// Callees: STLport vector<CStreamingFontDataBankEntry*>::_M_insert_overflow @ 0x00A5F8E0, operator new @ 0x00BFEA1A,
//   CBaseClassNonCopyable::CBaseClassNonCopyable @ 0x0099A2F0, CBankFile::NotifyEntryAdded @ 0x009CBC20.
// Shape: the CShaderDataBank::NotifyEntryAdded twin (0x00A623D0) with the 0x18-byte entry constructor inlined (base ctor,
//   vtable 0x0129D5B4, zeroed CCountedPointer<CFontBank>, then bank/index/type). Retail Fonts +0x178 (base 0x164 + StateBlock).
// Leaf: ?NotifyEntryAdded@CStreamingFontDataBank
template <class T> struct CArray;
class CStreamingFontDataBank;
struct __true_type { __true_type() { } };
struct CBaseClass { virtual ~CBaseClass(); };
struct CBaseClassNonCopyable : CBaseClass {
    CBaseClassNonCopyable();                                 // 0x0099A2F0
    virtual ~CBaseClassNonCopyable();                        // 0x0099A300
};
struct CCPPointerInfo;
struct CFontBank;
template <class T> struct CCountedPointer {
    T* Data; CCPPointerInfo* Info;
    CCountedPointer() : Data(0), Info(0) { }
};
class CStreamingFontDataBankEntry : public CBaseClassNonCopyable {   // vtable 0x0129D5B4, 0x18 bytes
public:
    CCountedPointer<CFontBank> Font;                        // +0x04
    CStreamingFontDataBank*    Bank;                        // +0x0c
    unsigned long              Index;                       // +0x10
    unsigned long              DataType;                    // +0x14
    CStreamingFontDataBankEntry(CStreamingFontDataBank* bank, unsigned long index, unsigned long type)
        : Bank(bank), Index(index), DataType(type) { }
    virtual bool IsLoaded() const;                          // slot 1 (0x00A5F220)
};
template <class T> struct CArray {
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    unsigned long size() const { return _M_finish - _M_start; }
    T* begin() { return _M_start; }
    T& operator[](unsigned long n) { return *(begin() + n); }
    void _M_insert_overflow(T* pos, const T& x, const __true_type& tag, unsigned long fill_len, bool atend);   // 0x00A5F8E0
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
    unsigned char _body[0x174];
};
class CStreamingFontDataBank : public CBankFile {
protected:
    CArray<CStreamingFontDataBankEntry*> Fonts;             // +0x178
    virtual void NotifyEntryAdded(unsigned long type, unsigned long index, const CArray<char>& data);
};
void CStreamingFontDataBank::NotifyEntryAdded(unsigned long type, unsigned long index, const CArray<char>& data)
{
    while (index >= Fonts.size())
        Fonts.push_back(0);
    CStreamingFontDataBankEntry* entry = 0;
    if (type == 0 || type == 1 || type == 2)
        entry = new CStreamingFontDataBankEntry(this, index, type);
    Fonts[index] = entry;
    CBankFile::NotifyEntryAdded(type, index, data);
}

// ---- stubs and driver ----
typedef CStreamingFontDataBankEntry E;
static E* g_store[8]; static int g_grow = 0, g_base = 0, g_nc = 0; static unsigned long g_baseType = 99, g_baseIndex = 99; static const CArray<char>* g_baseData = 0;
void* GrowVectorStorage_s7(void* storage, unsigned long count)
{
    ++g_grow; CArray<E*>* v = (CArray<E*>*)storage; E** p = v->_M_finish; v->_M_finish += count; if (v->_M_end_of_storage < v->_M_finish) v->_M_end_of_storage = v->_M_finish; return p;
}
CBaseClass::~CBaseClass() {}
CBaseClassNonCopyable::CBaseClassNonCopyable() { ++g_nc; }
CBaseClassNonCopyable::~CBaseClassNonCopyable() {}
bool E::IsLoaded() const { return false; }
void CBankFile::NotifyEntryAdded(unsigned long type, unsigned long index, const CArray<char>& data) { ++g_base; g_baseType = type; g_baseIndex = index; g_baseData = &data; }
struct TBank : CStreamingFontDataBank {
    void Call(unsigned long type, unsigned long index, const CArray<char>& data) { CStreamingFontDataBank::NotifyEntryAdded(type, index, data); }
    CArray<E*>& V() { return Fonts; }
};
int main()
{
    static TBank b; b.V()._M_start = g_store; b.V()._M_finish = g_store; b.V()._M_end_of_storage = g_store + 2;
    CArray<char> data; data._M_start = data._M_finish = data._M_end_of_storage = 0;
    g_store[2] = (E*)1;
    b.Call(2, 2, data);
    CHECK(b.V().size() == 3 && g_grow == 1 && g_store[0] == 0 && g_store[1] == 0);
    CHECK(g_store[2] != 0 && g_store[2] != (E*)1 && g_nc == 1); CHECK(g_store[2]->Bank == (CStreamingFontDataBank*)&b && g_store[2]->Index == 2 && g_store[2]->DataType == 2 && g_store[2]->Font.Data == 0 && g_store[2]->Font.Info == 0);
    CHECK(g_base == 1 && g_baseType == 2 && g_baseIndex == 2 && g_baseData == &data);
    b.Call(0, 0, data); CHECK(g_nc == 2 && g_store[0] != 0 && g_store[0]->DataType == 0 && g_store[0]->Index == 0 && g_base == 2);
    b.Call(1, 1, data); CHECK(g_nc == 3 && g_store[1] != 0 && g_store[1]->DataType == 1);
    b.Call(3, 2, data); CHECK(g_nc == 3 && g_store[2] == 0 && g_base == 4 && g_baseType == 3);
    b.V()._M_end_of_storage = g_store + 8; b.Call(9, 4, data); CHECK(b.V().size() == 5 && g_grow == 1 && g_store[3] == 0 && g_store[4] == 0);
    if (fails == 0) printf("00a5f430_TEST PASS\n"); else printf("00a5f430_TEST FAIL\n");
    return fails;
}