// behaviour test for 007fecb0 CTCUserActionPointBase::UpdateBlockedActionPoints: after the check frame, blocked points that
// are still blocked for a live owner stay blocked, the rest go back to the free list; the next check is 10 frames on
#include <cstdio>
#include <cstdlib>
#include <cstring>
class CThing;
struct CBaseObject;
struct C3DVector { float X, Y, Z; };
struct CRightHandedSet { C3DVector Forward; C3DVector Up; };
static long g_frame = 100;
long GFGetCurrentFrame() { return g_frame; }
struct __true_type { __true_type() { } };
struct CLongVector {
    long* _M_start;
    long* _M_finish;
    long* _M_end_of_storage;
    CLongVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
    ~CLongVector() { if (_M_start) std::free(_M_start); }
    long* begin() const { return _M_start; }
    long* end() const { return _M_finish; }
    bool empty() const { return _M_start == _M_finish; }
    void _M_insert_overflow(long* position, const long& x, const __true_type&, unsigned long n, bool)
    {
        unsigned long oldSize = _M_finish - _M_start;
        unsigned long len = oldSize + (oldSize < n ? n : oldSize);
        long* newStart = (long*)std::malloc(len * sizeof(long));
        if (oldSize) std::memcpy(newStart, _M_start, (position - _M_start) * sizeof(long));
        long* newFinish = newStart + (position - _M_start);
        *newFinish++ = x;
        if (_M_start) std::free(_M_start);
        _M_start = newStart; _M_finish = newFinish; _M_end_of_storage = newStart + len;
    }
    CLongVector& operator=(const CLongVector& other)
    {
        if (&other != this) {
            unsigned long length = other._M_finish - other._M_start;
            long* storage = length ? (long*)std::malloc(length * sizeof(long)) : 0;
            if (length) std::memcpy(storage, other._M_start, length * sizeof(long));
            if (_M_start) std::free(_M_start);
            _M_start = storage; _M_finish = storage + length; _M_end_of_storage = storage + length;
        }
        return *this;
    }
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
class CThing {
public:
    unsigned char m_flags;
    bool IsDead() const { return (m_flags & 1) != 0; }
};
struct CBaseIntelligentPointer {
    void*   __vftable;
    CThing* m_pItem;
    CThing* GetPItem() const { return m_pItem; }
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
    virtual bool IsActionPointIndexBlocked(CThing&, C3DVector&, CRightHandedSet&, long index) const { return index % 2 == 0; }
    CLongVector   FreeActionPointIndexes;
    CLongVector   BlockedActionPointIndexes;
    long          NumberOfActionPoints;
    long          FrameOfNextCheck;
    CBaseIntelligentPointer PThing;
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
int main()
{
    CThing owner; owner.m_flags = 0;
    CTCUserActionPointBase points; points.PThing.m_pItem = &owner; points.FrameOfNextCheck = 50;
    for (long i = 1; i <= 4; ++i) points.BlockedActionPointIndexes.push_back(i);
    points.UpdateBlockedActionPoints();
    bool ok = points.FrameOfNextCheck == 110 && points.BlockedActionPointIndexes._M_finish - points.BlockedActionPointIndexes._M_start == 2 &&
              points.BlockedActionPointIndexes._M_start[0] == 2 && points.BlockedActionPointIndexes._M_start[1] == 4 &&
              points.FreeActionPointIndexes._M_finish - points.FreeActionPointIndexes._M_start == 2 &&
              points.FreeActionPointIndexes._M_start[0] == 1 && points.FreeActionPointIndexes._M_start[1] == 3;
    points.UpdateBlockedActionPoints();          // too early: unchanged
    ok = ok && points.FrameOfNextCheck == 110;
    g_frame = 200; owner.m_flags = 1;            // dead owner: everything is freed
    points.UpdateBlockedActionPoints();
    ok = ok && points.BlockedActionPointIndexes.empty() && points.FreeActionPointIndexes._M_finish - points.FreeActionPointIndexes._M_start == 4;
    if (!ok) { std::printf("007fecb0_TEST FAIL\n"); return 1; }
    std::printf("007fecb0_TEST PASS\n");
    return 0;
}