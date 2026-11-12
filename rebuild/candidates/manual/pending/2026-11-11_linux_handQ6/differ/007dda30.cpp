// CTCCreatureFlyInCircles::QueueSwoop(const CThingCreatureBase& target, const C3DVector& start, const C3DVector& middle, const C3DVector& end) -- remember the swoop target and offset, set the swoop speed from the path length, rebuild the swoop spline (start, middle, end offset by the target's position), clear the 0x3a component's first flag and move the creature to the start
// Grade: RELOCATION_MATCH(642v642) on LOCAL models only -- parked for header debt, not reworked onto shared headers in this pass. Missing shared pieces: (1) no engine/CCardinalSpline.h (Ego_r.pdb 24 bytes: CArray<C3DVector> ControlPoints, float Tension, bool EndPointsConnected; retail 0x14) with inline DeleteAllControlPoints / AppendControlPoint, and no engine/CTCCreatureFlyInCircles.h (Ego_r layout: Mode +0x34, PSwoopTarget +0x44, SwoopSpline +0x4c, SwoopTargOffset +0x60, SwoopProgress +0x6c, SwoopDelta +0x70); (2) the shared vector<C3DVector> can't express retail's pair of callees: _M_insert_overflow 0x0066E370 (inline element copies, only a malloc call) and clear() through the __fastcall random-access __copy 0x0074CA20, because the shared __type_traits<C3DVector> is all __false_type and erase uses __copy_ptrs; (3) the load-bearing PAREN form of C3DVector operator- (X term parenthesised, 0x007DDB16) has no shared engine/C3DVector.h opt-in; (4) CThing::GetInterface, CIntelligentPointer<const CThingCreatureBase> and the physics TC's slot-31 SetPosition are local; next: add those as opt-ins and new headers, keeping the same-TU overflow + __copy bodies visible
// Identity: Ego_r.pdb / FableWin.pdb ?QueueSwoop@CTCCreatureFlyInCircles@@QAEXABVCThingCreatureBase@@ABVC3DVector@@11@Z; members by Ego_r layout.
// Leaf: ?QueueSwoop@CTCCreatureFlyInCircles@@QAEXABVCThingCreatureBase@@ABVC3DVector@@11@Z
//
// Retail 0x007DDA30, 640 bytes (`ret 0x10`).
// Callees: CBaseIntelligentPointer ctor @ 0x00A01B10 / SetPItem @ 0x00A01B90 / GetPItem @ 0x00A01B50 / dtor @ 0x00A01C10
// (the temporary's vtable 0x0125AAEC), CThing::GetPosition @ 0x004C73D0, STLport __copy<C3DVector> @ 0x0074CA20 (__fastcall,
// the clear), vector<C3DVector>::_M_insert_overflow @ 0x0066E370, CVectorMap::LowerBound @ 0x0040F020 (interface 0x3a),
// physics slot 31 (+0x7c, set position). Constant: the swoop distance @ 0x0126FA6C.
// Levers: (1) same-TU out-of-line bodies of the vector overflow 0x0066E370 and the STLport __copy 0x0074CA20 (retail's TU
//   holds them): the pushed value and the clear()'s tag temporaries are then not captured, giving retail's interleaved
//   placement copies and tag slots; (2) the intelligent-pointer assignment reads the item into a named local first
//   (shared engine/CIntelligentPointer.h form); (3) C3DVector subtraction groups its X difference in parentheses, the no-code
//   PAREN tuple that puts the clear()'s `push 0` between the second magnitude's `fld st(4)` and `fmul st(5)` (0x007DDB16).
// Header debt: local models (the CCardinalSpline control-point vector is modelled as a flat vector, its DeleteAllControlPoints /
//   AppendControlPoint expanded as clear / push_back; __copy as a named fastcall function).
// Layout (local model, Ego_r.pdb names): Creature +0x0c, Mode +0x34, Transitioning +0x38, PSwoopTarget +0x44, SwoopSpline
// +0x4c (ControlPoints vector), SwoopTargOffset +0x60, SwoopProgress +0x6c, SwoopDelta +0x70; the 0x3a component's flags +0x50.
#include <math.h>
#pragma intrinsic(sqrt)
class C3DVector {
public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    float GetSquaredMagnitude() const { return X * X + Y * Y + Z * Z; }
    float GetMagnitude() const { return (float)sqrt(GetSquaredMagnitude()); }
};
inline C3DVector operator+(const C3DVector& a, const C3DVector& b) { return C3DVector(a.X + b.X, a.Y + b.Y, a.Z + b.Z); }
inline C3DVector operator-(const C3DVector& a, const C3DVector& b) { return C3DVector((a.X - b.X), a.Y - b.Y, a.Z - b.Z); }
inline void* operator new(unsigned int, void* where) { return where; }
struct __true_type { __true_type() { } };
struct SRandomAccessIteratorTag { SRandomAccessIteratorTag() { } };
C3DVector* __fastcall CopyPositions(const C3DVector* first, const C3DVector* last, C3DVector* dest,
                                    const SRandomAccessIteratorTag& tag, long* distance);   // 0x0074CA20
