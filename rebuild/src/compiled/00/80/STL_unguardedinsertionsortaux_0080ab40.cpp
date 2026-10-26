// std::__unguarded_insertion_sort_aux(NPlayerTargeting::CTargetPoint* first, NPlayerTargeting::CTargetPoint* last, NPlayerTargeting::CTargetPoint*, CThingCompare_BestForTargeting2 comp) -- unguarded insertion sort of the player-targeting candidate points: each point, copied by value, goes through the unguarded linear insert with the 0x34-byte targeting comparator copied by value
// Leaf: ??$__unguarded_insertion_sort_aux@
// Retail: Fable.exe (TLC, VC7.1) @ 0x0080AB40, 205 bytes, __fastcall (first ecx, last edx; the null value-type pointer and
//   the 0x34-byte comparator on the stack, `ret 0x38`).
// Callers: one, 0x0080B196 (the targeting sort's __final_insertion_sort; the manifest names that caller
//   `_Insertion_sort<CThing**, CThingCompare_BestForTargeting2>`, propagated across the shape).
// Callees: __unguarded_linear_insert(last, val, comp) 0x0080A340; the inline CIntelligentPointer<CThing> copy constructor:
//   CBaseIntelligentPointer::CBaseIntelligentPointer 0x00A01B10, vtable 0x01238C6C, GetPItem 0x00A01B50, SetPItem 0x00A01B90
//   (the landed 0x008C7D80 / 0x008C5D40 model).
// Element: Ego_r.pdb NPlayerTargeting::CTargetPoint (28 bytes): CIntelligentPointer<CThing> PThing +0x0, long Index +0x8,
//   C3DVector TargetingPosition +0xC, long FrameRejected +0x18 -- the pointer re-registered, then five dword moves.
// Comparator: Ego_r.pdb CThingCompare_BestForTargeting2 (52 bytes): CThing* Hero +0x0, CVectorCompare_BestForTargeting
//   VectorCompare +0x4 {C3DVector AnchorPos, PreferredLine; float AngleCompare, SqrDistCompare,
//   OverrideShortDistanceFactor, OverrideShortDistanceFalloff; bool CompareInXYPlane +0x28}, float EnemyPreferenceFactor
//   +0x30; C3DVector's memberwise user copy constructor makes the by-value copy memberwise (the bool moved as a byte).
// Label: catalog "vector<std::pair<long ...>::_Destroy" is propagated; the body is STLport's sort helper.
// Shape: STLport 4.6 `for (__i = __first; __i != __last; ++__i) __unguarded_linear_insert(__i, _Tp(*__i), __comp);` -- the
//   comparator then the element are copy-constructed in their outgoing argument slots; operator() stays declaration-only.
class CThing;
class CBaseObjectPointer;

class C3DVector {                               // retail C3DVector: memberwise user copy constructor
public:
    float X;                                    // +0x0
    float Y;                                    // +0x4
    float Z;                                    // +0x8
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
};

class CBaseObject;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};

template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {   // CIntelligentPointer<CThing>: vtable 0x01238C6C
public:
    virtual ~CIntelligentPointer();
    CIntelligentPointer(const CIntelligentPointer<T>& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); }

    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};

namespace NPlayerTargeting {
class CTargetPoint {                            // Ego_r.pdb, 28 bytes
public:
    CIntelligentPointer<CThing> PThing;         // +0x00
    long      Index;                            // +0x08
    C3DVector TargetingPosition;                // +0x0C
    long      FrameRejected;                    // +0x18
};
}

class CVectorCompare_BestForTargeting {         // Ego_r.pdb, 44 bytes
public:
    C3DVector AnchorPos;                        // +0x00
    C3DVector PreferredLine;                    // +0x0C
    float     AngleCompare;                     // +0x18
    float     SqrDistCompare;                   // +0x1C
    float     OverrideShortDistanceFactor;      // +0x20
    float     OverrideShortDistanceFalloff;     // +0x24
    bool      CompareInXYPlane;                 // +0x28
};

class CThingCompare_BestForTargeting2 {         // Ego_r.pdb, 52 bytes
public:
    CThing*                         Hero;                   // +0x00
    CVectorCompare_BestForTargeting VectorCompare;          // +0x04
    float                           EnemyPreferenceFactor;  // +0x30
    bool operator()(const NPlayerTargeting::CTargetPoint& a, const NPlayerTargeting::CTargetPoint& b) const;
};

namespace std {
template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __unguarded_linear_insert(_RandomAccessIter __last, _Tp __val, _Compare __comp);   // 0x0080A340

template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __unguarded_insertion_sort_aux(_RandomAccessIter __first, _RandomAccessIter __last, _Tp*, _Compare __comp)
{
    for (_RandomAccessIter __i = __first; __i != __last; ++__i)
        __unguarded_linear_insert(__i, _Tp(*__i), __comp);
}

template void __fastcall __unguarded_insertion_sort_aux(NPlayerTargeting::CTargetPoint*, NPlayerTargeting::CTargetPoint*, NPlayerTargeting::CTargetPoint*, CThingCompare_BestForTargeting2);
}