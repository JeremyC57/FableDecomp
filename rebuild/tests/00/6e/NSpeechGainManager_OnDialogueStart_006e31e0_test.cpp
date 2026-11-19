// Behaviour test for 0x006E31E0 NSpeechGainManager::OnDialogueStart: the row's defines, includes and body verbatim. Things are raw
// zeroed images (retail offsets: UsingComponents +0x20, Components +0x44, Dead +0x91) with test doubles for the out-of-line callees
// (GetPos, the component map's LowerBound, the intelligent pointer, PeekMyPlayer / GetPControlledCreature, the sound player, GetTargetGain).
// Cases: a layer at or past DIALOGUE_LAYER_LAST and a dead speaker do nothing; a sound that is not playing does nothing; an empty list
// gets the line (IsLayerHead set, OutputGain = the target gain, the layer count bumped, the volume set on the speaker's sound player);
// a new line goes ahead of the first line on a later layer, behind same-layer lines it is not nearer than by the threshold, and ahead of
// a same-layer line whose speaker is farther from the hero by more than the threshold (strictly); dead / null same-layer speakers and a
// dead or missing hero never break the walk; the function always returns false.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#pragma intrinsic(sqrt)
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCSOUNDPLAYER
#define FABLE_RETAIL_CTCSOUNDPLAYER_ATTACHEDDIALOGUE
#define FABLE_RETAIL_CTCSOUNDPLAYER_SETSPECIFICSAMPLEVOLUME
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_PEEKDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER_GLOBALDEF
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_STLP_LIST_CREATE_NODE_NOINLINE
#define FABLE_RETAIL_NSPEECHGAINMANAGER_ONDIALOGUESTART
#define FABLE_RETAIL_NSPEECHGAINMANAGER_ACTIVEDIALOGUEINFO
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CThing.h"
#include "engine/CWorld.h"
#include "engine/CPlayer.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CTCSoundPlayer.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CGlobalDef.h"
#include "engine/ThingAlive.h"
#include "engine/NSpeechGainManager.h"

namespace NSpeechGainManager {

bool __fastcall OnDialogueStart(const CWorld& world, CThing& speaker, unsigned long sound_id, EDialogueLayer layer)
{
    if (layer < DIALOGUE_LAYER_LAST && IS_THING_ALIVE(&speaker)) {
        CTCSoundPlayer* ptc_sound_player;
        speaker.GetTC(&ptc_sound_player);
        if (ptc_sound_player->IsAttachedSoundPlaying(sound_id)) {
            const std::vector<CDialogueLayerDef>& layer_defs = world.PeekDefinitionManager().PeekGlobalDef().DialogueLayers;
            std::list<CActiveDialogueInfo>::iterator it;
            for (it = g_ActiveDialogue.begin(); it != g_ActiveDialogue.end(); ++it) {
                if (layer == it->Layer && IS_THING_ALIVE(it->ThingSpeaking)) {
                    const CThingPlayerCreature* pthing_player = world.PeekMyPlayer()->GetPControlledCreature();
                    if (IS_THING_ALIVE(pthing_player)) {
                        C3DVector old_to_speaker = it->ThingSpeaking->GetPos() - pthing_player->GetPos();
                        C3DVector new_to_speaker = speaker.GetPos() - pthing_player->GetPos();
                        if (new_to_speaker.GetMagnitude() < old_to_speaker.GetMagnitude() - layer_defs[layer].FocusShiftDistanceThreshold)
                            break;
                    }
                }
                else if (layer < it->Layer)
                    break;
            }
            it = g_ActiveDialogue.insert(it, CActiveDialogueInfo(speaker, sound_id, layer, true));
            ++g_LayerSpeakerCounts[layer];
            ptc_sound_player->SetSpecificSampleVolume(sound_id, it->OutputGain = GetTargetGain(*it, layer_defs));
        }
    }
    return false;
}

}


