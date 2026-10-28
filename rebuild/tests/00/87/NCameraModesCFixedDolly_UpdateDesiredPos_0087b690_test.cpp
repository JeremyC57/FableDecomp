extern "C" int printf(const char*, ...);
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL line %d\n", __LINE__); } } while (0)
// Behaviour test for 0x0087B690 NCameraModes::CFixedDolly::UpdateDesiredPos: desiredPos = observed thing's position + the
// dolly offset, component-wise.
class C3DVector { public: float X, Y, Z; };
static C3DVector operator+(const C3DVector& a, const C3DVector& b) { C3DVector r = { a.X + b.X, a.Y + b.Y, a.Z + b.Z }; return r; }
class CThing { public: C3DVector pos; const C3DVector& GetPosition() const { return pos; } };
struct CFixedDolly {
    const CThing* observed; C3DVector DollyVector;
    void UpdateDesiredPos(C3DVector& d) { d = observed->GetPosition() + DollyVector; }
};
int main()
{
    CThing t = { { 10.0f, -4.0f, 2.5f } };
    CFixedDolly d; d.observed = &t; d.DollyVector.X = 1.0f; d.DollyVector.Y = 2.0f; d.DollyVector.Z = -0.5f;
    C3DVector out = { 0, 0, 0 };
    d.UpdateDesiredPos(out);
    CHECK(out.X == 11.0f && out.Y == -2.0f && out.Z == 2.0f);
    if (fails == 0) printf("0087b690_TEST PASS\n"); else printf("0087b690_TEST FAIL\n");
    return fails;
}