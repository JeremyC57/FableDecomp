// Leaf: ?CWorldMap_GetDimensions@@YIABVC2DBoxI@@PAVCWorldMap@@@Z
class C2DBoxI { public: int x0, y0, x1, y1; };

class CWorldMap { public:
    char pad[0x5c];
    C2DBoxI dimensions;
};

C2DBoxI const & __fastcall CWorldMap_GetDimensions(CWorldMap *self)
{
    return self->dimensions;
}