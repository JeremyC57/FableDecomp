// Behaviour test for CAIStateGroup_DeedReactionRoot::OnReplace @ 0x00960760: reaction flags, tutorial, virtual deed id, speech release.
#include <cstdio>
#ifndef _MSC_VER
#define __forceinline inline
#endif
struct CDeedEffectsDef {
    long RefCount; bool SpeechReaction; int destroyed;
    void Destroy() { ++destroyed; }
    void Release() { if (--RefCount == 0) Destroy(); }
};
struct CDefPointer {
    CDeedEffectsDef* Object;
    CDefPointer(CDeedEffectsDef* o) : Object(o) { if (Object) ++Object->RefCount; }
    CDefPointer(const CDefPointer& o) : Object(o.Object) { if (Object) ++Object->RefCount; }
    ~CDefPointer() { if (Object) Object->Release(); }
    CDeedEffectsDef* operator->() const { return Object; }
};
struct CWorld { unsigned long frame; unsigned long GetWorldFrame() const { return frame; } };
struct CTCDeedReaction { unsigned long ReactionFrame; bool Reacted, SpeechReacted; CWorld* world; CWorld* GetWorld() { return world; } };
struct CTCDeedEffects { CDeedEffectsDef* def; long asked; CDefPointer GetDeedEffectsDef(long t) const { ((CTCDeedEffects*)this)->asked = t; return CDefPointer(def); } };
struct CTCTutorial { bool active; int done; bool IsActive() const { return active; } void OnDeedReactionDone() { ++done; } };
struct CTCTutorialGiver { long category; int gives; void GiveTutorialForCategory(long c, bool) { category = c; ++gives; } };
struct CTCTalk { int stops; void StopCurrentSpeech() { ++stops; } };
struct CSpeechManager { long released; int calls; void ReleaseDeedSpeech(long t, long) { released = t; ++calls; } };
struct CWorldObject { CSpeechManager* PSpeech; };
struct CThing {
    unsigned char m_flags91;
    CTCDeedReaction* reaction; CTCDeedEffects* effects; CTCTutorial* tutorial; CTCTutorialGiver* giver; CTCTalk* talk;
    CThing() : m_flags91(0), reaction(0), effects(0), tutorial(0), giver(0), talk(0) { }
    bool HasTC(long type) const
    {
        switch (type) {
        case 0x4f: return reaction != 0;
        case 0x51: return effects != 0;
        case 0xd7: return tutorial != 0;
        case 0x29: return giver != 0;
        case 0x0f: return talk != 0;
        }
        return false;
    }
    void* GetTC(long type) const
    {
        switch (type) {
        case 0x4f: return reaction;
        case 0x51: return effects;
        case 0xd7: return tutorial;
        case 0x29: return giver;
        case 0x0f: return talk;
        }
        return 0;
    }
};
struct CBaseIntelligentPointer { CThing* m_pItem; CThing* GetPItem() const { return m_pItem; } };
__forceinline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}
struct CAIStateGroup_DeedReactionRoot {
    CThing* creature; CWorldObject* worldObject;
    CBaseIntelligentPointer ThingReactingTo;
    long CurrentDeedType, CurrentDeedHandle; bool DeedIsVirtual;
    long LastOpinionSourceVirtualDeedID; bool DeliveredReaction, DeliveredSpeech;
    CThing* GetCreature() const { return creature; }
    CWorldObject* GetPWorld() const { return worldObject; }
    void OnReplace();
};
void CAIStateGroup_DeedReactionRoot::OnReplace()
{
    if (DeliveredReaction) {
        if (!DeedIsVirtual) {
            CThing* creature = GetCreature();
            CTCDeedReaction* reaction = 0;
            if (creature->HasTC(0x4f))
                reaction = (CTCDeedReaction*)creature->GetTC(0x4f);
            if (IS_THING_ALIVE(ThingReactingTo)) {
                const CThing* thing = ThingReactingTo.GetPItem();
                const CTCDeedEffects* effects = 0;
                if (thing->HasTC(0x51))
                    effects = (const CTCDeedEffects*)thing->GetTC(0x51);
                CDefPointer def = effects->GetDeedEffectsDef(CurrentDeedType);
                reaction->Reacted = true;
                if (def->SpeechReaction) {
                    reaction->Reacted = true;
                    reaction->SpeechReacted = true;
                }
                reaction->ReactionFrame = reaction->GetWorld()->GetWorldFrame();
                const CThing* me = GetCreature();
                if (me->HasTC(0xd7) && ((const CTCTutorial*)me->GetTC(0xd7))->IsActive()) {
                    CThing* target = ThingReactingTo.GetPItem();
                    if (target->HasTC(0x29)) {
                        CTCTutorialGiver* giver = (CTCTutorialGiver*)target->GetTC(0x29);
                        if (giver)
                            giver->GiveTutorialForCategory(0x11, false);
                    }
                }
            }
        } else
            LastOpinionSourceVirtualDeedID = CurrentDeedHandle;
        if (CurrentDeedType == 0x59) {
            CThing* creature = GetCreature();
            if (creature->HasTC(0xd7)) {
                CTCTutorial* tutorial = (CTCTutorial*)creature->GetTC(0xd7);
                if (tutorial)
                    tutorial->OnDeedReactionDone();
            }
        }
    }
    if (DeliveredSpeech) {
        CWorldObject* world = GetPWorld();
        world->PSpeech->ReleaseDeedSpeech(CurrentDeedType, 0);
    }
    CThing* creature = GetCreature();
    if (creature->HasTC(0x0f))
        ((CTCTalk*)creature->GetTC(0x0f))->StopCurrentSpeech();
}

