#include <stdio.h>
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __forceinline inline
#define __declspec(x)
#endif
// CTCUserActionPointBase::GetFreeActionPoint(CThing& thing, C3DVector& position, CRightHandedSet& rh, long& index) -- hand the thing a free action point: refresh the blocked list, remember the thing, then keep drawing random free points (taking each off the free list); one not known to be blocked and not blocked now is returned, a newly blocked one goes on the blocked list; false when the free list runs dry
// Leaf: GetFreeActionPoint
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007FEDE0, length 348 bytes (`ret 10h`, no frame pointer; Ego_r.pdb
//   `?GetFreeActionPoint@CTCUserActionPointBase@@QAE_NAAVCThing@@AAVC3DVector@@AAVCRightHandedSet@@AAJ@Z`).
// Callees: CTCUserActionPointBase::UpdateBlockedActionPoints() 0x007FECB0 (as landed); CBaseIntelligentPointer()
//   0x00A01B10, SetPItem 0x00A01B90, GetPItem 0x00A01B50, ~CBaseIntelligentPointer 0x00A01C10 (the inline
//   CIntelligentPointer<CThing> ctor / operator=, vtable 0x01238C6C); CTCBase::PeekWorld() const 0x00686A80 and the
//   world's random-seed accessor 0x0049D810 (as the landed AddWound 0x00757B30); GFROR13 0x00497890 (__fastcall, the
//   inline GFRandom LCG); memmove 0x00BFEAE6 (the inline vector<long>::erase); SGI __find<long*> 0x0050E540
//   (__fastcall, as landed); IsActionPointIndexBlocked (virtual slot 28, +0x70, as the landed UpdateBlockedActionPoints);
//   vector<long>::_M_insert_overflow 0x00440990 (the inline push_back).
// Layout: as the landed UpdateBlockedActionPoints 0x007FECB0 (retail 12-byte vectors): FreeActionPointIndexes +0x0c,
//   BlockedActionPointIndexes +0x18, PThing +0x2c.
// Shape: an empty free list returns false before anything else; the draw loop is a do-while on the free list (the
//   aligned loop head); the pick is `index = Free[GFRandom(size, seed)]` with the size read before the seed call; the
//   erase tests `finish != pos + 1` before the memmove; the blocked-list search is the external find with the end
//   pointer in a named local (edi) and the value as a named copy; the owner is a named GetPItem before the virtual call;
//   the overflow insert is a same-TU stand-in as in the landed sibling.
class CThing;
class CBaseObject;
class C3DVector { public: float X, Y, Z; };
class CRightHandedSet { public: C3DVector Forward; C3DVector Up; };

#include <stdlib.h>
#include <string.h>

unsigned long __fastcall GFROR13(unsigned long value);       // 0x00497890
__forceinline unsigned long GFRandom(unsigned long range, unsigned long* seed)
{
    *seed = *seed * 0x24a1 + 0x24df;
    *seed = GFROR13(*seed);
    return range ? *seed % range : 0;
}

struct CIteratorTag { CIteratorTag() { } };
struct __true_type { __true_type() { } };

// Same-TU body of the STLport __find<long*> instantiation 0x0050E540 (as landed). Retail compiled it in this TU, so VC7.1
// sees that the iterator tag is never read, and the empty tag temporary may share the live value's slot.
template <class _RandomAccessIter, class _Tp>
__declspec(noinline) _RandomAccessIter __fastcall Find(_RandomAccessIter first, _RandomAccessIter last, const _Tp& value, const CIteratorTag&)
{
    long count;
    for (count = (last - first) >> 2; 0 < count; --count) {
        if (*first == value) return first;
        ++first;
        if (*first == value) return first;
        ++first;
        if (*first == value) return first;
        ++first;
        if (*first == value) return first;
        ++first;
    }
    switch (last - first) {
    case 3:
        if (*first == value) return first;
        ++first;
    case 2:
        if (*first == value) return first;
        ++first;
    case 1:
        if (*first == value) return first;
    case 0:
    default:
        break;
    }
    return last;
}

struct CLongVector {                                         // STLport vector<long> (12 bytes)
    long* _M_start;
    long* _M_finish;
    long* _M_end_of_storage;
    long* begin() const { return _M_start; }
    long* end() const { return _M_finish; }
    bool empty() const { return _M_start == _M_finish; }
    unsigned long size() const { return _M_finish - _M_start; }
    void _M_insert_overflow(long* position, const long& x, const __true_type& tag, unsigned long n, bool atEnd);   // 0x00440990
    void push_back(const long& x)
    {
        if (_M_finish != _M_end_of_storage) {
            if (_M_finish)
                *_M_finish = x;
            ++_M_finish;
        } else
            _M_insert_overflow(_M_finish, x, __true_type(), 1, true);
    }
    long* erase(long* position)
    {
        long* next = position + 1;
        if (_M_finish != next)
            memmove(position, next, (const char*)_M_finish - (const char*)next);
        --_M_finish;
        return position;
    }
};

#pragma auto_inline(off)
static long* CopyTrivial(const long* first, const long* last, long* result)
{
    if (first == last)
        return result;
    unsigned int n = (const char*)last - (const char*)first;
    return (long*)((char*)memmove(result, first, n) + n);
}
// Same-TU stand-in for vector<long>::_M_insert_overflow 0x00440990 (the landed STLport body, as the sibling).
void CLongVector::_M_insert_overflow(long* position, const long& x, const __true_type&, unsigned long n, bool atEnd)
{
    unsigned long oldSize = _M_finish - _M_start;
    unsigned long len = oldSize + (oldSize < n ? n : oldSize);
    long* newStart = len ? (long*)malloc(len * sizeof(long)) : 0;
    long* newFinish = CopyTrivial(_M_start, position, newStart);
    for (unsigned long i = n; i != 0; --i)
        *newFinish++ = x;
    if (!atEnd)
        newFinish = CopyTrivial(position, _M_finish, newFinish);
    if (_M_start)
        free(_M_start);
    _M_start = newStart;
    _M_finish = newFinish;
    _M_end_of_storage = newStart + len;
}
#pragma auto_inline(on)

struct CBaseIntelligentPointer {
    virtual void v00();
    CBaseObject* m_pItem;                                    // +0x04
    CBaseIntelligentPointer();                               // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                              // 0x00A01C10
    CThing* GetPItem() const;                                // 0x00A01B50
    void SetPItem(CThing* item);                             // 0x00A01B90
};
struct CIntelligentPointer : CBaseIntelligentPointer {       // CIntelligentPointer<CThing>, vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointer(CThing* item = 0) { SetPItem(item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other)
    {
        CThing* item = other.GetPItem();
        SetPItem(item);
        return *this;
    }
};
class CWorld {
public:
    unsigned long* GetRandomSeed() const;                    // 0x0049D810
};

struct CTCUserActionPointBase {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24();
    virtual void v25(); virtual void v26(); virtual void v27();
    virtual bool IsActionPointIndexBlocked(CThing& thing, C3DVector& position, CRightHandedSet& rh, long index) const;   // slot 28 (+0x70)
    CThing*       ParentThing;                               // +0x04 (CTCBase)
    unsigned char _pad_0x08[4];
    CLongVector   FreeActionPointIndexes;                    // +0x0c
    CLongVector   BlockedActionPointIndexes;                 // +0x18
    long          NumberOfActionPoints;                      // +0x24
    long          FrameOfNextCheck;                          // +0x28
    CIntelligentPointer PThing;                              // +0x2c
    const CWorld& PeekWorld() const;                         // 0x00686A80 (CTCBase)
    void UpdateBlockedActionPoints();                        // 0x007FECB0
    bool GetFreeActionPoint(CThing& thing, C3DVector& position, CRightHandedSet& rh, long& index);
};

bool CTCUserActionPointBase::GetFreeActionPoint(CThing& thing, C3DVector& position, CRightHandedSet& rh, long& index)
{
    if (FreeActionPointIndexes.empty())
        return false;
    UpdateBlockedActionPoints();
    PThing = CIntelligentPointer(&thing);
    do {
        unsigned long count = FreeActionPointIndexes.size();
        unsigned long pick = GFRandom(count, PeekWorld().GetRandomSeed());
        index = FreeActionPointIndexes._M_start[pick];
        FreeActionPointIndexes.erase(FreeActionPointIndexes._M_start + pick);
        long value = index;
        long* last = BlockedActionPointIndexes._M_finish;
        if (Find(BlockedActionPointIndexes._M_start, last, value, CIteratorTag()) == last) {
            CThing* owner = PThing.GetPItem();
            if (!IsActionPointIndexBlocked(*owner, position, rh, index))
                return true;
            BlockedActionPointIndexes.push_back(index);
        }
    } while (!FreeActionPointIndexes.empty());
    return false;
}

// ---- behaviour test stubs ----
class CThing { public: int id; };
class CBaseObject { public: int dummy; };
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { printf("CHECK failed line %d\n", __LINE__); g_fail = 1; } } while (0)
static unsigned long g_rand[8]; static int g_randPos = 0;
unsigned long __fastcall GFROR13(unsigned long) { return g_rand[g_randPos++]; }
static unsigned long g_seed = 0;
static CWorld g_world;
unsigned long* CWorld::GetRandomSeed() const { return &g_seed; }
const CWorld& CTCUserActionPointBase::PeekWorld() const { return g_world; }
static int g_updates = 0;
void CTCUserActionPointBase::UpdateBlockedActionPoints() { ++g_updates; }
CBaseIntelligentPointer::CBaseIntelligentPointer() : m_pItem(0) { }
CBaseIntelligentPointer::~CBaseIntelligentPointer() { }
CThing* CBaseIntelligentPointer::GetPItem() const { return (CThing*)m_pItem; }
void CBaseIntelligentPointer::SetPItem(CThing* item) { m_pItem = (CBaseObject*)item; }
void CBaseIntelligentPointer::v00() { }
void CIntelligentPointer::v00() { }
#define V(n) void CTCUserActionPointBase::v##n() { }
V(00) V(01) V(02) V(03) V(04) V(05) V(06) V(07) V(08) V(09) V(10) V(11) V(12) V(13) V(14)
V(15) V(16) V(17) V(18) V(19) V(20) V(21) V(22) V(23) V(24) V(25) V(26) V(27)
static long g_blockedNow = -1; static int g_blockQueries = 0; static CThing* g_owner = 0;
bool CTCUserActionPointBase::IsActionPointIndexBlocked(CThing& thing, C3DVector&, CRightHandedSet&, long index) const
{ ++g_blockQueries; g_owner = &thing; return index == g_blockedNow; }
static void SetVec(CLongVector& v, const long* data, int n, int cap)
{ v._M_start = (long*)malloc(cap * sizeof(long)); memcpy(v._M_start, data, n * sizeof(long)); v._M_finish = v._M_start + n; v._M_end_of_storage = v._M_start + cap; }
int main()
{
    CTCUserActionPointBase* p = new CTCUserActionPointBase;
    CThing thing; thing.id = 1; C3DVector pos = { 0, 0, 0 }; CRightHandedSet rh; memset(&rh, 0, sizeof(rh)); long index = -1;
    // empty free list: false, nothing touched
    p->FreeActionPointIndexes._M_start = p->FreeActionPointIndexes._M_finish = p->FreeActionPointIndexes._M_end_of_storage = 0;
    CHECK(!p->GetFreeActionPoint(thing, pos, rh, index)); CHECK(g_updates == 0 && index == -1);
    // free {5,7,9}, blocked {7} (full, so the push_back overflows)
    long freeInit[3] = { 5, 7, 9 }; long blockedInit[1] = { 7 };
    SetVec(p->FreeActionPointIndexes, freeInit, 3, 3); SetVec(p->BlockedActionPointIndexes, blockedInit, 1, 1);
    g_rand[0] = 1; g_rand[1] = 1; g_rand[2] = 0;      // picks 7 (known blocked), then 9 (blocked now), then 5
    g_blockedNow = 9;
    CHECK(p->GetFreeActionPoint(thing, pos, rh, index));
    CHECK(index == 5); CHECK(g_updates == 1); CHECK(g_blockQueries == 2); CHECK(g_owner == &thing);
    CHECK(p->PThing.GetPItem() == &thing);
    CHECK(p->FreeActionPointIndexes._M_finish == p->FreeActionPointIndexes._M_start);
    CHECK(p->BlockedActionPointIndexes._M_finish - p->BlockedActionPointIndexes._M_start == 2);
    CHECK(p->BlockedActionPointIndexes._M_start[0] == 7 && p->BlockedActionPointIndexes._M_start[1] == 9);
    CHECK(g_seed == 0 && g_randPos == 3);
    // all remaining points blocked: false once the free list runs dry
    long free2[2] = { 3, 4 }; SetVec(p->FreeActionPointIndexes, free2, 2, 2); g_randPos = 0; g_rand[0] = 0; g_rand[1] = 0;
    g_blockedNow = 3; CHECK(p->GetFreeActionPoint(thing, pos, rh, index) == true && index == 4);
    long free3[1] = { 9 }; SetVec(p->FreeActionPointIndexes, free3, 1, 1); g_randPos = 0;
    CHECK(!p->GetFreeActionPoint(thing, pos, rh, index));
    if (g_fail) { printf("007fede0_TEST FAIL\n"); return 1; }
    printf("007fede0_TEST PASS\n");
    return 0;
}