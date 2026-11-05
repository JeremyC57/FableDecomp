// Leaf: ?CWorldMap_RegisterScriptedMapBrush@@YIXPAVCWorldMap@@HPAVCScriptedMapBrush@@J@Z
class CScriptedMapBrush;

// 0x82e250: thiscall-style, ecx = key, returns transformed key in eax
struct KeyHolder {
    int transform();
};

// 0x519230: thiscall, ecx = self, one stack arg, returns void** (slot)
struct MapSub {
    void** index(int k);
};

class CWorldMap { public:
    char pad[0x14];
    MapSub sub;
};

void __fastcall CWorldMap_RegisterScriptedMapBrush(CWorldMap* self, int edx, CScriptedMapBrush* brush, long key)
{
    int k = ((KeyHolder*)key)->transform();
    void** slot = self->sub.index(k);
    *slot = (void*)brush;
}