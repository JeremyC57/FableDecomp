// Behaviour test for CTCSoundPlayer::PlayAllSoundEvents (0x0066B370): the same statement text over small models of the engine classes.
#include <cstdio>
#include <string>
#include <vector>
typedef std::string CCharString;
static bool g_failed;
#define CHECK(c) do { if (!(c)) { g_failed = true; std::printf("CHECK failed line %d\n", __LINE__); } } while (0)
class C3DVector { public: float X, Y, Z; };
bool g_bSoundEnabled = true;
bool g_bPlayAnimationSoundEvents = true;
const unsigned long g_InvalidSoundHandle = ~0UL;
static bool g_in_range;
static float g_clip_seen;
bool GFWorld3DDistanceSmallerThan(const C3DVector&, const C3DVector&, float distance) { g_clip_seen = distance; return g_in_range; }
class CASoundBank { public: std::vector<std::string> attempts; std::string playable; };
class CASoundEmitter { public: std::vector<std::string> stopped; void StopAllCriteriaContainingString(const CCharString& text) { stopped.push_back(text); } };
class CThingSoundEmitter { public:
    CASoundEmitter* PSoundEmitter;
    void StopAllCriteriaContainingString(const CCharString& text) { if (PSoundEmitter) PSoundEmitter->StopAllCriteriaContainingString(text); }
};
template <class T> class CIntelligentPointer { public: T* p; T* GetPItem() const { return p; } T* operator->() const { return p; } };
class CTransitionBlendAlpha { public: float CurrentAlpha, LastAlpha; };
class CAnimSet { public:
    std::vector<short> Anims; std::vector<CTransitionBlendAlpha> InternalAlphas; float CurrentPercentageComplete, LastPercentageComplete;
    const std::vector<short>& GetAnimIndices() const { return Anims; }
    float GetAnimBlendStrength(long index) const { return InternalAlphas[index].CurrentAlpha; }
    float GetPercentageComplete() const { return CurrentPercentageComplete; }
    float GetLastPercentageComplete() const { return LastPercentageComplete; }
    void Add(short anim, float alpha) { Anims.push_back(anim); CTransitionBlendAlpha a; a.CurrentAlpha = alpha; a.LastAlpha = 0.0f; InternalAlphas.push_back(a); }
};
class CAnimateThing { public:
    CAnimSet Main; std::vector<CAnimSet*> Combination;
    CAnimSet& GetMainAnim() { return Main; }
    void GetAllCombinationAnims(std::vector<CAnimSet*>& sets) { for (unsigned i = 0; i < Combination.size(); ++i) sets.push_back(Combination[i]); }
};
class CTCAnimationComplex { public: CAnimateThing Animation; CAnimateThing& GetAnimation() { return Animation; } };
namespace NAnimationEvents {
class CEvent { public: unsigned long Index : 30; unsigned long ExternallyImported : 1; unsigned long StopEvent : 1; float Time; };
}
template <class T, long N> class CSmallVector { public:
    std::vector<T> v;
    const T* begin() const { return v.empty() ? 0 : &v[0]; }
    const T* end() const { return v.empty() ? 0 : &v[0] + v.size(); }
};
class CSymbolInfo { public: CCharString GroupName, EntryName; long Val; };
class CSymbolMap { public:
    std::vector<CSymbolInfo> infos;
    const CSymbolInfo* FindFirstSymbolWithVal(long value) const { for (unsigned i = 0; i < infos.size(); ++i) if (infos[i].Val == value) return &infos[i]; return 0; }
};
namespace NAnimationEvents {
class CManager { public:
    CSymbolMap EventSymbolMap;
    struct Rec { unsigned long anim; CEvent event; };
    std::vector<Rec> recs;
    mutable int queries;
    const CSymbolMap& GetEventSymbolMap() const { return EventSymbolMap; }
    bool GetAnimEventsBetweenTimes(unsigned long anim, CSmallVector<CEvent, 32>& events, float from, float to) const {
        ++queries;
        for (unsigned i = 0; i < recs.size(); ++i)
            if (recs[i].anim == anim && recs[i].event.Time > from && recs[i].event.Time <= to) events.v.push_back(recs[i].event);
        return !events.v.empty();
    }
};
static CManager g_manager;
CManager& GetSoundEventManager() { return g_manager; }
}
class CEntitySoundDef { public: CCharString SoundIdentifier; };
class CEngineThemeDef { public: long MaterialDef; };
class CMaterialDef { public: CCharString name; const CCharString& GetInstantiationName() const { return name; } };
template <class T> class CDefPointer { public: const T* Object; CDefPointer() : Object(0) { } const T* operator->() const { return Object; } };
static CMaterialDef g_materials[4];
static CEngineThemeDef g_theme;
class CDefinitionManager { public:
    bool GetDef(long index, CDefPointer<CEngineThemeDef>* out) const { if (index > 0) { out->Object = &g_theme; return true; } return false; }
    bool GetDef(long index, CDefPointer<CMaterialDef>* out) const { if (index > 0 && index < 4) { out->Object = &g_materials[index]; return true; } return false; }
};
static CDefinitionManager g_defs;
class CWorldMap { public: long theme; long GetStrongestEngineThemeAt(const C3DVector&, long& strength) const { strength = 1; return theme; } };
static CWorldMap g_world_map;
class CThingBaseDef { public: const CEntitySoundDef* sound; bool GetSubDef(CDefPointer<CEntitySoundDef>* out) const { if (sound) { out->Object = sound; return true; } return false; } };
class CTCPhysicsBase { public: bool on_ground; bool IsOnGroundSurface() const { return on_ground; } };
class CThing { public:
    C3DVector pos; CASoundBank bank; CThingBaseDef base_def; CTCPhysicsBase physics; CThing* standing_on; long floor_material; CTCAnimationComplex* anim_tc;
    const C3DVector& GetPos() const { return pos; }
    CASoundBank& GetSampleBank() { return bank; }
    const CThingBaseDef& PeekBaseDef() const { return base_def; }
    CTCPhysicsBase* GetPhysicsTC() { return &physics; }
    CThing* GetThingStandingOn() { return standing_on; }
    long GetFloorMaterial() const { return floor_material; }
    const CDefinitionManager& PeekDefinitionManager() const { return g_defs; }
    bool GetTC(CTCAnimationComplex** out) { if (anim_tc) { *out = anim_tc; return true; } return false; }
};
static C3DVector g_lod_centre;
class CTCSoundPlayer { public:
    CThing* ParentThing;
    CIntelligentPointer<CThingSoundEmitter> PAttachedEmitter;
    CCharString VoiceName, ExtraTag;
    short AnimCriteriaClipDistance;
    long LastMaterialIndex;
    CThing& GetParentThing() { return *ParentThing; }
    const CDefinitionManager& GetDefinitionManager() const { return g_defs; }
    const CWorldMap& PeekWorldMap() const { return g_world_map; }
    unsigned long InternalPlaySoundEvent(CASoundBank& bank, const CCharString& name, bool a, bool b, bool c, long d) {
        CHECK(!a && !b && !c && d == -1);
        bank.attempts.push_back(name);
        return bank.playable == name ? 7 : g_InvalidSoundHandle;
    }
    void PlayAllSoundEvents();
};
void CTCSoundPlayer::PlayAllSoundEvents()
{
    if (!g_bSoundEnabled || !g_bPlayAnimationSoundEvents ||
        !GFWorld3DDistanceSmallerThan(g_lod_centre, GetParentThing().GetPos(), AnimCriteriaClipDistance))
    {
        return;
    }
    CTCAnimationComplex* ptc_animation;
    if (GetParentThing().GetTC(&ptc_animation))
    {
        CCharString sound_identifier;
        CDefPointer<CEntitySoundDef> psound_def;
        if (GetParentThing().PeekBaseDef().GetSubDef(&psound_def))
            sound_identifier = psound_def->SoundIdentifier;
        if (sound_identifier != "")
        {
            CASoundBank& bank = GetParentThing().GetSampleBank();
            CCharString material_name;
            long material_index = 0;
            CThing* pstanding_on_thing = GetParentThing().GetThingStandingOn();
            if (pstanding_on_thing)
            {
                material_index = pstanding_on_thing->GetFloorMaterial();
                LastMaterialIndex = material_index;
            }
            else if (GetParentThing().GetPhysicsTC()->IsOnGroundSurface())
            {
                CDefPointer<CEngineThemeDef> pdef;
                long strength;
                if (GetDefinitionManager().GetDef(PeekWorldMap().GetStrongestEngineThemeAt(GetParentThing().GetPos(), strength), &pdef))
                {
                    material_index = pdef->MaterialDef;
                    LastMaterialIndex = material_index;
                }
            }
            else
                material_index = LastMaterialIndex;
            if (material_index > 0)
            {
                CDefPointer<CMaterialDef> pmaterial_def;
                GetParentThing().PeekDefinitionManager().GetDef(material_index, &pmaterial_def);
                material_name = pmaterial_def->GetInstantiationName();
            }
            CAnimateThing& anim = ptc_animation->GetAnimation();
            const NAnimationEvents::CManager& sound_event_manager = NAnimationEvents::GetSoundEventManager();
            std::vector<CAnimSet*> anim_sets;
            anim.GetAllCombinationAnims(anim_sets);
            CAnimSet& main_anim = anim.GetMainAnim();
            anim_sets.push_back(&main_anim);
            for (std::vector<CAnimSet*>::const_iterator it = anim_sets.begin(); it != anim_sets.end(); ++it)
            {
                const CAnimSet* panim_set = *it;
                const std::vector<short>& anim_indeces = panim_set->GetAnimIndices();
                long index_to_use = 0;
                float best_strength = 0.0f;
                for (long c1 = 0; c1 < (long)anim_indeces.size(); c1++)
                {
                    float strength = panim_set->GetAnimBlendStrength(c1);
                    if (strength > best_strength)
                    {
                        best_strength = strength;
                        index_to_use = anim_indeces[c1];
                    }
                }
                if (index_to_use != 0 &&
                    panim_set->GetPercentageComplete() > panim_set->GetLastPercentageComplete())
                {
                    CSmallVector<NAnimationEvents::CEvent, 32> event_container;
                    if (sound_event_manager.GetAnimEventsBetweenTimes(index_to_use, event_container, panim_set->LastPercentageComplete, panim_set->CurrentPercentageComplete))
                    {
                        for (const NAnimationEvents::CEvent* event_it = event_container.begin(); event_it != event_container.end(); event_it++)
                        {
                            const CSymbolInfo* psymbol_info = sound_event_manager.GetEventSymbolMap().FindFirstSymbolWithVal(event_it->Index);
                            const CCharString& loop_event_name = psymbol_info->EntryName;
                            if (event_it->StopEvent)
                            {
                                if (PAttachedEmitter.GetPItem())
                                    PAttachedEmitter->StopAllCriteriaContainingString(loop_event_name);
                            }
                            else
                            {
                                CCharString sound_name;
                                sound_name += sound_identifier;
                                sound_name += ";";
                                sound_name += loop_event_name;
                                CCharString voice_name(VoiceName);
                                CCharString extra_tag(ExtraTag);
                                bool played_sound = false;
                                if (voice_name != "")
                                {
                                    if (extra_tag != "")
                                    {
                                        CCharString sound_name_and_voice = sound_name + ";" + voice_name + ";" + extra_tag;
                                        if (InternalPlaySoundEvent(bank, sound_name_and_voice, false, false, false, -1) != g_InvalidSoundHandle)
                                            played_sound = true;
                                    }
                                    else
                                    {
                                        CCharString sound_name_and_voice = sound_name + ";" + voice_name;
                                        if (InternalPlaySoundEvent(bank, sound_name_and_voice, false, false, false, -1) != g_InvalidSoundHandle)
                                            played_sound = true;
                                    }
                                }
                                if (!played_sound)
                                {
                                    CCharString sound_name_and_tag_name_and_material = sound_name + ";" + material_name + ";" + extra_tag;
                                    if (InternalPlaySoundEvent(bank, sound_name_and_tag_name_and_material, false, false, false, -1) == g_InvalidSoundHandle)
                                    {
                                        CCharString sound_name_and_material = sound_name + ";" + material_name;
                                        if (InternalPlaySoundEvent(bank, sound_name_and_material, false, false, false, -1) == g_InvalidSoundHandle)
                                            if (InternalPlaySoundEvent(bank, sound_name, false, false, false, -1) == g_InvalidSoundHandle)
                                                InternalPlaySoundEvent(bank, loop_event_name, false, false, false, -1);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

static void AddSymbol(long val, const char* name) { CSymbolInfo s; s.GroupName = "G"; s.EntryName = name; s.Val = val; NAnimationEvents::g_manager.EventSymbolMap.infos.push_back(s); }
static void AddEvent(unsigned long anim, unsigned long index, bool stop, float time) {
    NAnimationEvents::CManager::Rec r; r.anim = anim; r.event.Index = index; r.event.ExternallyImported = 0; r.event.StopEvent = stop ? 1 : 0; r.event.Time = time;
    NAnimationEvents::g_manager.recs.push_back(r);
}
struct World {
    CThing thing; CThing floor; CTCAnimationComplex anim_tc; CEntitySoundDef sound_def; CASoundEmitter emitter; CThingSoundEmitter thingEmitter; CTCSoundPlayer player;
    World() {
        thing.pos.X = thing.pos.Y = thing.pos.Z = 0.0f; thing.base_def.sound = &sound_def; thing.physics.on_ground = false; thing.standing_on = 0;
        thing.floor_material = 0; thing.anim_tc = &anim_tc; floor.floor_material = 2; floor.standing_on = 0; floor.anim_tc = 0; floor.base_def.sound = 0;
        sound_def.SoundIdentifier = "HERO"; thingEmitter.PSoundEmitter = &emitter;
        player.ParentThing = &thing; player.PAttachedEmitter.p = &thingEmitter; player.AnimCriteriaClipDistance = 30; player.LastMaterialIndex = 3;
        anim_tc.Animation.Main.CurrentPercentageComplete = 0.5f; anim_tc.Animation.Main.LastPercentageComplete = 0.25f;
        anim_tc.Animation.Main.Add(11, 0.2f); anim_tc.Animation.Main.Add(12, 0.9f); anim_tc.Animation.Main.Add(13, 0.4f);
    }
};
static std::vector<std::string> Expect(const char* a, const char* b = 0, const char* c = 0, const char* d = 0, const char* e = 0) {
    std::vector<std::string> v; const char* s[5] = { a, b, c, d, e };
    for (int i = 0; i < 5; ++i) if (s[i]) v.push_back(s[i]);
    return v;
}
int main()
{
    g_materials[1].name = "GRASS"; g_materials[2].name = "WOOD"; g_materials[3].name = "STONE"; g_theme.MaterialDef = 1; g_world_map.theme = 5;
    AddSymbol(40, "FOOT"); AddSymbol(41, "SWISH");
    AddEvent(12, 40, false, 0.3f);      // the strongest anim (12), inside (0.25, 0.5]
    AddEvent(12, 41, true, 0.4f);       // a stop event
    AddEvent(12, 40, false, 0.9f);      // outside the window
    AddEvent(11, 41, false, 0.3f);      // a weaker anim: never queried
    g_in_range = true;
    {   // early outs: sound off, flag off, out of range, no animation component, no sound identifier
        World w; g_bSoundEnabled = false; w.player.PlayAllSoundEvents(); g_bSoundEnabled = true; CHECK(w.thing.bank.attempts.empty());
        g_bPlayAnimationSoundEvents = false; w.player.PlayAllSoundEvents(); g_bPlayAnimationSoundEvents = true; CHECK(w.thing.bank.attempts.empty());
        g_in_range = false; g_clip_seen = 0.0f; w.player.PlayAllSoundEvents(); g_in_range = true; CHECK(w.thing.bank.attempts.empty()); CHECK(g_clip_seen == 30.0f);
        w.thing.anim_tc = 0; w.player.PlayAllSoundEvents(); w.thing.anim_tc = &w.anim_tc; CHECK(w.thing.bank.attempts.empty());
        w.sound_def.SoundIdentifier = ""; w.player.PlayAllSoundEvents(); CHECK(w.thing.bank.attempts.empty());
        w.thing.base_def.sound = 0; w.player.PlayAllSoundEvents(); CHECK(w.thing.bank.attempts.empty()); CHECK(w.emitter.stopped.empty());
    }
    {   // not on anything, not on the ground: the last material (3 = STONE); nothing playable: the whole fallback chain, then the stop event
        World w; w.player.PlayAllSoundEvents();
        CHECK(w.thing.bank.attempts == Expect("HERO;FOOT;STONE;", "HERO;FOOT;STONE", "HERO;FOOT", "FOOT"));
        CHECK(w.emitter.stopped == Expect("SWISH")); CHECK(w.player.LastMaterialIndex == 3);
    }
    {   // standing on a thing: its floor material (2 = WOOD) is taken and remembered; the material name plays
        World w; w.thing.standing_on = &w.floor; w.thing.bank.playable = "HERO;FOOT;WOOD"; w.player.PlayAllSoundEvents();
        CHECK(w.thing.bank.attempts == Expect("HERO;FOOT;WOOD;", "HERO;FOOT;WOOD")); CHECK(w.player.LastMaterialIndex == 2);
    }
    {   // on the ground: the strongest engine theme's material (1 = GRASS); voice and extra tag set, the voice + extra name plays
        World w; w.thing.physics.on_ground = true; w.player.VoiceName = "VOICE"; w.player.ExtraTag = "WET";
        w.thing.bank.playable = "HERO;FOOT;VOICE;WET"; w.player.PlayAllSoundEvents();
        CHECK(w.thing.bank.attempts == Expect("HERO;FOOT;VOICE;WET")); CHECK(w.player.LastMaterialIndex == 1);
    }
    {   // voice without extra tag fails: falls to material + (empty) extra, which plays
        World w; w.thing.physics.on_ground = true; w.player.VoiceName = "VOICE"; w.thing.bank.playable = "HERO;FOOT;GRASS;"; w.player.PlayAllSoundEvents();
        CHECK(w.thing.bank.attempts == Expect("HERO;FOOT;VOICE", "HERO;FOOT;GRASS;"));
    }
    {   // on the ground with no theme def: the material stays 0 and LastMaterialIndex is untouched; no attached emitter: the stop event is ignored
        World w; w.thing.physics.on_ground = true; g_world_map.theme = 0; w.player.PAttachedEmitter.p = 0; w.thing.bank.playable = "HERO;FOOT"; w.player.PlayAllSoundEvents();
        g_world_map.theme = 5;
        CHECK(w.thing.bank.attempts == Expect("HERO;FOOT;;", "HERO;FOOT;", "HERO;FOOT")); CHECK(w.player.LastMaterialIndex == 3); CHECK(w.emitter.stopped.empty());
    }
    {   // no progress this frame: no event query; an emitter without a sound emitter ignores the stop
        World w; w.anim_tc.Animation.Main.CurrentPercentageComplete = 0.25f; int q = NAnimationEvents::g_manager.queries; w.player.PlayAllSoundEvents();
        CHECK(w.thing.bank.attempts.empty()); CHECK(NAnimationEvents::g_manager.queries == q);
        World v; v.thingEmitter.PSoundEmitter = 0; v.player.PlayAllSoundEvents(); CHECK(v.emitter.stopped.empty()); CHECK(v.thing.bank.attempts.size() == 4);
    }
    {   // a combination anim set is visited before the main one; a set whose strongest alpha is 0 (or index 0) is skipped
        World w; CAnimSet extra; extra.CurrentPercentageComplete = 1.0f; extra.LastPercentageComplete = 0.0f; extra.Add(11, 0.7f);
        CAnimSet none; none.CurrentPercentageComplete = 1.0f; none.LastPercentageComplete = 0.0f; none.Add(12, 0.0f);
        w.anim_tc.Animation.Combination.push_back(&extra); w.anim_tc.Animation.Combination.push_back(&none);
        w.thing.bank.playable = "HERO;SWISH"; w.player.PlayAllSoundEvents();
        CHECK(w.thing.bank.attempts.size() == 3 + 4); CHECK(w.thing.bank.attempts[2] == "HERO;SWISH"); CHECK(w.thing.bank.attempts[3] == "HERO;FOOT;STONE;");
    }
    std::puts(g_failed ? "0066b370_TEST FAIL" : "0066b370_TEST PASS");
    return g_failed ? 1 : 0;
}
