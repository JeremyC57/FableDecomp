// Behaviour test for CAnimComponentCombatFlourishTargetOffset::TransferBinaryOut @ 0x005db3d0: WriteBool(flag) then Write2DVector(offset at +9).
#include <cstdio>
#include <cstddef>
struct C2DVector { float X, Y; };
struct CDataOutputStream {
    int count; bool flag; float x, y;
    CDataOutputStream() : count(0), flag(false), x(0), y(0) { }
    void WriteBool(bool value) { flag = value; ++count; }
    void Write2DVector(const C2DVector& v) { x = v.X; y = v.Y; count += 10; }
};
#pragma pack(push, 1)
struct CAnimComponentCombatFlourishTargetOffset {
    void* __vftable; long m_refCount; bool TargetOffset; C2DVector TargetOffsetVector;
    void TransferBinaryOut(CDataOutputStream& stream) const { stream.WriteBool(TargetOffset); stream.Write2DVector(TargetOffsetVector); }
};
#pragma pack(pop)
int main()
{
    CAnimComponentCombatFlourishTargetOffset c; c.TargetOffset = true; c.TargetOffsetVector.X = 1.5f; c.TargetOffsetVector.Y = -2.0f;
    CDataOutputStream s; c.TransferBinaryOut(s);
    bool ok = s.count == 11 && s.flag && s.x == 1.5f && s.y == -2.0f
        && offsetof(CAnimComponentCombatFlourishTargetOffset, TargetOffsetVector) == 9 && sizeof(c) == 0x11;
    if (!ok) { std::printf("005db3d0_TEST FAIL\n"); return 1; }
    std::printf("005db3d0_TEST PASS\n");
    return 0;
}