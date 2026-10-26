struct MapEntry6 {
    unsigned short a;
    unsigned short b;
    unsigned short c;
    unsigned short d;
    unsigned short e;
    unsigned short f;
};

class CWorldMap { public:
    char pad[0x48];
    MapEntry6* begin; // +0x48
    MapEntry6* end;   // +0x4c
    long DrawGetNoMaps();
};

long CWorldMap::DrawGetNoMaps()
{
    return (long)(this->end - this->begin);
}