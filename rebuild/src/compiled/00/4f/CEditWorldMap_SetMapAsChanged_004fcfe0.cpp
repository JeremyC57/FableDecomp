#include "engine/CEditWorldMap.h"
#include "engine/CWorldMap.h"   // base class (Fable.exe RTTI): Maps at +0x20 in the retail STLport 4.6 layout

struct CEditMapRuntimeEntry {
    unsigned char data[0x48];
};

struct CEditWorldMap_Methods : CEditWorldMap {
    void SetMapAsChanged(int index, char changed);
};

void CEditWorldMap_Methods::SetMapAsChanged(int index, char changed)
{
    CEditMapRuntimeEntry* entries = *(CEditMapRuntimeEntry**)&((CWorldMap*)this)->Maps[0];
    *((char*)&entries[index] + 0x27) = changed;
}
