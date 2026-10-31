// CASoundManager::RegisterAtmosSoundBank(long id, const CWideString& name, const CWideString& fileName) -- create an ambient (atmos) sample bank through the platform factory, flag it, show "Register Atmos Sound Bank" progress, and add its (id, record) entry to the sorted bank map
// Leaf: ?RegisterAtmosSoundBank@CASoundManager@@
// Grade: DIFFER(333v338) -- CVectorMap::Add / Sort are __forceinline (VC7.1 otherwise keeps Add out of line) and
//   DisplayProgress takes the literal through the implicit CCharString conversion (its temporary built after the other
//   arguments, as retail); the only residue is retail's re-read of NeedsSort right after storing it (`mov byte
//   [esi+0Dh],1 ; cmp byte [esi+0Dh],bl ; je`, 5 bytes), which only an uncacheable (volatile) flag reproduces and the
//   brief bans
// Retail: Fable.exe (TLC, VC7.1) @ 0x00991C10, 338 bytes, thiscall `ret 0Ch`. Sound module: absent from the Ego PDB and
//   FableWin.pdb, so the method and member names are descriptive; the class is RTTI "CASoundManager" (vtable 0x0129A574,
//   abstract: slot 1 CreateSoundBank is _purecall). One caller 0x0041816A. String 0x0129A67C "Register Atmos Sound Bank".
// Callees: CreateSoundBank (this slot 1), the bank's slot 8 (+0x20), CCharString(const char*, long) 0x0099EBF0,
//   DisplayProgress 0x009E9F40, ~CCharString 0x0099EAE0, CCharString() 0x0099E4B0 (x4), ConvertToCharString 0x0099B2C0,
//   CCharString::operator= 0x0099EFB0, CCountedPointer<CASoundBank>::Reset 0x0042FD6F, the record copy ctor 0x009922F0
//   (pair copy and push_back's _Construct), vector::_M_insert_overflow 0x00993310 (this chunk), ~CSoundBankRecord
//   0x00992510, sort 0x009939B0.
// Shape: the record {Bank, LipSyncBank, Name, FileName, SymbolFileName, LipSyncFileName} is filled, then
//   CVectorMap::Add = push_back(pair(id, record)), NeedsSort = true, and the inline Sort() re-reads NeedsSort
//   (`mov byte [esi+0Dh],1 ; cmp byte [esi+0Dh],bl`): the dead load that survives a constant-true condition is the
//   signature of an uncacheable flag member that a lane source may not spell -- parked after one grade (see NOTES.md).
#include <stddef.h>
extern "C" void* __cdecl malloc(unsigned int size);                                 // 0x00BFEA0E
extern "C" void __cdecl free(void* block);                                          // 0x00BFEA14
void __cdecl operator delete(void* block);                                          // 0x00BFE9BC
inline void* operator new(unsigned int, void* p) { return p; }

class CCharStringData;
class CCharString
{
public:
    CCharStringData* PStringData;                                                   // +0x00 {text, length}
    CCharString();                                                                  // 0x0099E4B0
    CCharString(const char* text, long length = -1);                                // 0x0099EBF0
    CCharString(const CCharString& other);                                          // 0x0099EC30
    ~CCharString();                                                                 // 0x0099EAE0
    CCharString& operator=(const CCharString& other);                               // 0x0099EFB0
    bool IsEmpty() const { return PStringData == 0 || ((const long*)PStringData)[1] == 0; }
};

class CWideString
{
public:
    void* PStringData;
    CCharString ConvertToCharString() const;                                        // 0x0099B2C0
    bool operator!=(const wchar_t* other) const;                                    // 0x0099B150
};

void __fastcall DisplayProgress(const CCharString& text, float progress, bool a, bool b);   // 0x009E9F40

class CCPPointerInfo
{
public:
    long  Count;                                                                    // +0x00
    void (__fastcall* Destroy)(void* item);                                         // +0x04
    void* Item;                                                                     // +0x08
};
template <class T> class CCountedPointer
{
public:
    T*              Data;                                                           // +0x00
    CCPPointerInfo* Info;                                                           // +0x04
    CCountedPointer() : Data(0), Info(0) { }
    ~CCountedPointer();
    void Reset(T* data);                                                            // 0x0042FD6F (CASoundBank), 0x00992220 (CLipSyncDataBank)
    void Clear()
    {
        if (Info) {
            --Info->Count;
            if (Info->Count == 0) {
                Info->Destroy(Info->Item);
                ::operator delete(Info);
            }
        }
        Data = 0;
        Info = 0;
    }
    T* operator->() const { return Data; }
};

