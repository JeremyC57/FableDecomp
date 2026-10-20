// Grade: DIFFER(218v213) -- the 0x00A623D0 push_back gap: retail homes the null value and the __true_type tag in the one dead `index` slot (value stored once before the loop); the lane opens its own frame word for the value (re-stored per pass) and puts the tag in the index slot. The rest (inline entry ctor, type test, base call) lines up.
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
