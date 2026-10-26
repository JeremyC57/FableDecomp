// Behaviour test for 00a636c0: src/00a636c0.cpp copied verbatim (the function body and its same-TU stand-ins), the remaining
// declared callees stubbed below so calls and arguments are observable. CFontDataBank::NotifyEntryAdded(unsigned long type, unsigned long index, const CArray<char>& info)
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CFontDataBank::NotifyEntryAdded(unsigned long type, unsigned long index, const CArray<char>& info) -- grow the
// entry table to cover the new index (null entries), store a new CFontDataBankEntry for a type-0 entry (else null),
// then let CBankFile record the entry
// Leaf: ?NotifyEntryAdded@CFontDataBank
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A636C0, length 203 bytes, `ret 0Ch`.
// Callees: the STLport vector<CFontDataBankEntry*>::_M_insert_overflow @ 0x00A639E0, operator new @ 0x00BFEA1A,
// CBaseClassNonCopyable::CBaseClassNonCopyable @ 0x0099A2F0, CBankFile::NotifyEntryAdded @ 0x009CBC20.
// Vtable 0x0129DA70 = CFontDataBankEntry (graphics/font layer).
// Layout: engine/CFontDataBank.h is opaque padding; retail Entries (STLport vector of entry pointers) +0x120;
// CFontDataBankEntry (PDB, 0x18): CBaseClassNonCopyable vptr, Font +4 (CCountedPointer<CFontBank>), Bank +0xC,
// Index +0x10, DataType +0x14.
// Shape: `while (index >= size()) push_back(0);` with the inline STLport push_back (placement new null test, the
// zero value temporary and the __true_type tag both in the dead `index` slot, which retail addresses twice); the new
// entry's ctor inline; the table store, then the base-class call.

struct CBankFileEntryInfo;
template <class T> class CArray;

class CBaseClass { public:
    virtual ~CBaseClass();
};
struct CBaseClassNonCopyable : CBaseClass {
    CBaseClassNonCopyable();                        // 0x0099A2F0
    virtual ~CBaseClassNonCopyable();
};

class CFontDataBank;
class CFontBank;
struct CCountedPointerWords {
    void* Data;
    void* Info;
    CCountedPointerWords() : Data(0), Info(0) {}
};
class CFontDataBankEntry : public CBaseClassNonCopyable {  // vtable 0x0129DA70
public:
    CCountedPointerWords Font;                      // +0x04 CCountedPointer<CFontBank>
    CFontDataBank* Bank;                            // +0x0C
    unsigned long Index;                            // +0x10
    unsigned long DataType;                         // +0x14
    CFontDataBankEntry(CFontDataBank* bank, unsigned long index) : Bank(bank), Index(index), DataType(0) {}
    virtual ~CFontDataBankEntry();
};

struct __true_type { __true_type() {} };
inline void* operator new(unsigned int, void* where) { return where; }
extern void* operator new(unsigned int size);       // 0x00BFEA1A

template <class T> struct CRetailPtrVector {
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    unsigned long size() const { return _M_finish - _M_start; }
    void _M_insert_overflow(T* position, const T& x, const __true_type&, unsigned long fillLength, bool atEnd);   // 0x00A639E0
    void push_back(const T& x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) T(x);
            ++_M_finish;
        } else
            _M_insert_overflow(_M_finish, x, __true_type(), 1, true);
    }
    T& operator[](unsigned long i) { return _M_start[i]; }
};
void* GrowVectorStorage_s7(void* vector, unsigned long count);
#pragma auto_inline(off)
// Same-TU body of the vector growth template (_M_insert_overflow, instantiated in retail's TU): reads the value through its const
// reference into the new storage and never touches the empty tag (the landed push_back idiom).
template <class T>
__declspec(noinline) void CRetailPtrVector<T>::_M_insert_overflow(T* position, const T& x, const __true_type& p2, unsigned long fillLength, bool atEnd)
{
    T* p = (T*)GrowVectorStorage_s7(this, fillLength);
    if (p)
        *p = x;
    
}
#pragma auto_inline(on)

