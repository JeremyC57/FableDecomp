// CTCUserActionPointBase::UpdateBlockedActionPoints() -- at most every 10 frames, re-tests every blocked action point: those still blocked (for a live owner) stay blocked, the rest go back to the free list
// Leaf: UpdateBlockedActionPoints
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007FECB0, 291 bytes (plain `ret`; Ego_r.pdb
// `?UpdateBlockedActionPoints@CTCUserActionPointBase@@IAEXXZ`).
// Callees: GFGetCurrentFrame() 0x0049D870 (x2, `__fastcall` with no arguments); CBaseIntelligentPointer::GetPItem()
//          0x00A01B50 (x3); IsActionPointIndexBlocked(CThing&, C3DVector&, CRightHandedSet&, long) const (virtual slot 28,
//          `call [edx+70h]`); vector<long>::_M_insert_overflow 0x00440990 (both push_backs); vector<long>::operator=
//          0x00454886; free 0x00BFEA14 (the local vector's storage).
// Layout (retail, 12-byte vectors): FreeActionPointIndexes +0x0c, BlockedActionPointIndexes +0x18, NumberOfActionPoints
//          +0x24, FrameOfNextCheck +0x28, PThing +0x2c (PDB +0x34).
// Shape: the still-blocked list is a local vector whose finish pointer stays in ebp (reloaded after an overflow insert);
//        the index is copied into a named local (the push_back reference); the live owner is fetched into a named
//        CThing* before the virtual test (GetPItem runs ahead of the argument pushes) and the blocked arm `continue`s; the position and frame out-locals are
//        uninitialised function-scope locals; the two out-of-line vector<long> members are this TU's view of retail's
//        link-time knowledge, so they are defined above as stand-ins under auto_inline(off): the overflow insert as the
//        landed STLport body (CWorldMap::LoadTopologyFromFile 0x004FF080), operator= as the STLport assignment (it only
//        reads its argument, so the local's start pointer survives the call in edi).
class CThing;
struct CBaseObject;
struct C3DVector { float X, Y, Z; };
struct CRightHandedSet { C3DVector Forward; C3DVector Up; };

extern "C" void* __cdecl malloc(unsigned int size);         // 0x00BFEA0E
extern "C" void __cdecl free(void* block);                   // 0x00BFEA14
extern "C" void* __cdecl memmove(void* dest, const void* src, unsigned int count);   // 0x00BFEAE6
long __fastcall GFGetCurrentFrame();            // 0x0049D870

struct __true_type { __true_type() { } };
struct CLongVector {                            // STLport vector<long> (12 bytes)
    long* _M_start;
    long* _M_finish;
    long* _M_end_of_storage;
    CLongVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
    ~CLongVector() { if (_M_start) free(_M_start); }
    long* begin() const { return _M_start; }
    long* end() const { return _M_finish; }
    bool empty() const { return _M_start == _M_finish; }
    void _M_insert_overflow(long* position, const long& x, const __true_type& tag, unsigned long n, bool atEnd);   // 0x00440990
    CLongVector& operator=(const CLongVector& other);   // 0x00454886
    void push_back(const long& x)
    {
        if (_M_finish != _M_end_of_storage) {
            if (_M_finish)
                *_M_finish = x;
            ++_M_finish;
        } else
            _M_insert_overflow(_M_finish, x, __true_type(), 1, true);
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
CLongVector& CLongVector::operator=(const CLongVector& other)
{
    if (&other != this) {
        unsigned long length = other._M_finish - other._M_start;
        if (length > (unsigned long)(_M_end_of_storage - _M_start)) {
            long* storage = length ? (long*)malloc(length * sizeof(long)) : 0;
            CopyTrivial(other._M_start, other._M_finish, storage);
            if (_M_start)
                free(_M_start);
            _M_start = storage;
            _M_end_of_storage = storage + length;
        } else if ((unsigned long)(_M_finish - _M_start) >= length) {
            CopyTrivial(other._M_start, other._M_finish, _M_start);
        } else {
            unsigned long size = _M_finish - _M_start;
            CopyTrivial(other._M_start, other._M_start + size, _M_start);
            CopyTrivial(other._M_start + size, other._M_finish, _M_finish);
        }
        _M_finish = _M_start + length;
    }
    return *this;
}
#pragma auto_inline(on)

class CThing {
public:
    unsigned char _pad[0x91];
    unsigned char m_flags;                      // +0x91 bit 0: dead
    bool IsDead() const { return (m_flags & 1) != 0; }
};
struct CBaseIntelligentPointer {
    void*        __vftable;                     // +0x00
    CBaseObject* m_pItem;                       // +0x04
    CThing* GetPItem() const;                   // 0x00A01B50
};
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!p.GetPItem()->IsDead())
            return true;
    }
    return false;
}

struct CTCUserActionPointBase {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24();
    virtual void v25(); virtual void v26(); virtual void v27();
    virtual bool IsActionPointIndexBlocked(CThing& thing, C3DVector& position, CRightHandedSet& rh, long index) const;   // slot 28 (+0x70)
    CThing*       ParentThing;                  // +0x04 (CTCBase)
    unsigned char _pad_0x08[4];
    CLongVector   FreeActionPointIndexes;       // +0x0c
    CLongVector   BlockedActionPointIndexes;    // +0x18
    long          NumberOfActionPoints;         // +0x24
    long          FrameOfNextCheck;             // +0x28
    CBaseIntelligentPointer PThing;             // +0x2c
    void UpdateBlockedActionPoints();
};

void CTCUserActionPointBase::UpdateBlockedActionPoints()
{
    if (BlockedActionPointIndexes.empty())
        return;
    if (GFGetCurrentFrame() <= FrameOfNextCheck)
        return;
    CLongVector stillBlocked;
    C3DVector position;
    CRightHandedSet rh;
    for (long* it = BlockedActionPointIndexes.begin(); it != BlockedActionPointIndexes.end(); ++it) {
        long index = *it;
        if (IS_THING_ALIVE(PThing)) {
            CThing* thing = PThing.GetPItem();
            if (IsActionPointIndexBlocked(*thing, position, rh, index)) {
                stillBlocked.push_back(index);
                continue;
            }
        }
        FreeActionPointIndexes.push_back(index);
    }
    BlockedActionPointIndexes = stillBlocked;
    FrameOfNextCheck = GFGetCurrentFrame() + 10;
}