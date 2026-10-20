// Behaviour test for 009708a0: the model below is a copy of src/009708a0.cpp (compiled separately), driven by test doubles.
#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __forceinline inline
#endif
static int g_sets = 0, g_dtors = 0;
struct CThing {};
struct CBaseIntelligentPointer {
    virtual ~CBaseIntelligentPointer() { ++g_dtors; m_pItem = 0; }
    void* m_pItem;
    CThing* GetPItem() const { return (CThing*)m_pItem; }
    void SetPItem(CThing* item) { ++g_sets; m_pItem = item; }
};
struct CIntelligentPointerThing : CBaseIntelligentPointer {
    CIntelligentPointerThing& operator=(const CIntelligentPointerThing& other) { CThing* item = other.GetPItem(); SetPItem(item); return *this; }
};
__forceinline CIntelligentPointerThing* CopyRange(CIntelligentPointerThing* first, CIntelligentPointerThing* last, CIntelligentPointerThing* result)
{
    for (long n = last - first; n > 0; --n) {
        *result = *first;
        ++first;
        ++result;
    }
    return result;
}
struct CCoverThingVector {
    CIntelligentPointerThing* _M_start;
    CIntelligentPointerThing* _M_finish;
    CIntelligentPointerThing* _M_end_of_storage;
    CIntelligentPointerThing* begin() { return _M_start; }
    CIntelligentPointerThing* end() { return _M_finish; }
    __forceinline CIntelligentPointerThing* erase(CIntelligentPointerThing* pos)
    {
        if (pos + 1 != _M_finish)
            CopyRange(pos + 1, _M_finish, pos);
        --_M_finish;
        _M_finish->~CIntelligentPointerThing();
        return pos;
    }
};
struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase() {}
    void* PBrain;
    long ScriptAIPriority;
    void* Name_PStringData;
    unsigned char Terminated;
    unsigned char Priority;
    unsigned char Enabled;
    unsigned char _pad_0x13;
};
struct CAIStateGroup_CreatureUseCoverBase : CAIStateGroupBase {
    unsigned char _pad_0x14[8];
    bool HasDiveAnim;
    bool HasJumpOutAnims;
    bool TimingCritical;
    unsigned char _pad_0x1f;
    unsigned char PCoverThing[8];
    CCoverThingVector CoverList;
    void RemoveThingFromCoverList(CThing* thing);
};

void CAIStateGroup_CreatureUseCoverBase::RemoveThingFromCoverList(CThing* thing)
{
    for (CIntelligentPointerThing* it = CoverList.begin(); it != CoverList.end(); ) {
        if (it->GetPItem() == thing)
            it = CoverList.erase(it);
        else
            ++it;
    }
}

static CThing g_things[4];
static bool Run(const int* content, int count, int remove, const int* expect, int expectCount, int expectSets, int expectDtors)
{
    CIntelligentPointerThing slots[6];
    for (int i = 0; i < count; ++i) slots[i].m_pItem = &g_things[content[i]];
    for (int i = count; i < 6; ++i) slots[i].m_pItem = 0;
    CAIStateGroup_CreatureUseCoverBase g; g.CoverList._M_start = slots; g.CoverList._M_finish = slots + count; g.CoverList._M_end_of_storage = slots + 6;
    g_sets = g_dtors = 0;
    g.RemoveThingFromCoverList(&g_things[remove]);
    if (g.CoverList._M_finish - g.CoverList._M_start != expectCount) return false;
    for (int i = 0; i < expectCount; ++i) if (slots[i].m_pItem != &g_things[expect[i]]) return false;
    if (g_sets != expectSets || g_dtors != expectDtors) return false;
    return true;
}
int main()
{
    bool ok = true;
    { int c[] = { 0, 1, 2, 1 }; int e[] = { 0, 2 }; ok = ok && Run(c, 4, 1, e, 2, 2, 2); }   // both copies of thing 1 removed: two element copies for the first erase, none for the last, 2 dtors
    { int c[] = { 0, 2 };       int e[] = { 0, 2 }; ok = ok && Run(c, 2, 3, e, 2, 0, 0); }   // not present: untouched
    { int c[] = { 3 };          int e[] = { 0 };    ok = ok && Run(c, 1, 3, e, 0, 0, 1); }   // last element: no copy, one dtor
    { int c[] = { 0 };          int e[] = { 0 };    ok = ok && Run(c, 0, 0, e, 0, 0, 0); }   // empty list
    if (!ok) { std::printf("009708a0_TEST FAIL\n"); return 1; }
    std::printf("009708a0_TEST PASS\n");
    return 0;
}