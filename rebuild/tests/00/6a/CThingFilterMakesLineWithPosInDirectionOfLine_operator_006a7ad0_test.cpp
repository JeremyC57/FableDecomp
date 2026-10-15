// Behaviour test for 0x006A7AD0 CThingFilter_MakesLineWithPosInDirectionOfLine::operator()
#include <cstdio>
struct C3DVector { float x, y, z; };
static const float g_fZero = 0.0f;
struct CThing {
    C3DVector m_pos;
    const C3DVector& GetPosition() const { return m_pos; }
};
struct CThingFilter_MakesLineWithPosInDirectionOfLine {
    C3DVector Pos;
    C3DVector Line;
    bool operator()(CThing* thing) const
    {
        const C3DVector& pos = thing->GetPosition();
        C3DVector delta;
        delta.x = pos.x - Pos.x;
        delta.y = pos.y - Pos.y;
        delta.z = pos.z - Pos.z;
        return delta.x * Line.x + delta.y * Line.y + delta.z * Line.z > g_fZero;
    }
};
int main()
{
    CThingFilter_MakesLineWithPosInDirectionOfLine f;
    f.Pos.x = 1.0f; f.Pos.y = 1.0f; f.Pos.z = 1.0f;
    f.Line.x = 0.0f; f.Line.y = 1.0f; f.Line.z = 0.0f;
    CThing t; t.m_pos.x = 9.0f; t.m_pos.y = 3.0f; t.m_pos.z = -4.0f;
    bool ok = f(&t);                                // delta.y = 2 along +y
    t.m_pos.y = 1.0f; if (f(&t)) ok = false;        // perpendicular: dot 0 is not > 0
    t.m_pos.y = 0.0f; if (f(&t)) ok = false;        // behind
    if (!ok) { std::printf("006a7ad0_TEST FAIL\n"); return 1; }
    std::printf("006a7ad0_TEST PASS\n");
    return 0;
}