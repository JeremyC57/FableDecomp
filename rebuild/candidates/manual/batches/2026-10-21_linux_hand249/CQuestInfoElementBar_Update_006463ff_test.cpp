#include <cstdio>
// behaviour test: CQuestInfoElementBar::Update colours by health ratio, and re-acquires a lost thing in the current region
struct CThing { short MapNumber; unsigned char Flags91; float MaxHealth; float Health; };
typedef CThing CThingPhysical;
static CThing* g_lookup = 0; static long g_currentRegion = 5;
struct CIntelligentPointer { CThing* item; CThing* GetPItem() const { return item; } };
static bool IS_THING_ALIVE(const CIntelligentPointer* p) { return p->item && !(p->item->Flags91 & 1); }
static bool IS_THING_ALIVE_RAW(CThing* const* p) { return *p && !((*p)->Flags91 & 1); }
struct CThingManager { CThing* GetThingWithUID(unsigned long long) { return g_lookup; } };
struct CWorldMap { long GetRegionNumberMapIsIn(long map) { return map / 10; } long GetCurrentRegionNumber() { return g_currentRegion; } };
struct CWorld { CThingManager tm; CWorldMap wm; CThingManager* GetThingManager() { return &tm; } CWorldMap* GetWorldMap() { return &wm; } };
struct CRGBColour { unsigned char B, G, R, A; };
struct CQuestInfoElementBar {
    float Value, MaxValue, MinValue; CRGBColour Colour; bool ThingValid; CIntelligentPointer PThing;
    unsigned long long ThingUID; long ThingMapNumber; CWorld* world;
    bool Update();
};
bool CQuestInfoElementBar::Update()
{
    if (IS_THING_ALIVE(&PThing)) {
        if (PThing.GetPItem()->Flags91 & 0x20)
            return true;
        ThingMapNumber = PThing.GetPItem()->MapNumber;
        Value = PThing.GetPItem()->Health;
        MinValue = 0.0f;
        MaxValue = PThing.GetPItem()->MaxHealth;
        if (MaxValue > MinValue && Value >= MinValue && MaxValue >= Value) {
            float ratio = Value / (MaxValue - MinValue);
            if (ratio > 1.0f)
                ratio = 1.0f;
            Colour.R = (unsigned char)((1.0f - ratio) * 255.0f);
            Colour.G = (unsigned char)(ratio * 255.0f);
            Colour.B = 0;
        }
    } else if (ThingValid && ThingUID && ThingMapNumber) {
        CWorldMap* map = world->GetWorldMap();
        if (map->GetCurrentRegionNumber() == map->GetRegionNumberMapIsIn(ThingMapNumber)) {
            CThing* thing = world->GetThingManager()->GetThingWithUID(ThingUID);
            if (IS_THING_ALIVE_RAW(&thing)) {
                CIntelligentPointer found = { thing };
                PThing = found;
            }
        }
    }
    return true;
}
int main()
{
    CWorld w; CThing troll = { 52, 0, 100.0f, 25.0f };
    CQuestInfoElementBar bar; bar.world = &w; bar.PThing.item = &troll; bar.ThingValid = true; bar.ThingUID = 7; bar.ThingMapNumber = 0;
    bar.Colour.B = 9;
    bool ok = bar.Update() && bar.ThingMapNumber == 52 && bar.Value == 25.0f && bar.MaxValue == 100.0f
           && bar.Colour.R == 191 && bar.Colour.G == 63 && bar.Colour.B == 0;
    bar.PThing.item = 0; g_lookup = &troll; g_currentRegion = 4;
    ok = ok && bar.Update() && bar.PThing.item == 0;              // map 52 is region 5, not current
    g_currentRegion = 5;
    ok = ok && bar.Update() && bar.PThing.item == &troll;         // re-acquired
    troll.Flags91 = 0x20; bar.Value = -1;
    ok = ok && bar.Update() && bar.Value == -1;                    // hidden thing: untouched
    if (!ok) { std::printf("006463ff_TEST FAIL\n"); return 1; }
    std::printf("006463ff_TEST PASS\n");
    return 0;
}