static int failures = 0;
static void Check(bool ok, const char* what) { if (!ok) { std::printf("FAIL %s\n", what); ++failures; } }

int main()
{
    CWorld w; w.frame = 1234;
    CTCDeedReaction reaction; reaction.ReactionFrame = 0; reaction.Reacted = false; reaction.SpeechReacted = false; reaction.world = &w;
    CTCTutorial tutorial; tutorial.active = true; tutorial.done = 0;
    CTCTalk talk; talk.stops = 0;
    CThing me; me.reaction = &reaction; me.tutorial = &tutorial; me.talk = &talk;
    CDeedEffectsDef def; def.RefCount = 1; def.SpeechReaction = false; def.destroyed = 0;
    CTCDeedEffects effects; effects.def = &def; effects.asked = -1;
    CTCTutorialGiver giver; giver.category = 0; giver.gives = 0;
    CThing hero; hero.effects = &effects; hero.giver = &giver;
    CSpeechManager speech; speech.released = 0; speech.calls = 0;
    CWorldObject wo; wo.PSpeech = &speech;
    CAIStateGroup_DeedReactionRoot g;
    g.creature = &me; g.worldObject = &wo; g.ThingReactingTo.m_pItem = &hero;
    g.CurrentDeedType = 7; g.CurrentDeedHandle = 99; g.DeedIsVirtual = false;
    g.LastOpinionSourceVirtualDeedID = 0; g.DeliveredReaction = true; g.DeliveredSpeech = true;

    // Real deed, no speech reaction: Reacted, frame stored, tutorial given, speech released, talk stopped.
    g.OnReplace();
    Check(reaction.Reacted && !reaction.SpeechReacted && reaction.ReactionFrame == 1234, "reaction recorded");
    Check(effects.asked == 7 && def.RefCount == 1 && def.destroyed == 0, "def asked and released");
    Check(giver.gives == 1 && giver.category == 0x11, "tutorial given");
    Check(speech.calls == 1 && speech.released == 7 && talk.stops == 1, "speech released and stopped");
    Check(tutorial.done == 0, "no deed-reaction-done for other deeds");

    // Speech reaction def, inactive tutorial.
    def.SpeechReaction = true; tutorial.active = false; reaction.Reacted = false;
    g.OnReplace();
    Check(reaction.Reacted && reaction.SpeechReacted, "speech reacted");
    Check(giver.gives == 1, "no tutorial when inactive");

    // Dead target: no reaction update.
    reaction.Reacted = false; hero.m_flags91 = 1; w.frame = 5;
    g.OnReplace();
    Check(!reaction.Reacted && reaction.ReactionFrame == 1234, "dead target skipped");
    hero.m_flags91 = 0;

    // Virtual deed: only the handle is remembered; deed 0x59 tells the tutorial.
    g.DeedIsVirtual = true; g.CurrentDeedType = 0x59;
    g.OnReplace();
    Check(g.LastOpinionSourceVirtualDeedID == 99 && !reaction.Reacted, "virtual deed id");
    Check(tutorial.done == 1, "deed reaction done");

    // Nothing delivered: only the talk stop.
    g.DeliveredReaction = false; g.DeliveredSpeech = false;
    int calls = speech.calls;
    g.OnReplace();
    Check(speech.calls == calls && tutorial.done == 1 && talk.stops == 5, "only talk stop");

    // No talk component: no stop.
    me.talk = 0;
    g.OnReplace();
    Check(talk.stops == 5, "no talk no stop");

    if (failures == 0)
        std::printf("00960760_TEST PASS\n");
    return failures;
}