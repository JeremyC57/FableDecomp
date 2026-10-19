// CFontDataBank::NotifyEntryAdded(unsigned long type, unsigned long index, const CArray<char>& info) -- grow the
// Grade: DIFFER(ours v retail) -- 208v203: the push_back zero temporary gets its own frame word (sub esp,8, stored inside the loop) where retail hoists one zero store into the dead index slot and passes that slot as both the value and the __true_type tag (lane gap: tag/temporary over an argument slot)
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

struct CBaseClass {
    virtual ~CBaseClass();
};
struct CBaseClassNonCopyable : CBaseClass {
    CBaseClassNonCopyable();                        // 0x0099A2F0
    virtual ~CBaseClassNonCopyable();
};

class CFontDataBank;
struct CFontBank;
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
