// Behaviour test for 0x00AC53F0 C3DMeshFileEngineMaterialChunk(const C3DMeshFileMaterialChunk&): a portable model of the
// material -> engine-material conversion (map-type switch, ext properties, bank reset, CalculateMapFlags).
#include <cstdio>
#include <string>
#include <vector>
struct Map { int type; std::string filename; };
struct Material { long index; std::string name; bool twoSided; bool hasExt; unsigned long selfIllum; std::vector<Map> maps; };
struct Filenames { std::string diffuse, bump, reflection, illumination; };
struct Engine {
    long MaterialIndex; std::string Name; Filenames* MapFilenames; unsigned long Banks[4]; unsigned long MapFlags;
    unsigned long SelfIllumination; bool TwoSided, Transparent, BooleanAlpha, DegenerateTriangles;
    void CalculateMapFlags() {
        MapFlags = 0;
        if (MapFilenames) {
            if (!MapFilenames->diffuse.empty()) MapFlags |= 1;
            if (!MapFilenames->bump.empty()) MapFlags |= 2;
            if (!MapFilenames->reflection.empty()) MapFlags |= 4;
            if (!MapFilenames->illumination.empty()) MapFlags |= 8;
        } else {
            for (int i = 0; i < 4; ++i) if (Banks[i]) MapFlags |= 1u << i;
        }
    }
    Engine(const Material& m) : MapFilenames(0), DegenerateTriangles(false) {
        MaterialIndex = m.index; Name = m.name; TwoSided = m.twoSided; SelfIllumination = 0;
        if (m.hasExt) SelfIllumination = m.selfIllum;
        Transparent = false; BooleanAlpha = false;
        MapFilenames = new Filenames;
        for (size_t i = 0; i < m.maps.size(); ++i) {
            const Map& map = m.maps[i];
            switch (map.type) {
            case 6: Transparent = true; break;
            case -1: case 1: MapFilenames->diffuse = map.filename; break;
            case 8: MapFilenames->bump = map.filename; break;
            case 9: MapFilenames->reflection = map.filename; break;
            case 5: MapFilenames->illumination = map.filename; break;
            }
        }
        for (int i = 0; i < 4; ++i) Banks[i] = 0;
        CalculateMapFlags();
    }
    ~Engine() { delete MapFilenames; }
};
int main()
{
    Material m; m.index = 7; m.name = "rock"; m.twoSided = true; m.hasExt = true; m.selfIllum = 42;
    Map a = { 1, "d.tga" }, b = { 9, "r.tga" }, c = { 6, "" }, d = { 2, "x.tga" }, e = { 7, "y.tga" };
    m.maps.push_back(a); m.maps.push_back(b); m.maps.push_back(c); m.maps.push_back(d); m.maps.push_back(e);
    Engine x(m);
    if (x.MaterialIndex != 7 || x.Name != "rock" || !x.TwoSided || x.SelfIllumination != 42 || !x.Transparent || x.BooleanAlpha
        || x.DegenerateTriangles || x.MapFilenames->diffuse != "d.tga" || x.MapFilenames->reflection != "r.tga"
        || !x.MapFilenames->bump.empty() || x.MapFlags != 5) { std::printf("00ac53f0_TEST FAIL\n"); return 1; }
    Material n; n.index = -1; n.name = ""; n.twoSided = false; n.hasExt = false; n.selfIllum = 9;
    Map f = { -1, "dd" }, g = { 8, "bb" }, h = { 5, "ii" };
    n.maps.push_back(f); n.maps.push_back(g); n.maps.push_back(h);
    Engine y(n);
    if (y.SelfIllumination != 0 || y.Transparent || y.MapFlags != (1 | 2 | 8) || y.MapFilenames->illumination != "ii")
        { std::printf("00ac53f0_TEST FAIL\n"); return 1; }
    Material z; z.index = 0; z.twoSided = false; z.hasExt = false; z.selfIllum = 0;
    Engine w(z);
    if (w.MapFlags != 0 || w.Banks[0] != 0) { std::printf("00ac53f0_TEST FAIL\n"); return 1; }
    std::printf("00ac53f0_TEST PASS\n");
    return 0;
}