class CASoundBank
{
public:
    virtual ~CASoundBank();
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06();
    virtual void v07();
    virtual void SetAtmos();                                                        // slot 8 (+0x20)
    void InitSymbols(const CWideString& fileName);                                  // 0x00A01950
};
class CResourceBank;
class CLipSyncDataBank
{
public:
    CLipSyncDataBank();                                                             // 0x00A42B80 (0x180 bytes)
    virtual ~CLipSyncDataBank();
    virtual bool Load(const CCharString& fileName, long mode);                      // slot 1 (+0x04)
    void SetResourceBank(const CCountedPointer<CResourceBank>& bank);               // 0x00992470
    unsigned char _body[0x180 - 4];
};

class CSoundBankRecord                                                              // 0x20 bytes
{
public:
    CCountedPointer<CASoundBank>      Bank;                                         // +0x00
    CCountedPointer<CLipSyncDataBank> LipSyncBank;                                  // +0x08
    CCharString                       Name;                                         // +0x10
    CCharString                       FileName;                                     // +0x14
    CCharString                       SymbolFileName;                               // +0x18
    CCharString                       LipSyncFileName;                              // +0x1C
    CSoundBankRecord() { }
    CSoundBankRecord(const CSoundBankRecord& other);                                // 0x009922F0
    ~CSoundBankRecord();                                                            // 0x00992510
};

namespace std {
class __false_type { public: __false_type() { } };
template <class _Tp> class allocator { public: allocator() { } };
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
};
template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;                                                                  // +0x00
    _Tp* _M_finish;                                                                 // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;                         // +0x08
};
template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* iterator;
    iterator begin() { return this->_M_start; }
    iterator end() { return this->_M_finish; }
    void _M_insert_overflow(_Tp* __position, const _Tp& __x, const __false_type&, unsigned int __fill_len, bool __atend);   // 0x00993310
    void push_back(const _Tp& __x)
    {
        if (this->_M_finish != this->_M_end_of_storage._M_data) {
            _Construct(this->_M_finish, __x);
            ++this->_M_finish;
        } else
            _M_insert_overflow(this->_M_finish, __x, __false_type(), 1UL, true);
    }
};
template <class _RandomAccessIter, class _Compare>
void __fastcall sort(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp);   // 0x009939B0
} // namespace std

class CSoundBankEntryLess                                                           // empty comparator (1 byte)
{
public:
    bool operator()(const std::pair<long, CSoundBankRecord>& a, const std::pair<long, CSoundBankRecord>& b) const { return a.first < b.first; }
};

template <class K, class V, class Compare> class CVectorMap                         // 0x10 bytes
{
public:
    std::vector<std::pair<K, V> > Data;                                             // +0x00
    Compare                       Comp;                                             // +0x0C
    bool                          NeedsSort;                                        // +0x0D (retail re-reads it after the store)
    __forceinline void Sort()
    {
        if (NeedsSort) {
            std::sort(Data.begin(), Data.end(), Comp);
            NeedsSort = false;
        }
    }
    __forceinline void Add(const K& key, const V& value)
    {
        Data.push_back(std::pair<K, V>(key, value));
        NeedsSort = true;
        Sort();
    }
};

class CASoundManager                                                                // vtable 0x0129A574
{
public:
    virtual ~CASoundManager();
    virtual CASoundBank* CreateSoundBank(const CWideString& fileName) = 0;          // slot 1 (+0x04)
    void RegisterSoundBank(long id, const CWideString& name, const CWideString& fileName, const CWideString& symbolFileName, const CCharString& lipSyncFileName);
    void RegisterAtmosSoundBank(long id, const CWideString& name, const CWideString& fileName);
    unsigned char                                             _pad04[0x28 - 4];
    CCountedPointer<CResourceBank>                            ResourceBank;         // +0x28
    CVectorMap<long, CSoundBankRecord, CSoundBankEntryLess>   SoundBanks;           // +0x30
};

void CASoundManager::RegisterAtmosSoundBank(long id, const CWideString& name, const CWideString& fileName)
{
    CASoundBank* bank = CreateSoundBank(fileName);
    bank->SetAtmos();
    DisplayProgress("Register Atmos Sound Bank", -1.0f, false, false);
    CSoundBankRecord record;
    record.Name = name.ConvertToCharString();
    record.FileName = fileName.ConvertToCharString();
    record.Bank.Reset(bank);
    SoundBanks.Add(id, record);
}
