// Behaviour test for CVillageTaskPickCrops::IsBetterThan (0x00860D00): nearer crop wins; no rival or an unavailable
// task falls back to IsTaskAvailable().
#include <cstdio>
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector operator-(const C3DVector& o) const { C3DVector r; r.X = X - o.X; r.Y = Y - o.Y; r.Z = Z - o.Z; return r; }
    float GetLengthSquared() const { return X * X + Y * Y + Z * Z; }
};
struct CThing { C3DVector Pos; const C3DVector& GetPosition() const { return Pos; } };
struct CIntelligentPointer { CThing* Item; CThing* GetPItem() const { return Item; } };
struct CVillageTaskPickCrops {
    bool Available; CIntelligentPointer Crop;
    bool IsTaskAvailable() const { return Available; }
    bool IsBetterThan(const CVillageTaskPickCrops* other, const CThing& thing) const
    {
        if (other && IsTaskAvailable()) {
            C3DVector crop = Crop.GetPItem()->GetPosition();
            C3DVector rivalCrop = other->Crop.GetPItem()->GetPosition();
            C3DVector position = thing.GetPosition();
            C3DVector myDelta = crop - position;
            C3DVector rivalDelta = rivalCrop - position;
            return myDelta.GetLengthSquared() < rivalDelta.GetLengthSquared();
        }
        return IsTaskAvailable();
    }
};
int main()
{
    CThing a = { C3DVector(1, 0, 0) }, b = { C3DVector(0, 5, 0) }, me = { C3DVector(0, 0, 0) };
    CVillageTaskPickCrops t1 = { true, { &a } }, t2 = { true, { &b } };
    bool ok = t1.IsBetterThan(&t2, me) && !t2.IsBetterThan(&t1, me) && t1.IsBetterThan(0, me) && !t1.IsBetterThan(&t1, me);
    t1.Available = false;
    ok = ok && !t1.IsBetterThan(&t2, me) && !t1.IsBetterThan(0, me);
    if (!ok) { std::printf("00860d00_TEST FAIL\n"); return 1; }
    std::printf("00860d00_TEST PASS\n");
    return 0;
}