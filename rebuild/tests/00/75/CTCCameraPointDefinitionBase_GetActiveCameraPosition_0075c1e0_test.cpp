// Test for 0x0075C1E0 CTCCameraPointDefinitionBase::GetActiveCameraPosition: links the source object; the result is a
// copy (not a reference) of the parent thing's position, and the call goes through the class's virtual slot.
#include <stdio.h>
class C3DVector {
public:
    long X; long Y; long Z;
    C3DVector() { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
};
static C3DVector g_pos;
class CThing { public: const C3DVector& GetPosition() const; };
const C3DVector& CThing::GetPosition() const { return g_pos; }
class CTCBase {
public:
    virtual void v00();
    CThing* ParentThing;
    bool    ToBeRemoved;
    bool    IsInGlobalUpdateTCs;
};
void CTCBase::v00() { }
class CTCCameraPointDefinitionBase : public CTCBase {
    friend int main();
protected:
    virtual C3DVector GetActiveCameraPosition() const;
};
int main()
{
    CThing t;
    g_pos.X = 10; g_pos.Y = -20; g_pos.Z = 30;
    CTCCameraPointDefinitionBase cp; cp.ParentThing = &t;
    const CTCCameraPointDefinitionBase& c = cp;
    C3DVector r = c.GetActiveCameraPosition();
    g_pos.X = 0;
    bool ok = r.X == 10 && r.Y == -20 && r.Z == 30;
    if (!ok) { puts("0075c1e0_TEST FAIL"); return 1; }
    puts("0075c1e0_TEST PASS");
    return 0;
}
