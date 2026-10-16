// Grade: DIFFER(235v235) -- register allocation in the loop: retail forms &Filenames[i] (eax) before loading filenames[i] (ecx/edx base), ours the reverse, and the size compare uses ecx/eax vs our edx/ecx | tried: plain compare, crc local read before the call (fixed the prologue), #pragma function(strcmp) (fixed the size: the intrinsic expands inline) | next: named `const CCharString& mine = Filenames[i]` before the strcmp, or c_str() as out-of-line-shaped inline taking the element pointer
// CBankFileEntryUpdateData::Equals(const CArray<CCharString>& filenames, CCountedPointer<CBankStateBlock> stateBlock) const -- the update record still matches when the state block's CRC and every source filename are unchanged
// Leaf: ?Equals@CBankFileEntryUpdateData@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009CC750, length 235 bytes (+5 int3), ret 0xc. Ego_r.pdb
// ?Equals@CBankFileEntryUpdateData@@ABE_NABV?$CArray@VCCharString@@@@V?$CCountedPointer@VCBankStateBlock@@@@@Z (private
// there; public here).
// Callees: the state block's virtual slot 2 (+0x08, GetCRC), strcmp @ 0x00BFEBA8, operator delete @ 0x00BFE9BC (the by-value
// counted pointer's count block, released on each of the three exits). "" @ 0x0129AAF4 for empty strings.
// Shape: CCharString's inline c_str() (`p ? p->Text : ""`); the small vector's count is a signed char (`movsx`); the loop
// bound (the array's size) is re-evaluated after each strcmp; the argument's destructor releases the count block without
// zeroing (no reload: the block pointer is read once).
// Layout = Ego_r.pdb CBankFileEntryUpdateData: StateBlockCRC +0, Filenames +0xc (CSmallVector<CCharString,8>: Start +0,
// Size +4 schar).
extern "C" int __cdecl strcmp(const char* a, const char* b);   // 0x00BFEBA8
#pragma function(strcmp)                           // a real call, as retail (the intrinsic expands inline)
void __cdecl operator delete(void* p);             // 0x00BFE9BC
extern const char g_emptyString[];                 // "" @ 0x0129AAF4

struct CCharStringData { const char* Text; };
struct CCharString {
    CCharStringData* PStringData;
    const char* c_str() const { return PStringData ? PStringData->Text : g_emptyString; }
};
template <class T> struct CArray {                 // retail CArray: 12 bytes
    T* m_pFirst;
    T* m_pLast;
    T* m_pEnd;
    long size() const { return m_pLast - m_pFirst; }
    const T& operator[](unsigned long index) const { return m_pFirst[index]; }
};
template <class T, int N> struct CSmallVector {
    T* Start;                                      // +0
    signed char Size;                              // +4
    long size() const { return Size; }
    const T& operator[](unsigned long index) const { return Start[index]; }
};
struct SCountBlock {
    long m_references;                             // +0
    void (__fastcall* m_pfnDestroy)(void*);        // +4
    void* m_pObject;                               // +8
};
template <class T> struct CCountedPointer {
    T* Data;                                       // +0
    SCountBlock* Info;                             // +4
    ~CCountedPointer()
    {
        SCountBlock* info = Info;
        if (info && --info->m_references == 0) {
            info->m_pfnDestroy(info->m_pObject);
            operator delete(info);
        }
    }
    T* operator->() const { return Data; }
};
class CBankStateBlock {
public:
    virtual void v00(); virtual void v01();
    virtual unsigned long GetCRC() const;          // slot 2 (+0x08)
};

class CBankFileEntryUpdateData {
public:
    unsigned long StateBlockCRC;                   // +0x00
    unsigned long InfoSize;                        // +0x04
    char* InfoBuffer;                              // +0x08
    CSmallVector<CCharString, 8> Filenames;        // +0x0c
    bool RequiresUpdate;                           // +0x14
    bool Exists;                                   // +0x15
    bool Equals(const CArray<CCharString>& filenames, CCountedPointer<CBankStateBlock> stateBlock) const;
};

bool CBankFileEntryUpdateData::Equals(const CArray<CCharString>& filenames, CCountedPointer<CBankStateBlock> stateBlock) const
{
    unsigned long crc = StateBlockCRC;
    if (crc != stateBlock->GetCRC())
        return false;
    if (Filenames.size() != filenames.size())
        return false;
    for (unsigned long i = 0; i < (unsigned long)filenames.size(); ++i) {
        if (strcmp(Filenames[i].c_str(), filenames[i].c_str()) != 0)
            return false;
    }
    return true;
}