class CBankFile {
public:
    virtual ~CBankFile();
protected:
    virtual void NotifyEntryAdded(unsigned long type, unsigned long index, const CArray<char>& info);   // 0x009CBC20
};

class CFontDataBank : public CBankFile {
public:
    unsigned char _pad004[0x11c];
    CRetailPtrVector<CFontDataBankEntry*> Entries;  // +0x120
protected:
    virtual void NotifyEntryAdded(unsigned long type, unsigned long index, const CArray<char>& info);
};

void CFontDataBank::NotifyEntryAdded(unsigned long type, unsigned long index, const CArray<char>& info)
{
    while (index >= Entries.size())
        Entries.push_back((CFontDataBankEntry*)0);
    CFontDataBankEntry* entry = 0;
    if (type == 0)
        entry = new CFontDataBankEntry(this, index);
    Entries[index] = entry;
    CBankFile::NotifyEntryAdded(type, index, info);
}

// ---- stubs and driver ----
typedef CFontDataBankEntry E;
static E* g_store[8]; static int g_grow = 0, g_base = 0, g_nc = 0; static unsigned long g_baseType = 99, g_baseIndex = 99; static const CArray<char>* g_baseData = 0;
void* GrowVectorStorage_s7(void* storage, unsigned long count)
{
    ++g_grow; CRetailPtrVector<E*>* v = (CRetailPtrVector<E*>*)storage; E** p = v->_M_finish; v->_M_finish += count; if (v->_M_end_of_storage < v->_M_finish) v->_M_end_of_storage = v->_M_finish; return p;
}
CBaseClass::~CBaseClass() {}
CBaseClassNonCopyable::CBaseClassNonCopyable() { ++g_nc; }
CBaseClassNonCopyable::~CBaseClassNonCopyable() {}
E::~CFontDataBankEntry() {}
CBankFile::~CBankFile() {}
void CBankFile::NotifyEntryAdded(unsigned long type, unsigned long index, const CArray<char>& info) { ++g_base; g_baseType = type; g_baseIndex = index; g_baseData = &info; }
struct TBank : CFontDataBank {
    void Call(unsigned long type, unsigned long index, const CArray<char>& data) { CFontDataBank::NotifyEntryAdded(type, index, data); }
};
int main()
{
    static TBank b; b.Entries._M_start = g_store; b.Entries._M_finish = g_store; b.Entries._M_end_of_storage = g_store + 2;
    static long dummy[3]; const CArray<char>& data = *(const CArray<char>*)dummy;
    g_store[2] = (E*)1;
    b.Call(0, 2, data);
    CHECK(b.Entries.size() == 3 && g_grow == 1 && g_store[0] == 0 && g_store[1] == 0);
    CHECK(g_store[2] != 0 && g_store[2] != (E*)1 && g_nc == 1); CHECK(g_store[2]->Bank == (CFontDataBank*)&b && g_store[2]->Index == 2 && g_store[2]->DataType == 0 && g_store[2]->Font.Data == 0 && g_store[2]->Font.Info == 0);
    CHECK(g_base == 1 && g_baseType == 0 && g_baseIndex == 2 && g_baseData == &data);
    b.Call(1, 0, data); CHECK(g_nc == 1 && g_store[0] == 0 && g_base == 2 && g_baseType == 1);
    b.Call(2, 2, data); CHECK(g_nc == 1 && g_store[2] == 0 && g_base == 3);
    b.Entries._M_end_of_storage = g_store + 8; b.Call(0, 3, data); CHECK(b.Entries.size() == 4 && g_grow == 1 && g_store[3] != 0 && g_nc == 2 && g_store[3]->Index == 3);
    if (fails == 0) printf("00a636c0_TEST PASS\n"); else printf("00a636c0_TEST FAIL\n");
    return fails;
}