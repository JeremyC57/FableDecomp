// CEditWorldMap::GetEditMap(int index)
// Leaf: ?CEditWorldMap_GetEditMap@@YIPAXPAVCEditWorldMap@@PAXH@Z
// this=ecx (__fastcall). Element stride = 12 bytes (3 dwords); returns dword at +4.
struct EditMapEntry {
    void* a;      // +0x0
    void* map;    // +0x4  <- returned
    void* c;      // +0x8
};

class CEditWorldMap { public:
    char pad[0x3c];
    EditMapEntry* maps;   // +0x3c
};

void* __fastcall CEditWorldMap_GetEditMap(CEditWorldMap* self, void* /*edx*/, int index)
{
    return self->maps[index].map;
}