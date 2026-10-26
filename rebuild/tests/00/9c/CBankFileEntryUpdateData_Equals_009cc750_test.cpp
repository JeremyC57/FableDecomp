// Behaviour test for 009cc750: src/009cc750.cpp copied verbatim; the update record matches when the state block's CRC
// and every filename (null block compared as "") are unchanged; the by-value counted pointer is released on every exit.
#include <cstdio>
#include <cstdlib>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CBankFileEntryUpdateData::Equals(const CArray<CCharString>& filenames, CCountedPointer<CBankStateBlock> stateBlock) const -- the update record still matches when the state block's CRC and every source filename are unchanged
// Leaf: ?Equals@CBankFileEntryUpdateData@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009CC750, length 235 bytes (+5 int3), ret 0xc. Ego_r.pdb
// ?Equals@CBankFileEntryUpdateData@@ABE_NABV?$CArray@VCCharString@@@@V?$CCountedPointer@VCBankStateBlock@@@@@Z (private
// there; public here).
// Callees: the state block's virtual slot 2 (+0x08, GetCRC), strcmp @ 0x00BFEBA8, operator delete @ 0x00BFE9BC (the by-value
// counted pointer's count block, released on each of the three exits). "" @ 0x0129AAF4 for empty strings.
// Shape: CCharString's inline c_str() tests the null block first (`if (p == 0) return ""; return p->Text;`, retail's
//   `test ; jne` to the load); the loop names `const CCharString& mine = Filenames[i]` before the strcmp (its element
//   address is formed before the argument's c_str, as retail); the small vector's count is a signed char (`movsx`); the loop
// bound (the array's size) is re-evaluated after each strcmp; the argument's destructor releases the count block without
// zeroing (no reload: the block pointer is read once).
// Layout = Ego_r.pdb CBankFileEntryUpdateData: StateBlockCRC +0, Filenames +0xc (CSmallVector<CCharString,8>: Start +0,
// Size +4 schar).
extern "C" int __cdecl strcmp(const char* a, const char* b);   // 0x00BFEBA8
#pragma function(strcmp)                           // a real call, as retail (the intrinsic expands inline)
void __cdecl operator delete(void* p);             // 0x00BFE9BC
extern const char g_emptyString[];                 // "" @ 0x0129AAF4

class CCharStringData { public: const char* Text; };
class CCharString { public:
    CCharStringData* PStringData;
    const char* c_str() const { if (PStringData == 0) return g_emptyString; return PStringData->Text; }
};
template <class T> class CArray { public:                 // retail CArray: 12 bytes
    T* m_pFirst;
    T* m_pLast;
    T* m_pEnd;
    long size() const { return m_pLast - m_pFirst; }
    const T& operator[](unsigned long index) const { return m_pFirst[index]; }
};
template <class T, int N> class CSmallVector { public:
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
template <class T> class CCountedPointer { public:
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
        const CCharString& mine = Filenames[i];
        if (strcmp(mine.c_str(), filenames[i].c_str()) != 0)
            return false;
    }
    return true;
}
// ---- end source ----
const char g_emptyString[] = "";
static unsigned long g_crc; static int g_destroyed;
struct TestBlock : CBankStateBlock { void v00() {} void v01() {} unsigned long GetCRC() const { return g_crc; } };
void CBankStateBlock::v00() {} void CBankStateBlock::v01() {}
unsigned long CBankStateBlock::GetCRC() const { return 0; }
static void __fastcall Destroy(void*) { ++g_destroyed; }
static TestBlock g_block;
static bool Ask(const CBankFileEntryUpdateData& d, const CArray<CCharString>& names, unsigned long crc, int refs)
{
    g_crc = crc;
    SCountBlock* info = (SCountBlock*)malloc(sizeof(SCountBlock));
    info->m_references = refs; info->m_pfnDestroy = Destroy; info->m_pObject = &g_block;
    CCountedPointer<CBankStateBlock> p; p.Data = &g_block; p.Info = info;
    bool r = d.Equals(names, p);   // the copy shares Info; Equals releases its copy
    if (refs > 1) { CHECK(info->m_references == refs - 1); free(info); }
    p.Info = 0;
    return r;
}
int main()
{
    CCharStringData a = { "a.bnk" }, b = { "b.bnk" }, b2 = { "b.bnk" }, c = { "c.bnk" };
    CCharString mine[3]; mine[0].PStringData = &a; mine[1].PStringData = &b; mine[2].PStringData = 0;
    CCharString theirs[3]; theirs[0].PStringData = &a; theirs[1].PStringData = &b2; theirs[2].PStringData = 0;
    CBankFileEntryUpdateData d; d.StateBlockCRC = 77; d.InfoSize = 0; d.InfoBuffer = 0; d.Filenames.Start = mine; d.Filenames.Size = 3;
    CArray<CCharString> names; names.m_pFirst = theirs; names.m_pLast = theirs + 3; names.m_pEnd = theirs + 3;
    CHECK(Ask(d, names, 77, 2) == true);
    CHECK(Ask(d, names, 78, 2) == false);                  // CRC changed
    theirs[1].PStringData = &c; CHECK(Ask(d, names, 77, 2) == false); theirs[1].PStringData = &b2;
    CCharStringData empty = { "" }; theirs[2].PStringData = &empty; CHECK(Ask(d, names, 77, 2) == true);   // null == ""
    names.m_pLast = theirs + 2; CHECK(Ask(d, names, 77, 2) == false); names.m_pLast = theirs + 3;          // size differs
    g_destroyed = 0; CHECK(Ask(d, names, 77, 1) == true && g_destroyed == 1);   // last reference: destroyed, block freed
    if (fails == 0) printf("009cc750_TEST PASS\n"); else printf("009cc750_TEST FAIL\n");
    return fails;
}