#include <cstdio>
// behaviour test of CThingFilter_OverlappingArea::operator() (0x0050EF30): a thing with physics passes when its centre lies inside the area grown by the rounded-up bounding radius
#ifdef _MSC_VER
#pragma optimize("y", off)
__forceinline long CeilToLong(float value)
{
    long result; float one; float half; float back;
    half = 0.5f; one = 1.0f;
    __asm {
        fld value
        fadd half
        fistp result
        fild result
        fsub one
        fstp back
        mov eax, value
        cmp eax, back
        jne done
        dec result
    done:
    }
    return result;
}
#else
#include <math.h>
inline long CeilToLong(float value) { return (long)ceil((double)value); }
#endif
class C3DVector { public: float X, Y, Z; };
class C2DBoxI { public:
    long X0, Y0, X1, Y1;
    C2DBoxI() { }
    C2DBoxI(const C2DBoxI& o) : X0(o.X0), Y0(o.Y0), X1(o.X1), Y1(o.Y1) { }
    void Grow(float amount)
    {
        X1 = (long)(X1 + amount); X0 = (long)(X0 - amount); Y0 = (long)(Y0 - amount); Y1 = (long)(Y1 + amount);
    }
};
class CTCPhysicsBase {
public:
    C3DVector Centre; float Radius;
    void GetBoundingInfo(C3DVector* centre, float* radius) { *centre = Centre; *radius = Radius; }
};
class CThing { public: CTCPhysicsBase* PhysicsTC; };
class CThingFilter_OverlappingArea {
public:
    const C2DBoxI* Area;
    bool operator()(CThing* thing) const
    {
        if (thing->PhysicsTC) {
            float radius; C3DVector centre;
            thing->PhysicsTC->GetBoundingInfo(&centre, &radius);
            radius = (float)CeilToLong(radius);
            C2DBoxI box = *Area;
            box.Grow(radius);
            long x = (long)centre.X; long y = (long)centre.Y;
            return x >= box.X0 && x < box.X1 && y >= box.Y0 && y < box.Y1;
        }
        return false;
    }
};
int main()
{
    C2DBoxI area; area.X0 = 10; area.Y0 = 10; area.X1 = 20; area.Y1 = 20;
    CThingFilter_OverlappingArea filter; filter.Area = &area;
    CTCPhysicsBase phys; phys.Radius = 1.2f;              // grows the box by 2 on every side: [8, 22)
    CThing thing; thing.PhysicsTC = &phys;
    CThing ghost; ghost.PhysicsTC = 0;
    bool ok = true;
    phys.Centre.X = 15.0f; phys.Centre.Y = 15.0f; phys.Centre.Z = 0.0f; ok = ok && filter(&thing);
    phys.Centre.X = 8.0f;  phys.Centre.Y = 21.9f;  ok = ok && filter(&thing);      // inside the grown box
    phys.Centre.X = 7.9f;  phys.Centre.Y = 15.0f;  ok = ok && !filter(&thing);     // (long)7.9 = 7 < 8: outside
    ok = ok && area.X0 == 10 && area.X1 == 20;                                     // the area itself is untouched
    phys.Centre.X = 22.0f; phys.Centre.Y = 15.0f;  ok = ok && !filter(&thing);     // X1 exclusive
    phys.Centre.X = 15.0f; phys.Centre.Y = 7.0f;   ok = ok && !filter(&thing);
    ok = ok && !filter(&ghost);                                                    // no physics: never overlaps
    std::printf(ok ? "0050ef30_TEST PASS\n" : "0050ef30_TEST FAIL\n");
    return ok ? 0 : 1;
}