// ---- test doubles -----------------------------------------------------------------------------------------------------------------
struct FakeThing { double align; unsigned char raw[0x1000]; C3DVector pos; };
static FakeThing* fake_of(const void* p) { return (FakeThing*)((const char*)p - sizeof(double)); }
const C3DVector& CThing::GetPos() const { return fake_of(this)->pos; }

CBaseIntelligentPointer::CBaseIntelligentPointer() : PData(0) { }
CBaseIntelligentPointer::~CBaseIntelligentPointer() { }
CBaseObject* CBaseIntelligentPointer::GetPItem() const { return (CBaseObject*)PData; }
void CBaseIntelligentPointer::SetPItem(CBaseObject* item) { PData = (CBaseObjectPointer*)item; }

typedef CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*> > TCMap;
template<> TCMap::iterator TCMap::LowerBound(const ETCInterfaceType& key)
{
    std::pair<ETCInterfaceType,CTCBase*>** words = (std::pair<ETCInterfaceType,CTCBase*>**)this;
    std::pair<ETCInterfaceType,CTCBase*>* p = words[0];
    while (p != words[1] && p->first < key) ++p;
    return p;
}

static CThing* g_hero = 0;
static CPlayer* g_player_image = (CPlayer*)0x1234;
const CPlayer* CWorld::PeekMyPlayer() const { return g_player_image; }
CThingPlayerCreature* CPlayer::GetPControlledCreature() const { return (CThingPlayerCreature*)g_hero; }

static int g_playing = 1, g_playing_calls = 0, g_volume_calls = 0;
static const CTCSoundPlayer* g_last_player = 0; static unsigned long g_last_sample = 0, g_last_query = 0; static float g_last_volume = 0;
bool CTCSoundPlayer::IsAttachedSoundPlaying(unsigned long sample_id) const { ++g_playing_calls; g_last_query = sample_id; g_last_player = this; return g_playing != 0; }
void CTCSoundPlayer::SetSpecificSampleVolume(unsigned long sample_id, float volume) { ++g_volume_calls; g_last_sample = sample_id; g_last_volume = volume; g_last_player = this; }

static const NSpeechGainManager::CActiveDialogueInfo* g_gain_info = 0; static const void* g_gain_defs = 0;
float __fastcall NSpeechGainManager::GetTargetGain(const CActiveDialogueInfo& info, const std::vector<CDialogueLayerDef>& layer_defs)
{
    g_gain_info = &info; g_gain_defs = &layer_defs;
    return 0.5f + 0.125f * (float)info.Layer;
}

std::list<NSpeechGainManager::CActiveDialogueInfo> NSpeechGainManager::g_ActiveDialogue;
std::vector<unsigned short> NSpeechGainManager::g_LayerSpeakerCounts;

// ---- harness ------------------------------------------------------------------------------------------------------------------------
using namespace NSpeechGainManager;
static int g_fail = 0;
static void check(int ok, const char* what) { if (!ok) { printf("FAIL %s\n", what); g_fail = 1; } }

static FakeThing things[8];
static unsigned char sound_players[8][0x100];
static std::pair<ETCInterfaceType,CTCBase*> tc_maps[8][2];
static unsigned char world_image[0x200], defman_image[0x200], globaldef_image[0x400];
static unsigned char layer_def_image[3][0x40];

static CThing* MakeThing(int i, float x, float y, float z, bool sound_player)
{
    memset(&things[i], 0, sizeof(things[i]));
    unsigned char* raw = things[i].raw;
    things[i].pos.X = x; things[i].pos.Y = y; things[i].pos.Z = z;
    if (sound_player) {
        *(unsigned long*)(raw + 0x20) |= 1UL << TCI_SOUND_PLAYER;                 // UsingComponents bit 0x10
        tc_maps[i][0].first = (ETCInterfaceType)0x08; tc_maps[i][0].second = 0;
        tc_maps[i][1].first = TCI_SOUND_PLAYER; tc_maps[i][1].second = (CTCBase*)sound_players[i];
        *(void**)(raw + 0x44) = &tc_maps[i][0]; *(void**)(raw + 0x48) = &tc_maps[i][2];
    }
    return (CThing*)raw;
}
static void SetDead(CThing* t, bool dead) { unsigned char* b = (unsigned char*)t + 0x91; *b = (unsigned char)(dead ? (*b | 1) : (*b & ~1)); }

