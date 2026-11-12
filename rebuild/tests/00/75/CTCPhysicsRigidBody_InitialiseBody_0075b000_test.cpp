// Behaviour test for CTCPhysicsRigidBody::InitialiseBody 0x0075B000: the mass / inertia / reset arithmetic through a copy of the model.
#include <cstdio>
#include <cmath>
struct V { float X, Y, Z; float Mag() const { return (float)sqrt(X*X+Y*Y+Z*Z); } void Clear() { X=Y=Z=0.0f; } };
struct M { float E11,E12,E13,E21,E22,E23,E31,E32,E33,E41,E42,E43;
    void SetIdentity() { E12=E13=E21=E23=E31=E32=E41=E42=E43=0.0f; E11=E22=E33=1.0f; } };
struct Config { V CMPosition; V CMVelocity, AngularMomentum, AngularVelocity, CMForce, Torque;
    void Reset() { CMVelocity.Clear(); AngularMomentum.Clear(); AngularVelocity.Clear(); CMForce.Clear(); Torque.Clear(); } };
struct Body { float OneOverMass, CoefficientOfRestitution; M Inertia; V CMPosition; Config Current, Last; };
static void Initialise(Body& b, V xe, V ye, V ze, V centre, V position)
{
    float x = xe.Mag(), y = ye.Mag(), z = ze.Mag();
    float mass = y * z * x * 3.2f;
    b.OneOverMass = 1.0f / mass;
    b.Inertia.SetIdentity();
    b.CoefficientOfRestitution = 1.0f;
    b.Inertia.E11 = 3.0f / ((y * y + z * z) * mass);
    b.Inertia.E22 = 3.0f / ((x * x + z * z) * mass);
    b.Inertia.E33 = 3.0f / ((x * x + y * y) * mass);
    b.CMPosition = centre;
    b.Current.Reset();
    b.Current.CMPosition.X = centre.X + position.X; b.Current.CMPosition.Y = centre.Y + position.Y; b.Current.CMPosition.Z = centre.Z + position.Z;
    b.Last = b.Current;
}
static bool near(float a, float e) { return fabs(a - e) < 1e-4f * (fabs(e) + 1.0f); }
int main()
{
    Body b; b.Current.CMVelocity.X = 5.0f; b.Current.Torque.Z = 7.0f; b.Inertia.E12 = 9.0f;
    V xe = {3.0f, 4.0f, 0.0f}, ye = {0.0f, 2.0f, 0.0f}, ze = {0.0f, 0.0f, 1.0f}, c = {1.0f, 2.0f, 3.0f}, p = {10.0f, 20.0f, 30.0f};
    Initialise(b, xe, ye, ze, c, p);
    float mass = 5.0f * 2.0f * 1.0f * 3.2f;  // 32
    if (!near(b.OneOverMass, 1.0f / mass) || b.CoefficientOfRestitution != 1.0f || b.Inertia.E12 != 0.0f || b.Inertia.E43 != 0.0f
        || !near(b.Inertia.E11, 3.0f / (5.0f * mass)) || !near(b.Inertia.E22, 3.0f / (26.0f * mass)) || !near(b.Inertia.E33, 3.0f / (29.0f * mass))
        || b.Current.CMVelocity.X != 0.0f || b.Current.Torque.Z != 0.0f || b.Current.CMPosition.Y != 22.0f || b.Last.CMPosition.Z != 33.0f
        || b.CMPosition.X != 1.0f) { std::printf("0075b000_TEST FAIL\n"); return 1; }
    std::printf("0075b000_TEST PASS\n");
    return 0;
}