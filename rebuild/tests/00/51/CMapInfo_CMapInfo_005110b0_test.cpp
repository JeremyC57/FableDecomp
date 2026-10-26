// Behaviour test for 0x005110B0 CMapInfo copy constructor.
#include <cstdio>
#include <cstring>
static int g_deleted = 0;
static void DeleteMap(void*) { ++g_deleted; }
class CCharString { public:
    const char* text;
    CCharString() : text("") { }
    CCharString(const CCharString& o) : text(o.text) { }
    CCharString& operator=(const char* t) { text = t; return *this; }
};
class C2DBoxI { public: int TLX, TLY, BRX, BRY; };
class CCPPointerInfo { public: unsigned int RefCount; void (*DeleteFunc)(void*); void* Data; };
class CMap { public: int dummy; };
struct CCountedPointer {
    CMap* Data; CCPPointerInfo* Info;
    CCountedPointer() : Data(0), Info(0) { }
    CCountedPointer(const CCountedPointer& o) : Data(o.Data), Info(o.Info) { if (Info) ++Info->RefCount; }
    void Reset() { if (Info) { --Info->RefCount; if (Info->RefCount == 0) { Info->DeleteFunc(Info->Data); delete Info; } } Info = 0; Data = 0; }
};
struct Stream {
    int log[16]; int n;
    Stream() : n(0) { }
    void WritePresizedString(const CCharString& s) { log[n++] = (int)std::strlen(s.text); }
    void WriteBool(bool v) { log[n++] = v ? 1 : 0; }
    void WriteLong(int v) { log[n++] = v; }
    void WriteUInt64(unsigned int v) { log[n++] = (int)v; }
};
class CMapInfo { public:
    C2DBoxI Area; CCountedPointer PMap; CCharString LevelName, LevelScriptName; unsigned int MapUID;
    bool Used, Placed, Activated, Loading, LoadedOnPlayerProximity; CCharString SerialiseScript; unsigned int LevStamp[2], TngStamp[2]; bool IsSea;
    CMapInfo() { }
    CMapInfo(const CMapInfo& o)
        : Area(o.Area), PMap(o.PMap), LevelName(o.LevelName), LevelScriptName(o.LevelScriptName), MapUID(o.MapUID), Used(o.Used),
          Placed(o.Placed), Activated(o.Activated), Loading(o.Loading), LoadedOnPlayerProximity(o.LoadedOnPlayerProximity),
          SerialiseScript(o.SerialiseScript), IsSea(o.IsSea)
    { LevStamp[0] = o.LevStamp[0]; LevStamp[1] = o.LevStamp[1]; TngStamp[0] = o.TngStamp[0]; TngStamp[1] = o.TngStamp[1]; }
    void Clear()
    {
        PMap.Reset(); Used = false; Placed = false; Activated = false; Loading = false;
        LevelName = ""; LevelScriptName = ""; MapUID = 0; IsSea = false; LoadedOnPlayerProximity = true;
    }
    void SaveBinary(Stream* s)
    {
        s->WritePresizedString(LevelName); s->WritePresizedString(LevelScriptName);
        s->WriteBool(Used); s->WriteBool(LoadedOnPlayerProximity); s->WriteBool(IsSea);
        s->WriteLong(Area.TLX); s->WriteLong(Area.BRX); s->WriteLong(Area.TLY); s->WriteLong(Area.BRY);
        s->WriteBool(Placed); s->WriteUInt64(MapUID);
    }
};
static void Fill(CMapInfo& m)
{
    m.Area.TLX = 1; m.Area.TLY = 2; m.Area.BRX = 3; m.Area.BRY = 4;
    m.PMap.Data = 0; m.PMap.Info = new CCPPointerInfo; m.PMap.Info->RefCount = 1; m.PMap.Info->DeleteFunc = DeleteMap; m.PMap.Info->Data = 0;
    m.LevelName = "abc"; m.LevelScriptName = "scr"; m.MapUID = 77; m.Used = true; m.Placed = true; m.Activated = true; m.Loading = false;
    m.LoadedOnPlayerProximity = false; m.SerialiseScript = "s"; m.LevStamp[0] = 5; m.LevStamp[1] = 6; m.TngStamp[0] = 7; m.TngStamp[1] = 8; m.IsSea = true;
}
int main()
{
    CMapInfo a; Fill(a);
    CMapInfo b(a);
    bool ok = b.Area.BRY == 4 && b.PMap.Info == a.PMap.Info && a.PMap.Info->RefCount == 2 && std::strcmp(b.LevelName.text, "abc") == 0 &&
              b.MapUID == 77 && b.Placed && !b.LoadedOnPlayerProximity && b.TngStamp[1] == 8 && b.IsSea;
    b.PMap.Info = 0;
    CMapInfo c(b);
    ok = ok && c.PMap.Info == 0;
    if (!ok) { std::printf("005110b0_TEST FAIL\n"); return 1; }
    std::printf("005110b0_TEST PASS\n");
    return 0;
}