// Behaviour test for 0x00603000 CTCInventoryMap::CreateMiniMapMarkers: the four clip flags (prison courtyard 1 / 2 from the region's
// mini-map graphic, Headsman's Hill north from the execution-tree graphic, turned into south when the living hero stands at
// Y <= 4740), the cached-marker replay when the region is not the one the parent thing's map is in (marker per cached entry of
// that region number, graphic index only when non-zero, angle always; then return), the early returns (no maps / a map not
// loaded), the purge of the region's cached entries, then in retail order: the hero marker (angle 1 - facing with physics),
// creatures with information that are not body-guard followers (not cached), teleporters (unavailable when inactive; cached),
// houses owned / for sale, wives married / attracted, enemies of a living hero except guild sparrows, demon doors, the region-exit
// markers, available shop keepers, script markers (thing position or origin; clip tests on Z 51 / Y 4740; graphic index; not
// cached) and body-guard henchmen (cached). Every visible marker is clipped by IsThingAsPosBeingDrawn (the four flags).
// Model: the row's statements in source order over plain structs; every marker goes to the parent in the order retail adds it.
#include <cstdio>
#include <string>
#include <vector>
#include <list>
#include <map>
struct V3 { float X, Y, Z; V3() : X(0), Y(0), Z(0) { } V3(float x, float y, float z) : X(x), Y(y), Z(z) { } };
struct V2 { float X, Y; V2() : X(0), Y(0) { } V2(float x, float y) : X(x), Y(y) { } };
struct Child { std::string Def; V2 Position; long Graphic; float Angle; bool Unavailable; };   // Graphic -1: SetGraphicIndex never called
struct Cached { V2 Position; long Region; std::string Def; long Length; long Graphic; float Angle; };
struct Thing { std::string Def; V3 Pos; bool Dead; bool Info; long Map; bool Active, Owned, ForSale, Married, Attracted;
               bool Enemy, DemonDoor, Shop, Keeper, Available; };
