// Leaf: ?DrawGetHeightMapExtents@CMap@@UBE?AVC2DExtentsI@@XZ
class C2DExtentsI { public: int a; int b; };

class CMap { public:
    char pad[0x1d10];
    int f1d14; // +0x1d14
    int f1d18; // +0x1d18
    virtual C2DExtentsI DrawGetHeightMapExtents() const;
};

C2DExtentsI CMap::DrawGetHeightMapExtents() const
{
    C2DExtentsI r;
    r.b = this->f1d18;
    r.a = this->f1d14;
    return r;
}