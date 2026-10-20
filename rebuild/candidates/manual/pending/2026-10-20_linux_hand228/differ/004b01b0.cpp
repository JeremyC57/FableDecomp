// Grade: DIFFER(281v277) -- loop 1 now matches except the iterator start (retail loads begin into edi for size() then copies it to ebx after the guard; ours loads ebx first and copies to edi); loop 2 keeps n alive for the malloc size (`lea edx,[eax*4]` vs retail `shl eax,2`) and rotates eax/ecx/edx
// CQuestManager::IsRegionValidInRegionLockingMode(const CRegion& region) const -- true when a quest between its start and end screens claims the region, or when none of those quests claims any region; false otherwise
// Leaf: IsRegionValidInRegionLockingMode
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004B01B0, length 275 bytes (`ret 4`). The catalog label
// "CScriptBase::ProcessQuestRegions" is wrong: the only caller (0x004A3817) runs it on the quest manager global 0x013B89FC
// with the world map's current region, and FableWin.pdb names ?IsRegionValidInRegionLockingMode@CQuestManager@@QBE_NABVCRegion@@@Z.
// Callees: CQuestManager::GetActiveQuestFromScriptName @ 0x004AF5B0 (landed), STLport __find(const CRegion**, const CRegion**,
// const CRegion* const&, const random_access_iterator_tag&) @ 0x004B74E0 (`__fastcall`; inlined CActiveQuest::ContainsRegion),
// malloc / memmove / free (MSVCR71 import thunks 0x00BFEA0E / 0x00BFEAE6 / 0x00BFEA14: the inlined by-value GetRegions copy).
// Layout: retail CQuestManager (landed GetActiveQuestFromScriptName model): BetweenStartAndEndScreenQuests vector<CCharString>
// at +0x4c (PDB +0x6c, retail containers are smaller); CActiveQuest::Regions vector<const CRegion*> at +0x10.
// Shape: the first loop's `found` flag and counter live in the frame; ContainsRegion is the inline `find(begin, end, &region)
// != end` with `end` kept in ebp (also used for the empty test); the second loop builds and drops the GetRegions() copy.
#include <string.h>

extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
extern "C" void  __cdecl free(void* block);            // 0x00BFEA14

struct random_access_iterator_tag { random_access_iterator_tag() { } };
struct CRegion;

// Stand-in body of the retail __find @ 0x004B74E0 compiled above the caller so VC7.1 knows it writes no memory (the retail
// build keeps Regions.end() in ebp across the call); the lander binds the call to the retail address.
#pragma auto_inline(off)
const CRegion** __fastcall __find(const CRegion** first, const CRegion** last, const CRegion* const& value,
                                  const random_access_iterator_tag&)
{
    for (; first != last; ++first)
        if (*first == value)
            return first;
    return last;
}
#pragma auto_inline(on)
inline const CRegion** find(const CRegion** first, const CRegion** last, const CRegion* const& value)
{
    return __find(first, last, value, random_access_iterator_tag());
}

template<class T> struct CRetailVector {          // STLport vector over a POD element type, malloc allocator
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T* begin() const { return _M_start; }
    T* end() const { return _M_finish; }
    bool empty() const { return _M_start == _M_finish; }
    unsigned long size() const { return _M_finish - _M_start; }
    T& operator[](unsigned long n) const { return *(begin() + n); }
    CRetailVector(const CRetailVector& other)
    {
        unsigned long n = other.size();
        _M_start = n ? (T*)malloc(n * sizeof(T)) : 0;
        _M_end_of_storage = _M_start + n;
        if (other._M_finish != other._M_start)
            memmove(_M_start, other._M_start, (char*)other._M_finish - (char*)other._M_start);
        _M_finish = _M_start + n;
    }
    ~CRetailVector() { if (_M_start) free(_M_start); }
};

struct CCharString { void* PStringData; };

struct CActiveQuest {                              // CQuestManager::CActiveQuest, retail layout
    long                          ID;              // +0x00
    void*                         PScriptInfo;     // +0x04
    void*                         PScript[2];      // +0x08 CCountedPointer<CScriptBase>
    CRetailVector<const CRegion*> Regions;         // +0x10
    bool ContainsRegion(const CRegion& region) const
    {
        const CRegion** last = Regions.end();
        return find(Regions.begin(), last, &region) != last;
    }
    const CRetailVector<const CRegion*> GetRegions() const { return Regions; }
};

struct CQuestManagerRetail {
    unsigned char              _pad00[0x4c];
    CRetailVector<CCharString> BetweenStartAndEndScreenQuests;   // +0x4c
    CActiveQuest* GetActiveQuestFromScriptName(const CCharString& name) const;   // 0x004AF5B0
    bool IsRegionValidInRegionLockingMode(const CRegion& region) const;
};

bool CQuestManagerRetail::IsRegionValidInRegionLockingMode(const CRegion& region) const
{
    bool questHasRegions = false;
    const CCharString* name = BetweenStartAndEndScreenQuests.begin();
    for (unsigned long i = 0; i < BetweenStartAndEndScreenQuests.size(); ++i, ++name) {
        CActiveQuest* quest = GetActiveQuestFromScriptName(*name);
        if (quest) {
            if (quest->ContainsRegion(region))
                return true;
            if (!quest->Regions.empty())
                questHasRegions = true;
        }
    }
    if (!questHasRegions)
        return true;
    for (unsigned long j = 0; j < BetweenStartAndEndScreenQuests.size(); ++j) {
        CActiveQuest* quest = GetActiveQuestFromScriptName(BetweenStartAndEndScreenQuests[j]);
        if (quest)
            quest->GetRegions();
    }
    return false;
}
