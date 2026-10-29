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

long __fastcall GFROR13(unsigned long);       // 0x00497890
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

class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {       // CIntelligentPointer<CThing>, vtable 0x01238C6C
public:
    CIntelligentPointer(T* item = 0) { SetPItem(item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other)
    {
        T* item = other.GetPItem();
        SetPItem(item);
        return *this;
    }

    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};
class CWorld {
public:
    unsigned long* GetWorldSeed() const;                    // 0x0049D810
};

class CTCBase
{
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24();
    virtual void v25(); virtual void v26(); virtual void v27();
    virtual bool IsActionPointIndexBlocked(CThing& thing, C3DVector& position, CRightHandedSet& rh, long index) const;   // slot 28 (+0x70)
protected:
    const CWorld& PeekWorld() const;   // 0x00686A80
};

struct CTCUserActionPointBase : public CTCBase {
    CThing*       ParentThing;                               // +0x04 (CTCBase)
    unsigned char _pad_0x08[4];
    CLongVector   FreeActionPointIndexes;                    // +0x0c
    CLongVector   BlockedActionPointIndexes;                 // +0x18
    long          NumberOfActionPoints;                      // +0x24
    long          FrameOfNextCheck;                          // +0x28
    CIntelligentPointer<CThing> PThing;                              // +0x2c
    void UpdateBlockedActionPoints();                        // 0x007FECB0
    bool GetFreeActionPoint(CThing& thing, C3DVector& position, CRightHandedSet& rh, long& index);
};

bool CTCUserActionPointBase::GetFreeActionPoint(CThing& thing, C3DVector& position, CRightHandedSet& rh, long& index)
{
    if (FreeActionPointIndexes.empty())
        return false;
    UpdateBlockedActionPoints();
    PThing = CIntelligentPointer<CThing>(&thing);
    do {
        unsigned long count = FreeActionPointIndexes.size();
        unsigned long pick = GFRandom(count, PeekWorld().GetWorldSeed());
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