struct CPositionVector {                                     // STLport vector<C3DVector>
    C3DVector* _M_start;
    C3DVector* _M_finish;
    C3DVector* _M_end_of_storage;
    C3DVector* begin() const { return _M_start; }
    C3DVector* end() const { return _M_finish; }
    void _M_insert_overflow(C3DVector* position, const C3DVector& x, const __true_type& tag, unsigned long fillLength,
                            bool atEnd);                     // 0x0066E370
    C3DVector* erase(C3DVector* first, C3DVector* last)
    {
        C3DVector* i = CopyPositions(last, _M_finish, first, SRandomAccessIteratorTag(), 0);
        _M_finish = i;
        return first;
    }
    void clear() { erase(begin(), end()); }
    void push_back(const C3DVector& x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) C3DVector(x);
            ++_M_finish;
        } else
            _M_insert_overflow(_M_finish, x, __true_type(), 1, true);
    }
};
// Same-TU out-of-line bodies (retail's TU holds them): STLport 4.6 vector::_M_insert_overflow (__true_type) and __copy.
extern "C" void* __cdecl malloc(unsigned int);
extern "C" void __cdecl free(void*);
#pragma auto_inline(off)
void CPositionVector::_M_insert_overflow(C3DVector* position, const C3DVector& x, const __true_type&, unsigned long fillLength,
                                         bool atEnd)
{
    unsigned long oldSize = _M_finish - _M_start;
    unsigned long len = oldSize + (oldSize > fillLength ? oldSize : fillLength);
    C3DVector* newStart = (C3DVector*)malloc(len * sizeof(C3DVector));
    C3DVector* newFinish = newStart;
    for (C3DVector* p = _M_start; p != position; ++p, ++newFinish)
        *newFinish = *p;
    for (unsigned long n = fillLength; n > 0; --n, ++newFinish)
        *newFinish = x;
    if (!atEnd)
        for (C3DVector* p = position; p != _M_finish; ++p, ++newFinish)
            *newFinish = *p;
    free(_M_start);
    _M_start = newStart;
    _M_finish = newFinish;
    _M_end_of_storage = newStart + len;
}
C3DVector* __fastcall CopyPositions(const C3DVector* first, const C3DVector* last, C3DVector* dest,
                                    const SRandomAccessIteratorTag&, long*)
{
    for (long n = last - first; n > 0; --n) {
        *dest = *first;
        ++first;
        ++dest;
    }
    return dest;
}
#pragma auto_inline(on)
#define VF(n) virtual void v##n();
struct CTCPhysicsBase {
    VF(0) VF(1) VF(2) VF(3) VF(4) VF(5) VF(6) VF(7) VF(8) VF(9) VF(10) VF(11) VF(12) VF(13) VF(14) VF(15) VF(16) VF(17) VF(18)
    VF(19) VF(20) VF(21) VF(22) VF(23) VF(24) VF(25) VF(26) VF(27) VF(28) VF(29) VF(30)
    virtual void SetPosition(const C3DVector& pos);          // slot 31 (+0x7c)
};
#undef VF
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);                    // 0x0040F020
};
struct CTC3A {
    unsigned char _pad00[0x50];
    unsigned char Flags;                                     // +0x50
};
class CThing {
public:
    unsigned char   _pad0[0x20];
    unsigned long   m_presentBits[9];                        // +0x20
    CInterfaceMap   m_interfaces;                            // +0x44
    unsigned char   _pad4c[0x60 - 0x4c];
    CTCPhysicsBase* PhysicsTC;                               // +0x60
    const C3DVector& GetPosition() const;                    // 0x004C73D0
    template <class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)(it == map->last || interfaceType < it->type ? map->last : it)->component;
            return true;
        }
        return false;
    }
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* PItem;                                           // +0x04
    CBaseIntelligentPointer();                               // 0x00A01B10
    ~CBaseIntelligentPointer();                              // 0x00A01C10
    CThing* GetPItem() const;                                // 0x00A01B50
    void SetPItem(CThing* item);                             // 0x00A01B90
};
struct CIntelligentPointerTarget : CBaseIntelligentPointer {  // vtable 0x0125AAEC
    virtual void v00();
    CIntelligentPointerTarget(CThing* item) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentPointerTarget& operator=(const CIntelligentPointerTarget& other) { CThing* item = other.GetPItem(); SetPItem(item); return *this; }
};
class CThingCreatureBase : public CThing { };
extern float g_swoopDistance;                                // 0x0126FA6C
struct CTCCreatureFlyInCircles {
    virtual void v00();
    CThing*       ParentThing;                               // +0x04
    unsigned char _pad08[0x0c - 0x08];
    CThing*       Creature;                                  // +0x0c
    unsigned char _pad10[0x34 - 0x10];
    long          Mode;                                      // +0x34 EFlyMode
    bool          Transitioning;                             // +0x38
    unsigned char _pad39[0x44 - 0x39];
    CIntelligentPointerTarget PSwoopTarget;                  // +0x44 CIntelligentPointer<const CThingCreatureBase>
    CPositionVector SwoopSpline;                             // +0x4c CCardinalSpline (its ControlPoints vector)
    unsigned char _pad58[0x60 - 0x58];
    C3DVector     SwoopTargOffset;                           // +0x60
    float         SwoopProgress;                             // +0x6c
    float         SwoopDelta;                                // +0x70
    void QueueSwoop(const CThingCreatureBase& target, const C3DVector& start, const C3DVector& middle, const C3DVector& end);
};

void CTCCreatureFlyInCircles::QueueSwoop(const CThingCreatureBase& target, const C3DVector& start, const C3DVector& middle,
                                         const C3DVector& end)
{
    PSwoopTarget = CIntelligentPointerTarget((CThing*)&target);
    Mode = 2;
    Transitioning = false;
    SwoopTargOffset = middle;
    C3DVector targetPos = PSwoopTarget.GetPItem()->GetPosition();
    SwoopProgress = 0.0f;
    SwoopDelta = g_swoopDistance / ((end - middle).GetMagnitude() + (middle - start).GetMagnitude());
    SwoopSpline.clear();
    SwoopSpline.push_back(targetPos + start);
    SwoopSpline.push_back(targetPos + middle);
    SwoopSpline.push_back(targetPos + end);
    CTC3A* tc;
    if (Creature->GetInterface(&tc, 0x3a))
        tc->Flags &= ~1;
    Creature->PhysicsTC->SetPosition(start);
}