static void ClearList()
{
    while (!g_ActiveDialogue.empty()) {
        std::list<CActiveDialogueInfo>::iterator it = g_ActiveDialogue.begin();
        std::_List_node_base* n = it._M_node; n->_M_prev->_M_next = n->_M_next; n->_M_next->_M_prev = n->_M_prev;
        ((std::_List_node<CActiveDialogueInfo>*)n)->_M_data.~CActiveDialogueInfo(); free(n);
    }
    for (int i = 0; i < 3; ++i) g_LayerSpeakerCounts[i] = 0;
}
static void Push(CThing* t, unsigned long id, EDialogueLayer layer)
{
    g_ActiveDialogue.insert(g_ActiveDialogue.end(), CActiveDialogueInfo(*t, id, layer, false));
}
static int Order(unsigned long* ids, int max)
{
    int n = 0;
    for (std::list<CActiveDialogueInfo>::iterator it = g_ActiveDialogue.begin(); it != g_ActiveDialogue.end() && n < max; ++it) ids[n++] = it->SoundID;
    return n;
}
static int SameOrder(const unsigned long* want, int n)
{
    unsigned long got[16]; int m = Order(got, 16);
    if (m != n) return 0;
    for (int i = 0; i < n; ++i) if (got[i] != want[i]) return 0;
    return 1;
}
static const CActiveDialogueInfo* Find(unsigned long id)
{
    for (std::list<CActiveDialogueInfo>::iterator it = g_ActiveDialogue.begin(); it != g_ActiveDialogue.end(); ++it) if (it->SoundID == id) return &*it;
    return 0;
}