struct Info { const Thing* PThing; long Graphic; };
struct Hero { bool Dead; float Y; bool HasPhysics; float Facing; };
struct Region { std::string Name; long Number; std::vector<long> Maps; long Graphic; };
struct Map { long Number; bool Loaded; };
struct Model {
    bool C1, C2, North, South;
    std::vector<Cached> MiniMapMarkers;
    std::vector<Child> Parent;
    std::vector<Map> Maps; std::vector<Thing> Things; std::vector<Info> NonAuto; std::vector<const Thing*> Followers;
    bool HasFollowed; long ParentRegion; long Courtyard1, Courtyard2, ExecutionTree; bool GraphicKnown; int RegionExits;
    bool Drawn(const V3& p) const {
        if (C1 && p.Z > 51.0f) return false;
        if (C2 && p.Z <= 51.0f) return false;
        if (North && p.Y <= 4740.0f) return false;
        if (South && p.Y > 4740.0f) return false;
        return true;
    }
    static V2 Rel(const V3& p) { return V2(p.X * 0.5f, p.Y * 0.25f); }
    bool Loaded(long m) const { for (size_t i = 0; i < Maps.size(); ++i) if (Maps[i].Number == m) return Maps[i].Loaded; return false; }
    Child& Add(const std::string& def, const V2& p) { Child c; c.Def = def; c.Position = p; c.Graphic = -1; c.Angle = 0.0f; c.Unavailable = false; Parent.push_back(c); return Parent.back(); }
    void Cache(const std::string& def, const V2& p, long region) { Cached c; c.Position = p; c.Region = region; c.Def = def; c.Length = (long)def.size(); c.Graphic = 0; c.Angle = 0.0f; MiniMapMarkers.push_back(c); }
    static bool BodyGuard(const Thing* t) { return t->Def == "CREATURE_BODY_GUARD" || t->Def == "CREATURE_BODY_GUARD_WEAK" || t->Def == "CREATURE_BODY_GUARD_GOOD" || t->Def == "CREATURE_BODY_GUARD_GOOD_WEAK"; }
    bool IsFollower(const Thing* t) const { for (size_t i = 0; i < Followers.size(); ++i) if (Followers[i] == t) return true; return false; }
    void Create(const Region& region, const Hero* hero) {
        C1 = C2 = North = South = false;
        if (GraphicKnown) {
            if (region.Graphic == Courtyard1) C1 = true;
            else if (region.Graphic == Courtyard2) { C1 = false; C2 = true; }
            else if (region.Graphic == ExecutionTree) North = true;
        }
        if (hero && !hero->Dead) {
            if (North && hero->Y <= 4740.0f) { North = false; South = true; }
            else if (South && hero->Y > 4740.0f) { North = true; South = false; }
        }
        if (region.Number != ParentRegion) {
            for (size_t i = 0; i < MiniMapMarkers.size(); ++i)
                if (MiniMapMarkers[i].Region == region.Number) {
                    Child& c = Add(MiniMapMarkers[i].Def, MiniMapMarkers[i].Position);
                    if (MiniMapMarkers[i].Graphic) c.Graphic = MiniMapMarkers[i].Graphic;
                    c.Angle = MiniMapMarkers[i].Angle;
                }
            return;
        }
        if (region.Maps.empty()) return;
        for (size_t i = 0; i < region.Maps.size(); ++i) if (!Loaded(region.Maps[i])) return;
        for (std::vector<Cached>::iterator it = MiniMapMarkers.begin(); it != MiniMapMarkers.end(); )
            if (it->Region == region.Number) it = MiniMapMarkers.erase(it); else ++it;
        if (hero && !hero->Dead) {
            Child& c = Add("UI_HERO_MARKER", Rel(V3(1.0f, hero->Y, 0.0f)));
            if (hero->HasPhysics) c.Angle = 1.0f - hero->Facing;
        }
        for (size_t m = 0; m < region.Maps.size(); ++m)
            for (size_t t = 0; t < Things.size(); ++t) {
                const Thing* c = &Things[t];
                if (!c->Info || c->Map != region.Maps[m]) continue;
                if ((!HasFollowed || !IsFollower(c)) || !BodyGuard(c))
                    if (Drawn(c->Pos)) Add("UI_INTERESTING_MARKER", Rel(c->Pos));
            }
        for (size_t m = 0; m < region.Maps.size(); ++m)
            for (size_t t = 0; t < Things.size(); ++t) {
                const Thing& c = Things[t];
                if (c.Def != "TELEPORTER" || c.Map != region.Maps[m] || !Drawn(c.Pos)) continue;
                Child& k = Add("UI_TELEPORT_MARKER", Rel(c.Pos));
                if (!c.Active) k.Unavailable = true;
                Cache("UI_TELEPORT_MARKER", Rel(c.Pos), region.Number);
            }
        for (size_t m = 0; m < region.Maps.size(); ++m)
            for (size_t t = 0; t < Things.size(); ++t) {
                const Thing& c = Things[t];
                if (c.Def != "HOUSE" || c.Map != region.Maps[m] || !Drawn(c.Pos)) continue;
                const char* def = c.Owned ? "UI_HOUSE_OWNED_MARKER" : c.ForSale ? "UI_HOUSE_FOR_SALE_MARKER" : 0;
                if (def) { Add(def, Rel(c.Pos)); Cache(def, Rel(c.Pos), region.Number); }
            }
        for (size_t m = 0; m < region.Maps.size(); ++m)
            for (size_t t = 0; t < Things.size(); ++t) {
                const Thing& c = Things[t];
                if (c.Def != "WIFE" || c.Map != region.Maps[m] || !Drawn(c.Pos)) continue;
                const char* def = c.Married ? "UI_WIFE_MARKER" : c.Attracted ? "UI_ATTRACTED_MARKER" : 0;
                if (def) { Add(def, Rel(c.Pos)); Cache(def, Rel(c.Pos), region.Number); }
            }
        if (hero && !hero->Dead)
            for (size_t m = 0; m < region.Maps.size(); ++m)
                for (size_t t = 0; t < Things.size(); ++t) {
                    const Thing& c = Things[t];
                    if (!c.Enemy || c.Map != region.Maps[m]) continue;
                    if (Drawn(c.Pos) && c.Def != "CREATURE_BIRD_GUILD_SPARROW") { Add("UI_ENEMY_MARKER", Rel(c.Pos)); Cache("UI_ENEMY_MARKER", Rel(c.Pos), region.Number); }
                }
        for (size_t t = 0; t < Things.size(); ++t)
            if (Things[t].DemonDoor && Drawn(Things[t].Pos)) { Add("UI_DEMON_DOOR_MARKER", Rel(Things[t].Pos)); Cache("UI_DEMON_DOOR_MARKER", Rel(Things[t].Pos), region.Number); }
        ++RegionExits;
        for (size_t m = 0; m < region.Maps.size(); ++m)
            for (size_t t = 0; t < Things.size(); ++t) {
                const Thing& c = Things[t];
                if (!c.Keeper || c.Map != region.Maps[m]) continue;
                if (Drawn(c.Pos) && !c.Dead && c.Shop && c.Available) { Add("UI_TRADER_MARKER", Rel(c.Pos)); Cache("UI_TRADER_MARKER", Rel(c.Pos), region.Number); }
            }
        for (size_t i = 0; i < NonAuto.size(); ++i) {
            V3 pos;
            if (NonAuto[i].PThing) pos = NonAuto[i].PThing->Pos;
            if (C1 && pos.Z > 51.0f) continue;
            if (C2 && pos.Z <= 51.0f) continue;
            if (North && pos.Y <= 4740.0f) continue;
            if (South && pos.Y > 4740.0f) continue;
            Add("UI_MINI_MAP_MARKER", Rel(pos)).Graphic = NonAuto[i].Graphic;
        }
        if (HasFollowed)
            for (size_t j = 0; j < Followers.size(); ++j)
                if (Drawn(Followers[j]->Pos) && BodyGuard(Followers[j])) { Add("UI_HENCHMAN_MARKER", Rel(Followers[j]->Pos)); Cache("UI_HENCHMAN_MARKER", Rel(Followers[j]->Pos), region.Number); }
    }
};
static Thing T(const char* def, float x, float y, float z, long map)
{ Thing t; t.Def = def; t.Pos = V3(x, y, z); t.Dead = false; t.Info = false; t.Map = map; t.Active = t.Owned = t.ForSale = t.Married = t.Attracted = false;
  t.Enemy = t.DemonDoor = t.Shop = t.Keeper = t.Available = false; return t; }
