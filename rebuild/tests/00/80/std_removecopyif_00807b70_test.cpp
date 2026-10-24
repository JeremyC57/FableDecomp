#include <cstdio>
// Behaviour test for 0x00807B70: STLport remove_copy_if over 28-byte CTargetPoints, dropping the live, non-null
// things that are not enemies of the predicate's thing.
struct CThing { unsigned char Dead : 1; int team; };
struct CBaseIntelligentPointer {
    virtual void v00() { }
    CThing* m_pItem;
    CBaseIntelligentPointer() : m_pItem(0) { }
    CThing* GetPItem() const { return m_pItem; }
    void SetPItem(CThing* item) { m_pItem = item; }
};
template <class T> struct CIntelligentPointer : CBaseIntelligentPointer {
    virtual void v00() { }
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); return *this; }
};
struct C3DVector {
    float X, Y, Z;
    C3DVector() : X(0), Y(0), Z(0) { }
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
};
static bool AreThingsEnemies(CThing* a, CThing* b) { return a->team != b->team; }
struct CTargetPoint {
    CIntelligentPointer<CThing> PThing;
    long Index;
    C3DVector TargetingPosition;
    long FrameRejected;
};
struct CTargetPointIsNotEnemyOfThing {
    CThing* Thing;
    bool operator()(const CTargetPoint& tp) const
    {
        return tp.PThing.GetPItem() != 0 && !tp.PThing.GetPItem()->Dead && !AreThingsEnemies(Thing, tp.PThing.GetPItem());
    }
};
template <class _InputIter, class _OutputIter, class _Predicate>
_OutputIter remove_copy_if(_InputIter __first, _InputIter __last, _OutputIter __result, _Predicate __pred)
{
    for ( ; __first != __last; ++__first)
        if (!__pred(*__first)) { *__result = *__first; ++__result; }
    return __result;
}
int main()
{
    CThing me; me.Dead = 0; me.team = 1;
    CThing t[4];
    for (int i = 0; i < 4; ++i) { t[i].Dead = 0; t[i].team = 1; }
    t[1].team = 2;                       // enemy: kept
    t[2].team = 1; t[2].Dead = 1;        // dead friend: kept
    // t[0], t[3]: live friends -> removed
    CTargetPoint v[6];
    CThing* who[6] = { &t[1], &t[0], 0, &t[2], &t[3], &t[1] };
    for (int i = 0; i < 6; ++i) {
        v[i].PThing.SetPItem(who[i]); v[i].Index = i; v[i].FrameRejected = 10 + i;
        v[i].TargetingPosition.X = (float)i; v[i].TargetingPosition.Y = 2.0f * i; v[i].TargetingPosition.Z = 3.0f * i;
    }
    CTargetPointIsNotEnemyOfThing pred; pred.Thing = &me;
    CTargetPoint* e = remove_copy_if(v + 2, v + 6, v + 1, pred);    // from the first removable entry
    if (e != v + 4) { std::printf("00807b70_TEST FAIL\n"); return 1; }
    if (v[1].PThing.GetPItem() != 0 || v[1].Index != 2 || v[1].FrameRejected != 12 || v[1].TargetingPosition.Z != 6.0f
        || v[2].PThing.GetPItem() != &t[2] || v[2].Index != 3 || v[2].TargetingPosition.Y != 6.0f
        || v[3].PThing.GetPItem() != &t[1] || v[3].Index != 5 || v[3].TargetingPosition.X != 5.0f || v[3].FrameRejected != 15) {
        std::printf("00807b70_TEST FAIL\n"); return 1;
    }
    std::printf("00807b70_TEST PASS\n");
    return 0;
}