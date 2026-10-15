// Behaviour test for 0x00B09F90 CEnginePrimitiveSpriteTrail::CEnginePrimitiveSpriteTrail
#include <cstdio>
#include <cstdlib>
struct CEnginePrimitiveThemeChannelDesc { unsigned char C0, C1, F, Pad; CEnginePrimitiveThemeChannelDesc() : C0(0), C1(0), F(0) {} };
struct CEnginePrimitive { long Type; long InterpolationSet; CEnginePrimitiveThemeChannelDesc Theme; CEnginePrimitive(long t) : Type(t), InterpolationSet(0) {} };
struct C3DVector { float X, Y, Z; };
struct CMatrix3x4 {
    C3DVector XAxis, YAxis, ZAxis, Position;
    CMatrix3x4() { XAxis.Y = XAxis.Z = YAxis.X = YAxis.Z = ZAxis.X = ZAxis.Y = Position.X = Position.Y = Position.Z = 0.0f; XAxis.X = YAxis.Y = ZAxis.Z = 1.0f; }
};
struct SZeroVector { long x, y, z; SZeroVector() : x(0), y(0), z(0) {} };
struct CList {
    struct SNode { SNode* next; SNode* prev; float v[4]; };
    SNode* head;
    CList() { head = 0; SNode* n = (SNode*)std::malloc(sizeof(SNode)); n->next = n; n->prev = n; head = n; }
    ~CList() { std::free(head); }
};
struct CEnginePrimitiveSpriteTrail : CEnginePrimitive {
    CList Trail; long FadeStart; SZeroVector OffsetVector; CMatrix3x4 WorldTransform;
    CEnginePrimitiveSpriteTrail() : CEnginePrimitive(0x14), FadeStart(0) {}
};
int main()
{
    CEnginePrimitiveSpriteTrail t;
    bool ok = t.Type == 0x14 && t.InterpolationSet == 0 && t.Theme.C0 == 0 && t.Theme.C1 == 0 && t.Theme.F == 0
        && t.Trail.head != 0 && t.Trail.head->next == t.Trail.head && t.Trail.head->prev == t.Trail.head
        && t.FadeStart == 0 && t.OffsetVector.x == 0 && t.OffsetVector.z == 0
        && t.WorldTransform.XAxis.X == 1.0f && t.WorldTransform.YAxis.Y == 1.0f && t.WorldTransform.ZAxis.Z == 1.0f
        && t.WorldTransform.XAxis.Y == 0.0f && t.WorldTransform.Position.Z == 0.0f;
    if (!ok) { std::printf("00b09f90_TEST FAIL\n"); return 1; }
    std::printf("00b09f90_TEST PASS\n");
    return 0;
}
