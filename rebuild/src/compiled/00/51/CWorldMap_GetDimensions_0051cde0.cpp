// Leaf: ?GetDimensions@CWorldMap@@UBEABVC2DBoxI@@XZ
class C2DBoxI { public: int x0, y0, x1, y1; };

class CWorldMap { public:
    char pad[0x58];
    C2DBoxI dimensions;
    virtual const class C2DBoxI& GetDimensions() const;
};

const class C2DBoxI& CWorldMap::GetDimensions() const
{
    CWorldMap* self = const_cast<CWorldMap*>(this);
    return self->dimensions;
}
// Flatten route (flatten_pilot v4, NF324): CWorldMap_GetDimensions -> CWorldMap::GetDimensions, the RTTI-placed PDB member; class model: the row's local class model.