static bool Near(float a, float b) { return a - b < 0.001f && b - a < 0.001f; }
static bool Is(const Model& m, size_t i, const char* def, float x, float y)
{ return i < m.Parent.size() && m.Parent[i].Def == def && Near(m.Parent[i].Position.X, x * 0.5f) && Near(m.Parent[i].Position.Y, y * 0.25f); }
static void Base(Model& m)
{
    m.C1 = m.C2 = m.North = m.South = false; m.HasFollowed = true; m.ParentRegion = 4; m.Courtyard1 = 11; m.Courtyard2 = 12; m.ExecutionTree = 13;
    m.GraphicKnown = true; m.RegionExits = 0;
    Map a; a.Number = 1; a.Loaded = true; Map b; b.Number = 2; b.Loaded = true; m.Maps.push_back(a); m.Maps.push_back(b);
}
int main()
{
    bool ok = true;
    Region region; region.Name = "OAKVALE"; region.Number = 4; region.Maps.push_back(1); region.Maps.push_back(2); region.Graphic = 13;
    Hero hero; hero.Dead = false; hero.Y = 4000.0f; hero.HasPhysics = true; hero.Facing = 0.25f;
    {   // full pass: execution tree + hero at Y 4000 <= 4740 -> Headsman's Hill south, which hides every position at Y > 4740
        Model m; Base(m);
        Cached old; old.Region = 4; old.Def = "UI_OLD"; old.Graphic = 0; old.Angle = 0.0f; old.Length = 6; m.MiniMapMarkers.push_back(old);
        Cached other = old; other.Region = 9; other.Def = "UI_OTHER"; m.MiniMapMarkers.push_back(other);
        m.Things.push_back(T("CREATURE_VILLAGER", 2.0f, 100.0f, 0.0f, 1)); m.Things.back().Info = true;
        m.Things.push_back(T("CREATURE_BODY_GUARD", 4.0f, 200.0f, 0.0f, 2)); m.Things.back().Info = true;        // follower + body guard: skipped
        m.Things.push_back(T("CREATURE_BODY_GUARD_WEAK", 6.0f, 300.0f, 0.0f, 2)); m.Things.back().Info = true;   // body guard, not a follower
        m.Things.push_back(T("CREATURE_HIGH", 8.0f, 5000.0f, 0.0f, 1)); m.Things.back().Info = true;             // clipped (Y > 4740)
        m.Things.push_back(T("TELEPORTER", 10.0f, 400.0f, 0.0f, 1));                                               // inactive
        m.Things.push_back(T("TELEPORTER", 12.0f, 440.0f, 0.0f, 3)); m.Things.back().Active = true;              // map 3: not this region
        m.Things.push_back(T("HOUSE", 14.0f, 500.0f, 0.0f, 2)); m.Things.back().Owned = true; m.Things.back().ForSale = true;
        m.Things.push_back(T("HOUSE", 16.0f, 520.0f, 0.0f, 2)); m.Things.back().ForSale = true;
        m.Things.push_back(T("HOUSE", 18.0f, 540.0f, 0.0f, 1));                                                    // neither: nothing
        m.Things.push_back(T("WIFE", 20.0f, 600.0f, 0.0f, 1)); m.Things.back().Attracted = true;
        m.Things.push_back(T("CREATURE_BIRD_GUILD_SPARROW", 22.0f, 700.0f, 0.0f, 1)); m.Things.back().Enemy = true;
        m.Things.push_back(T("CREATURE_BANDIT", 24.0f, 720.0f, 0.0f, 2)); m.Things.back().Enemy = true;
        m.Things.push_back(T("DEMON_DOOR", 26.0f, 800.0f, 0.0f, 7)); m.Things.back().DemonDoor = true;          // any map
        m.Things.push_back(T("TRADER", 28.0f, 900.0f, 0.0f, 1)); m.Things.back().Keeper = true; m.Things.back().Shop = true; m.Things.back().Available = true;
        m.Things.push_back(T("TRADER", 30.0f, 920.0f, 0.0f, 1)); m.Things.back().Keeper = true; m.Things.back().Shop = true;   // shop closed
        m.Followers.push_back(&m.Things[1]); m.Followers.push_back(&m.Things[0]);
        Info i1; i1.PThing = &m.Things[0]; i1.Graphic = 5; m.NonAuto.push_back(i1);
        Info i2; i2.PThing = &m.Things[3]; i2.Graphic = 6; m.NonAuto.push_back(i2);                               // Y 5000: skipped
        Info i3; i3.PThing = 0; i3.Graphic = 7; m.NonAuto.push_back(i3);                                           // origin
        m.Create(region, &hero);
        size_t n = 0;
        ok = ok && !m.North && m.South && !m.C1 && !m.C2;
        ok = ok && Is(m, n, "UI_HERO_MARKER", 1.0f, 4000.0f) && Near(m.Parent[n].Angle, 0.75f); ++n;
        ok = ok && Is(m, n++, "UI_INTERESTING_MARKER", 2.0f, 100.0f) && Is(m, n++, "UI_INTERESTING_MARKER", 6.0f, 300.0f);
        ok = ok && Is(m, n, "UI_TELEPORT_MARKER", 10.0f, 400.0f) && m.Parent[n].Unavailable; ++n;
        ok = ok && Is(m, n++, "UI_HOUSE_OWNED_MARKER", 14.0f, 500.0f) && Is(m, n++, "UI_HOUSE_FOR_SALE_MARKER", 16.0f, 520.0f);
        ok = ok && Is(m, n++, "UI_ATTRACTED_MARKER", 20.0f, 600.0f) && Is(m, n++, "UI_ENEMY_MARKER", 24.0f, 720.0f);
        ok = ok && Is(m, n++, "UI_DEMON_DOOR_MARKER", 26.0f, 800.0f) && Is(m, n++, "UI_TRADER_MARKER", 28.0f, 900.0f);
        ok = ok && Is(m, n, "UI_MINI_MAP_MARKER", 2.0f, 100.0f) && m.Parent[n].Graphic == 5; ++n;
        ok = ok && Is(m, n, "UI_MINI_MAP_MARKER", 0.0f, 0.0f) && m.Parent[n].Graphic == 7; ++n;
        ok = ok && Is(m, n++, "UI_HENCHMAN_MARKER", 4.0f, 200.0f) && n == m.Parent.size() && m.RegionExits == 1;
        // cache: the region's old entry purged, the other region's kept; then teleporter, 2 houses, wife, enemy, door, trader, henchman
        ok = ok && m.MiniMapMarkers.size() == 9 && m.MiniMapMarkers[0].Def == "UI_OTHER" && m.MiniMapMarkers[1].Def == "UI_TELEPORT_MARKER"
                && m.MiniMapMarkers[8].Def == "UI_HENCHMAN_MARKER" && m.MiniMapMarkers[8].Length == 18 && m.MiniMapMarkers[8].Region == 4;
        // second visit from another region: replay of the cached entries of region 4 only (graphic only when non-zero, angle always)
        Model r = m; r.Parent.clear(); r.ParentRegion = 5; r.MiniMapMarkers[1].Graphic = 3; r.MiniMapMarkers[1].Angle = 0.5f;
        r.Create(region, &hero);
        ok = ok && r.Parent.size() == 8 && r.Parent[0].Def == "UI_TELEPORT_MARKER" && r.Parent[0].Graphic == 3 && Near(r.Parent[0].Angle, 0.5f)
                && r.Parent[1].Graphic == -1 && r.RegionExits == 1 && r.MiniMapMarkers.size() == 9;
    }
    {   // prison courtyard 1 (Z > 51 hidden), dead hero: no hero / enemy markers, no south switch; a map not loaded returns early
        Model m; Base(m); region.Graphic = 11;
        m.Things.push_back(T("CREATURE_LOW", 2.0f, 9000.0f, 10.0f, 1)); m.Things.back().Info = true;
        m.Things.push_back(T("CREATURE_UP", 4.0f, 9000.0f, 60.0f, 1)); m.Things.back().Info = true;
        m.Things.push_back(T("CREATURE_BANDIT", 6.0f, 9000.0f, 0.0f, 1)); m.Things.back().Enemy = true;
        Hero dead = hero; dead.Dead = true;
        m.Create(region, &dead);
        ok = ok && m.C1 && !m.C2 && !m.North && !m.South && m.Parent.size() == 1 && Is(m, 0, "UI_INTERESTING_MARKER", 2.0f, 9000.0f);
        Model u; Base(u); u.Maps[1].Loaded = false; Cached c; c.Region = 4; c.Def = "UI_OLD"; c.Graphic = 0; c.Angle = 0; c.Length = 6; u.MiniMapMarkers.push_back(c);
        u.Create(region, &hero);
        ok = ok && u.Parent.empty() && u.MiniMapMarkers.size() == 1 && u.RegionExits == 0;
        region.Graphic = 12; Model c2; Base(c2); c2.Create(region, 0);
        ok = ok && !c2.C1 && c2.C2 && c2.RegionExits == 1 && c2.Parent.empty();
    }
    std::printf(ok ? "00603000_TEST PASS\n" : "00603000_TEST FAIL\n");
    return ok ? 0 : 1;
}