int main()
{
    g_LayerSpeakerCounts.push_back(0); g_LayerSpeakerCounts.push_back(0); g_LayerSpeakerCounts.push_back(0);
    // world -> definition manager (+0x10) -> global def (+0xD0) -> DialogueLayers (+0x10C: start, finish, end of storage)
    *(void**)(world_image + 0x10) = defman_image;
    *(void**)(defman_image + 0xD0) = globaldef_image;
    void** layers = (void**)(globaldef_image + 0x10C);
    layers[0] = layer_def_image[0]; layers[1] = layer_def_image[3]; layers[2] = layer_def_image[3];
    *(float*)(layer_def_image[0] + 0x3C) = 1.0f; *(float*)(layer_def_image[1] + 0x3C) = 2.0f; *(float*)(layer_def_image[2] + 0x3C) = 4.0f;
    const CWorld& world = *(const CWorld*)world_image;

    g_hero = MakeThing(0, 0, 0, 0, false);
    CThing* speaker = MakeThing(1, 10, 0, 0, true);
    CThing* a = MakeThing(2, 20, 0, 0, true);
    CThing* b = MakeThing(3, 0, 13, 0, true);
    CThing* c = MakeThing(4, 0, 0, 30, true);
    CThing* dead = MakeThing(5, 100, 0, 0, true); SetDead(dead, true);

    // 1. layer at or past DIALOGUE_LAYER_LAST: nothing
    check(OnDialogueStart(world, *speaker, 7, DIALOGUE_LAYER_LAST) == false, "returns false (layer last)");
    check(OnDialogueStart(world, *speaker, 7, (EDialogueLayer)9) == false, "returns false (layer 9)");
    check(g_playing_calls == 0 && g_ActiveDialogue.empty(), "layer >= LAST does nothing");
    // 2. dead speaker: nothing
    check(OnDialogueStart(world, *dead, 7, DIALOGUE_LAYER_FOREGROUND) == false && g_playing_calls == 0 && g_ActiveDialogue.empty(), "dead speaker does nothing");
    // 3. the sound is not playing: queried on the speaker's sound player, nothing inserted
    g_playing = 0;
    check(OnDialogueStart(world, *speaker, 7, DIALOGUE_LAYER_FOREGROUND) == false, "returns false (not playing)");
    check(g_playing_calls == 1 && g_last_query == 7 && g_last_player == (const CTCSoundPlayer*)sound_players[1], "IsAttachedSoundPlaying on the speaker's player");
    check(g_ActiveDialogue.empty() && g_volume_calls == 0, "not playing: nothing inserted");
    g_playing = 1;
    // 4. empty list: inserted as layer head with the target gain, counted, volume set
    check(OnDialogueStart(world, *speaker, 11, DIALOGUE_LAYER_MIDGROUND) == false, "returns false (insert)");
    { unsigned long w[] = { 11 }; check(SameOrder(w, 1), "empty list gets the line"); }
    { const CActiveDialogueInfo* i = Find(11);
      check(i && i->Layer == DIALOGUE_LAYER_MIDGROUND && i->IsLayerHead && i->OutputGain == 0.625f, "new line fields");
      check(i && i->ThingSpeaking.GetPItem() == (CBaseObject*)speaker, "new line speaker");
      check(g_gain_info == i && g_gain_defs == (const void*)layers, "GetTargetGain on the inserted line and the global def's layers"); }
    check(g_LayerSpeakerCounts[1] == 1 && g_LayerSpeakerCounts[0] == 0 && g_LayerSpeakerCounts[2] == 0, "layer count bumped");
    check(g_volume_calls == 1 && g_last_sample == 11 && g_last_volume == 0.625f && g_last_player == (const CTCSoundPlayer*)sound_players[1], "volume set");

    // 5. ahead of the first line on a later layer; behind earlier layers
    ClearList();
    Push(a, 1, DIALOGUE_LAYER_FOREGROUND); Push(b, 2, DIALOGUE_LAYER_BACKGROUND); Push(c, 3, DIALOGUE_LAYER_BACKGROUND);
    OnDialogueStart(world, *speaker, 20, DIALOGUE_LAYER_MIDGROUND);
    { unsigned long w[] = { 1, 20, 2, 3 }; check(SameOrder(w, 4), "ahead of the first later-layer line"); }
    // 6. same layer (midground, threshold 2): existing speaker b at 13, new at 10: 10 < 13 - 2 -> ahead
    ClearList();
    Push(a, 1, DIALOGUE_LAYER_FOREGROUND); Push(b, 2, DIALOGUE_LAYER_MIDGROUND);
    OnDialogueStart(world, *speaker, 21, DIALOGUE_LAYER_MIDGROUND);
    { unsigned long w[] = { 1, 21, 2 }; check(SameOrder(w, 3), "nearer by more than the threshold goes ahead"); }
    // 7. same layer, background threshold 4: 10 < 13 - 4 is false -> behind (appended)
    ClearList();
    Push(b, 2, DIALOGUE_LAYER_BACKGROUND);
    OnDialogueStart(world, *speaker, 22, DIALOGUE_LAYER_BACKGROUND);
    { unsigned long w[] = { 2, 22 }; check(SameOrder(w, 2), "not nearer by the threshold stays behind"); }
    // 8. exactly at the threshold (13 - 3 == 10, threshold 3): strict, stays behind
    *(float*)(layer_def_image[2] + 0x3C) = 3.0f;
    ClearList(); Push(b, 2, DIALOGUE_LAYER_BACKGROUND);
    OnDialogueStart(world, *speaker, 23, DIALOGUE_LAYER_BACKGROUND);
    { unsigned long w[] = { 2, 23 }; check(SameOrder(w, 2), "equal to the threshold stays behind (strict <)"); }
    *(float*)(layer_def_image[2] + 0x3C) = 4.0f;
    // 9. distances are 3D magnitudes from the hero: c at (0,0,30) vs new at (10,0,0), foreground threshold 1 -> ahead
    ClearList(); Push(c, 3, DIALOGUE_LAYER_FOREGROUND);
    OnDialogueStart(world, *speaker, 24, DIALOGUE_LAYER_FOREGROUND);
    { unsigned long w[] = { 24, 3 }; check(SameOrder(w, 2), "3D distance from the hero"); }
    // 10. hero moved next to the old speaker: the new one is now farther -> behind
    things[0].pos.X = 0; things[0].pos.Y = 0; things[0].pos.Z = 29;
    ClearList(); Push(c, 3, DIALOGUE_LAYER_FOREGROUND);
    OnDialogueStart(world, *speaker, 25, DIALOGUE_LAYER_FOREGROUND);
    { unsigned long w[] = { 3, 25 }; check(SameOrder(w, 2), "distances measured from the hero"); }
    things[0].pos.Z = 0;
    // 11. same-layer line with a dead speaker: never breaks (would be far: 100), walk continues to the later layer
    ClearList(); Push(dead, 5, DIALOGUE_LAYER_FOREGROUND); Push(b, 2, DIALOGUE_LAYER_MIDGROUND);
    OnDialogueStart(world, *speaker, 26, DIALOGUE_LAYER_FOREGROUND);
    { unsigned long w[] = { 5, 26, 2 }; check(SameOrder(w, 3), "dead same-layer speaker skipped"); }
    // 12. same-layer line with a null speaker: skipped too
    ClearList(); Push(a, 1, DIALOGUE_LAYER_FOREGROUND);
    g_ActiveDialogue.begin()->ThingSpeaking.SetPItem(0);
    OnDialogueStart(world, *speaker, 27, DIALOGUE_LAYER_FOREGROUND);
    { unsigned long w[] = { 1, 27 }; check(SameOrder(w, 2), "null same-layer speaker skipped"); }
    // 13. dead hero: no distance test, no break
    SetDead(g_hero, true);
    ClearList(); Push(a, 1, DIALOGUE_LAYER_FOREGROUND);
    a = MakeThing(2, 50, 0, 0, true); 
    OnDialogueStart(world, *speaker, 28, DIALOGUE_LAYER_FOREGROUND);
    { unsigned long w[] = { 1, 28 }; check(SameOrder(w, 2), "dead hero: no break"); }
    SetDead(g_hero, false);
    // 14. no hero: no break
    CThing* hero = g_hero; g_hero = 0;
    ClearList(); Push(a, 1, DIALOGUE_LAYER_FOREGROUND);
    OnDialogueStart(world, *speaker, 29, DIALOGUE_LAYER_FOREGROUND);
    { unsigned long w[] = { 1, 29 }; check(SameOrder(w, 2), "no hero: no break"); }
    g_hero = hero;
    // 15. with the hero back, the far line (50) is passed: ahead
    ClearList(); Push(a, 1, DIALOGUE_LAYER_FOREGROUND);
    OnDialogueStart(world, *speaker, 30, DIALOGUE_LAYER_FOREGROUND);
    { unsigned long w[] = { 30, 1 }; check(SameOrder(w, 2), "far same-layer line passed"); }
    // 16. counts accumulate per layer
    check(g_LayerSpeakerCounts[0] == 1 && g_LayerSpeakerCounts[1] == 0 && g_LayerSpeakerCounts[2] == 0, "count per layer after reset");
    OnDialogueStart(world, *speaker, 31, DIALOGUE_LAYER_FOREGROUND);
    check(g_LayerSpeakerCounts[0] == 2, "count accumulates");
    ClearList();
    if (!g_fail) printf("006e31e0_TEST PASS\n");
    return g_fail;
}