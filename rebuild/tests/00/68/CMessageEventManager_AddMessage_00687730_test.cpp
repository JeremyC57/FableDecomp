// Behaviour test for CMessageEventManager::AddMessage(const CMessageEventDef&, ...) (0x00687730): a model of the row -- the event
// takes the def's type, radii (int -> float), lifespan and reaction speech, the next message id, the originator / target UIDs (0 for
// a null pointer) and the target's TCI_ENEMY faction (0 without the component); it is pushed on the front and returned.
#include <stdio.h>
#include <list>
struct Faction { int id; };
struct Thing { unsigned long long UID; Faction* enemyFaction; };   // enemyFaction == 0: no TCI_ENEMY component
struct Def { long SightRadius, HearingRadius; unsigned long Lifespan; int Reaction, Type; };
struct Event { int Type; float X, Y, Z, Sight, Hearing; unsigned long long OrigUID, TargetUID; Faction* PTargetFaction;
               long TimeStamp; unsigned long Lifespan; long ExtraData; unsigned long MessageID; int Reaction; };
static unsigned long g_NextMessageID = 7;
static long g_frame = 100;
static Event* AddMessage(std::list<Event>& messages, const Def& def, const float pos[3], Thing* originator, Thing* target, long extra)
{
    Event e;
    e.TimeStamp = g_frame;
    e.X = pos[0]; e.Y = pos[1]; e.Z = pos[2];
    e.Hearing = (float)def.HearingRadius; e.Sight = (float)def.SightRadius;
    e.Lifespan = def.Lifespan; e.Type = def.Type; e.MessageID = g_NextMessageID++; e.Reaction = def.Reaction;
    e.PTargetFaction = 0;
    e.OrigUID = originator ? originator->UID : 0;
    if (target) {
        e.TargetUID = target->UID;
        if (target->enemyFaction) e.PTargetFaction = target->enemyFaction;
    } else
        e.TargetUID = 0;
    e.ExtraData = extra;
    messages.push_front(e);
    return &*messages.begin();
}
int main()
{
    std::list<Event> messages;
    Faction f = { 3 };
    Thing a = { 11, 0 }, b = { 22, &f };
    Def def = { 5, 9, 30, 2, 4 };
    float pos[3] = { 1, 2, 3 };
    Event* e1 = AddMessage(messages, def, pos, &a, &b, 77);
    Event* e2 = AddMessage(messages, def, pos, 0, &a, 0);
    bool ok = e1->OrigUID == 11 && e1->TargetUID == 22 && e1->PTargetFaction == &f && e1->Sight == 5.0f && e1->Hearing == 9.0f
        && e1->MessageID == 7 && e1->ExtraData == 77 && e1->Z == 3.0f
        && e2->OrigUID == 0 && e2->TargetUID == 11 && e2->PTargetFaction == 0 && e2->MessageID == 8
        && &messages.front() == e2 && messages.size() == 2;
    printf(ok ? "00687730_TEST PASS\n" : "00687730_TEST FAIL\n");
    return ok ? 0 : 1;
}