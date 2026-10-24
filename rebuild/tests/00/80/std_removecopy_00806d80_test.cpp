// behaviour test for 0x00806D80 (STLport remove_copy over CTargetPoint records)
#include <stdio.h>
#ifndef _MSC_VER
#define __fastcall
#endif
struct CBaseObject { int x; };
struct CThing : CBaseObject {};
struct CBaseIntelligentPointer {
    CBaseObject* PData;
    CBaseIntelligentPointer() : PData(0) {}
    virtual ~CBaseIntelligentPointer() {}
    CBaseObject* GetPItem() const { return PData; }
    void SetPItem(CBaseObject* item) { PData = item; }
};
template <class T>
struct CIntelligentPointer : CBaseIntelligentPointer {
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); return *this; }
    bool operator==(const CIntelligentPointer& other) const { return GetPItem() == other.GetPItem(); }
};
struct C3DVector {
    float X, Y, Z;
    C3DVector() : X(0), Y(0), Z(0) {}
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
};
struct CTargetPoint {
    CIntelligentPointer<CThing> PThing;
    long                        Index;
    C3DVector                   TargetingPosition;
    long                        FrameRejected;
    CTargetPoint& operator=(const CTargetPoint& other)
    {
        PThing = other.PThing;
        Index = other.Index;
        TargetingPosition = other.TargetingPosition;
        FrameRejected = other.FrameRejected;
        return *this;
    }
    bool operator==(const CTargetPoint& other) const { return PThing == other.PThing && Index == other.Index; }
};
template <class InputIter, class OutputIter, class T>
OutputIter __fastcall remove_copy(InputIter first, InputIter last, OutputIter result, const T& val)
{
    for ( ; first != last; ++first)
        if (!(*first == val)) {
            *result = *first;
            ++result;
        }
    return result;
}
int main()
{
    CThing a, b;
    CTargetPoint p[5];
    CThing* things[5] = { &a, &b, &a, &a, &b };
    long idx[5] = { 1, 1, 2, 1, 7 };
    for (int i = 0; i < 5; ++i) { p[i].PThing.SetPItem(things[i]); p[i].Index = idx[i]; p[i].TargetingPosition.X = (float)i; p[i].FrameRejected = 100 + i; }
    CTargetPoint val; val.PThing.SetPItem(&a); val.Index = 1;              // equal to p[0] and p[3] only
    CTargetPoint* end = remove_copy(p, p + 5, p, val);
    int ok = end == p + 3
          && p[0].PThing.GetPItem() == &b && p[0].Index == 1 && p[0].TargetingPosition.X == 1.0f && p[0].FrameRejected == 101
          && p[1].PThing.GetPItem() == &a && p[1].Index == 2 && p[1].TargetingPosition.X == 2.0f && p[1].FrameRejected == 102
          && p[2].PThing.GetPItem() == &b && p[2].Index == 7 && p[2].TargetingPosition.X == 4.0f && p[2].FrameRejected == 104
          && remove_copy(p, p, p + 1, val) == p + 1;
    if (!ok) { printf("00806d80_TEST FAIL\n"); return 1; }
    printf("00806d80_TEST PASS\n");
    return 0;
}