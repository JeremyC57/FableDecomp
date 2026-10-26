// CTCInventoryMap::GetRelativePosOnMap(NUISystem::CComponent* component, const C3DVector& position, C2DVector& relative) const -- scale a world position to the map screen: the bounding box of the world-map records 1..n-1 gives the world extent, and the UI misc-things def's map width/height over that extent scale X and Y
// Leaf: GetRelativePosOnMap
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005FB630, 429 bytes (+int3), `ret 0Ch`, esp frame (speed TU).
//   Ego_r.pdb: ?GetRelativePosOnMap@CTCInventoryMap@@IBEXPAVCComponent@NUISystem@@ABVC3DVector@@AAVC2DVector@@@Z.
// Callees: CCharString(const char*, long) @ 0x0099EBF0 / ~CCharString @ 0x0099EAE0, CGameDefinitionManager::Get @ 0x0044C6B0
//   (static `__fastcall`), GetGlobalIndexFromClassIndex(const CCharString&, long) @ 0x009AD390, GetDef<CUIMiscThingsDef>
//   @ 0x0042B312, CTCBase::GetWorld @ 0x00686A80 and CWorld slot 13 (+0x34, GetWorldMap). String "UI_MISC_THINGS_DEF"
//   @ 0x01230018.
// Shape: the def index is a named local (the class-name temporary dies before GetDef); GetDef has a same-TU noinline
//   stand-in so the def pointer's release reads it once (`dec [edx+4] ; jne`); the world map is re-fetched at every use and
//   size() is a signed /72 (`imul 38E38E39h`); the element is read straight off `_M_start[i]` (an inline operator[] or begin()
//   accessor leaves a dead `add eax,20h`); the bounds are an uninitialised C2DBoxI whose right/bottom live in ebp/ebx.
//   `component` is unused.
// Layout: CWorldMap map records (72 bytes, box first) at +0x20; CUIMiscThingsDef map width/height at +0xF8/+0xFC (retail).
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
struct CUIMiscThingsDef {
    virtual void v00();
    virtual void Destroy();                             // slot 1
    long RefCount;                                      // +0x04
    unsigned char _pad08[0xf8 - 0x08];
    float MapWidth;                                     // +0xF8
    float MapHeight;                                    // +0xFC
};
struct CUIMiscThingsDefPointer {
    CUIMiscThingsDef* Object;
    CUIMiscThingsDefPointer() : Object(0) {}
    ~CUIMiscThingsDefPointer()
    {
        if (Object && --Object->RefCount == 0)
            Object->Destroy();
    }
    CUIMiscThingsDef* operator->() const { return Object; }
};
class CDefinitionManager {
public:
    long GetGlobalIndexFromClassIndex(const CCharString& className, long classIndex) const;   // 0x009AD390
    bool GetDef(long index, CUIMiscThingsDefPointer* out) const;                            // 0x0042B312
};
class CGameDefinitionManager : public CDefinitionManager {
public:
    static CGameDefinitionManager& __fastcall Get();    // 0x0044C6B0
};
void* FindDef(long index);
#pragma auto_inline(off)
__declspec(noinline) bool CDefinitionManager::GetDef(long index, CUIMiscThingsDefPointer* out) const
{
    void* def = FindDef(index);
    if (!def)
        return false;
    out->Object = (CUIMiscThingsDef*)def;
    return true;
}
#pragma auto_inline(on)
class C2DCoordI { public: long X, Y; };
class C2DBoxI { public: C2DCoordI TopLeft; C2DCoordI BottomRight; };
struct CMapRecord {                                     // 72 bytes
    C2DBoxI Box;                                        // +0x00
    unsigned char _pad10[0x48 - 0x10];
};
struct CMapRecordVector {
    CMapRecord* _M_start;
    CMapRecord* _M_finish;
    CMapRecord* _M_end_of_storage;
    long size() const { return _M_finish - _M_start; }
    CMapRecord* begin() const { return _M_start; }
    const CMapRecord& operator[](long n) const { return *(begin() + n); }
};
class CWorldMap { public:
    unsigned char _pad00[0x20];
    CMapRecordVector Maps;                              // +0x20
};
class CWorld { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12();
    virtual const CWorldMap* GetWorldMap() const;       // slot 13 (+0x34)
};
class C3DVector { public: float X, Y, Z; };
class C2DVector { public: float X, Y; };
namespace NUISystem { class CComponent; }
class CTCBase
{
protected:
    const CWorld& PeekWorld() const;   // 0x00686A80
};
namespace NInventory {
class CTCInventoryBase : public CTCBase
{
public:
};
}

class CTCInventoryMap : public NInventory::CTCInventoryBase { public:
    void GetRelativePosOnMap(NUISystem::CComponent* component, const C3DVector& position, C2DVector& relative) const;
};

void CTCInventoryMap::GetRelativePosOnMap(NUISystem::CComponent* component, const C3DVector& position, C2DVector& relative) const
{
    CUIMiscThingsDefPointer def;
    long index = CGameDefinitionManager::Get().GetGlobalIndexFromClassIndex("UI_MISC_THINGS_DEF", 1);
    CGameDefinitionManager::Get().GetDef(index, &def);
    if (PeekWorld().GetWorldMap()->Maps.size() > 0) {
        C2DBoxI bounds;
        for (long i = 1; i < PeekWorld().GetWorldMap()->Maps.size(); ++i) {
            const C2DBoxI& box = PeekWorld().GetWorldMap()->Maps._M_start[i].Box;
            if (i == 1) {
                bounds = box;
            } else {
                if (bounds.TopLeft.X > box.TopLeft.X)
                    bounds.TopLeft.X = box.TopLeft.X;
                if (bounds.TopLeft.Y > box.TopLeft.Y)
                    bounds.TopLeft.Y = box.TopLeft.Y;
                if (bounds.BottomRight.X < box.BottomRight.X)
                    bounds.BottomRight.X = box.BottomRight.X;
                if (bounds.BottomRight.Y < box.BottomRight.Y)
                    bounds.BottomRight.Y = box.BottomRight.Y;
            }
        }
        long width = bounds.BottomRight.X - bounds.TopLeft.X;
        long height = bounds.BottomRight.Y - bounds.TopLeft.Y;
        relative.X = def->MapWidth / width * position.X;
        relative.Y = def->MapHeight / height * position.Y;
    }
}