#include <cstdio>
#include <vector>
struct V { float X, Y, Z; V(float x=0,float y=0,float z=0):X(x),Y(y),Z(z){} };
typedef std::vector<V> Arr;
static Arr Interp(const Arr& a, const Arr& b, float t, const V& d)
{   // GFInterpolateArray model: pad the shorter with default, lerp elementwise
    size_t n = a.size() > b.size() ? a.size() : b.size(); Arr r;
    for (size_t i = 0; i < n; ++i) { V x = i < a.size() ? a[i] : d, y = i < b.size() ? b[i] : d;
        r.push_back(V((1-t)*x.X+t*y.X,(1-t)*x.Y+t*y.Y,(1-t)*x.Z+t*y.Z)); }
    return r;
}
struct Spline { Arr P; float Tension; bool Conn; };
struct Data { Spline* Init; Spline This, Prev; bool Only, Reset; Arr Lim, Spd;
    void InterpolateTo(const Data& o, float t) {
        float s = 1.0f - t;
        This.Tension = s*This.Tension + t*o.This.Tension;
        This.P = Interp(Arr(This.P), Arr(o.This.P), t, V());
        Prev.Tension = s*Prev.Tension + t*o.Prev.Tension;
        Prev.P = Interp(Arr(Prev.P), Arr(o.Prev.P), t, V());
        long prevCount = (long)Prev.P.size();
        if ((long)This.P.size() != prevCount) Prev = This;
        Only = t > 0.5f ? o.Only : Only;
        Reset = t > 0.5f ? o.Reset : Reset;
        Lim = Interp(Lim, o.Lim, t, V()); Spd = Interp(Spd, o.Spd, t, V());
    } };
int main()
{
    Data a, b; a.Init = b.Init = 0;
    a.This.Tension = 0; b.This.Tension = 1; a.Prev.Tension = 2; b.Prev.Tension = 4;
    a.This.P.push_back(V(0,0,0)); a.This.P.push_back(V(2,2,2)); b.This.P.push_back(V(4,4,4)); b.This.P.push_back(V(6,6,6));
    a.Prev.P.push_back(V(1,1,1)); b.Prev.P.push_back(V(3,3,3)); a.This.Conn = true;
    a.Only = false; b.Only = true; a.Reset = true; b.Reset = false;
    a.Lim.push_back(V(10,0,0)); b.Lim.push_back(V(20,0,0)); a.Spd.push_back(V(1,1,1));
    a.InterpolateTo(b, 0.25f);
    bool ok = a.This.Tension == 0.25f && a.This.P.size() == 2 && a.This.P[1].X == 3.0f
        && a.Prev.P.size() == 2 && a.Prev.Tension == 0.25f && a.Prev.Conn       // counts differed -> Prev = This
        && a.Only == false && a.Reset == true && a.Lim[0].X == 12.5f && a.Spd[0].X == 0.75f;
    a.InterpolateTo(b, 0.75f);
    ok = ok && a.Only == true && a.Reset == false && a.Prev.P.size() == 2;
    if (!ok) { std::printf("00b211b0_TEST FAIL\n"); return 1; }
    std::printf("00b211b0_TEST PASS\n");
    return 0;
}