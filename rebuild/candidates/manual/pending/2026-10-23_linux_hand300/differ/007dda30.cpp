// CTCCreatureFlyInCircles::QueueSwoop(CThing* target, const C3DVector& start, const C3DVector& middle, const C3DVector& end)
// Grade: DIFFER(642v642) -- the push_back placement copy interleaves loads in retail (ours sequential), empty tag temps homed in end's slot (ours target's), one lea before GetPItem
// -- remember the target and the swoop centre, set the swoop speed from the path length, rebuild the swoop path (start,
// middle, end offset by the target's position), clear the 0x3a component's first flag and move the creature to the start
// Leaf: QueueSwoop
//
// Retail 0x007DDA30, 640 bytes (`ret 0x10`).
// Callees: CBaseIntelligentPointer ctor @ 0x00A01B10 / SetPItem @ 0x00A01B90 / GetPItem @ 0x00A01B50 / dtor @ 0x00A01C10
// (the temporary's vtable 0x0125AAEC), CThing::GetPosition @ 0x004C73D0, STLport __copy<C3DVector> @ 0x0074CA20 (__fastcall,
// the clear), vector<C3DVector>::_M_insert_overflow @ 0x0066E370, CVectorMap::LowerBound @ 0x0040F020 (interface 0x3a),
// physics slot 31 (+0x7c, set position). Constant: the swoop distance @ 0x0126FA6C.
// Layout (local model): Creature +0x0c, SwoopState +0x34, SwoopFlag +0x38, PTarget +0x44 (CIntelligentPointer), SwoopPath
// +0x4c (vector<C3DVector>), SwoopCentre +0x60, SwoopProgress +0x6c, SwoopSpeed +0x70; the 0x3a component's flags +0x50.
#include <math.h>
#pragma intrinsic(sqrt)
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    float Magnitude() const { return (float)sqrt(X * X + Y * Y + Z * Z); }
};
inline C3DVector operator+(const C3DVector& a, const C3DVector& b) { return C3DVector(a.X + b.X, a.Y + b.Y, a.Z + b.Z); }
inline C3DVector operator-(const C3DVector& a, const C3DVector& b) { return C3DVector(a.X - b.X, a.Y - b.Y, a.Z - b.Z); }
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
struct CThing {
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
    CIntelligentPointerTarget& operator=(const CIntelligentPointerTarget& other) { SetPItem(other.GetPItem()); return *this; }
};
extern float g_swoopDistance;                                // 0x0126FA6C
struct CTCCreatureFlyInCircles {
    virtual void v00();
    CThing*       ParentThing;                               // +0x04
    unsigned char _pad08[0x0c - 0x08];
    CThing*       Creature;                                  // +0x0c
    unsigned char _pad10[0x34 - 0x10];
    long          SwoopState;                                // +0x34
    bool          SwoopFlag;                                 // +0x38
    unsigned char _pad39[0x44 - 0x39];
    CIntelligentPointerTarget PTarget;                       // +0x44
    CPositionVector SwoopPath;                               // +0x4c
    unsigned char _pad58[0x60 - 0x58];
    C3DVector     SwoopCentre;                               // +0x60
    float         SwoopProgress;                             // +0x6c
    float         SwoopSpeed;                                // +0x70
    void QueueSwoop(CThing* target, const C3DVector& start, const C3DVector& middle, const C3DVector& end);
};

void CTCCreatureFlyInCircles::QueueSwoop(CThing* target, const C3DVector& start, const C3DVector& middle, const C3DVector& end)
{
    PTarget = CIntelligentPointerTarget(target);
    SwoopState = 2;
    SwoopFlag = false;
    SwoopCentre = middle;
    C3DVector targetPos = PTarget.GetPItem()->GetPosition();
    SwoopProgress = 0.0f;
    SwoopSpeed = g_swoopDistance / ((end - middle).Magnitude() + (middle - start).Magnitude());
    SwoopPath.clear();
    SwoopPath.push_back(targetPos + start);
    SwoopPath.push_back(targetPos + middle);
    SwoopPath.push_back(targetPos + end);
    CTC3A* tc;
    if (Creature->GetInterface(&tc, 0x3a))
        tc->Flags &= ~1;
    Creature->PhysicsTC->SetPosition(start);
}
