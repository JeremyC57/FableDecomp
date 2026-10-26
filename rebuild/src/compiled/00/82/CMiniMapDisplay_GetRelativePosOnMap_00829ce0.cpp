class C3DVector { public: float x, y, z; };
class C2DVector { public: float x, y; };
struct IPair { int a, b; };

// callee at 0x66c6a0 is a real __fastcall member on the (this+0x54) subobject.
struct MapSub {
    void GetBounds(IPair* p1, IPair* p2);
};

class CMiniMapDisplay { public:
    char pad[0x54];
    MapSub sub;
};

void __fastcall GetRelativePosOnMap(CMiniMapDisplay* display, void* edx,
                                    C3DVector* a, C2DVector* out)
{
    IPair lo;
    IPair hi;
    display->sub.GetBounds(&lo, &hi);
    out->x = (a->x - (float)lo.a) / (float)hi.a;
    out->y = (a->y - (float)lo.b) / (float)hi.b;
}