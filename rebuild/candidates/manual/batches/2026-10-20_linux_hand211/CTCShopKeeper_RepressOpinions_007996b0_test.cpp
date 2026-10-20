// Behaviour test for CTCShopKeeper::RepressOpinions (0x007996B0): only a change is acted on; a creature keeper with an opinion
// component gets deed 77's mask entry = !repress and the on-the-job masks (repress) or its def's default masks (release).
#include <cstdio>
#include <cstring>
class CCharString {
public:
    const char* PStringData;
    CCharString(const char* text, long length) : PStringData(text) { }
    ~CCharString() { }
};
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) { SEntry* p = first; while (p != last && p->type < *type) ++p; return p; }
};
struct CThing {
    unsigned char ThingClass;
    unsigned long m_presentBits[9];
    CInterfaceMap m_interfaces;
    unsigned long HasTC(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    CTCBase* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (it == map->last || interfaceType < it->type ? map->last : it)->component;
    }
};
struct COpinionOfHeroDef {
    long ReactionMask;
    long DeedMask;
};
struct CBoolVector {
    bool* _M_start; bool* _M_finish; bool* _M_end_of_storage;
    bool& operator[](long i) { return *(_M_start + i); }
};
struct CTCCreatureOpinionOfHero {
    COpinionOfHeroDef* PDef;
    CBoolVector DeedMask;
    long reactionIndex, deedIndex; const char* reactionName; const char* deedName; int calls;
    void SetOpinionReactionMask(long defIndex) { reactionIndex = defIndex; reactionName = 0; ++calls; }
    void SetOpinionReactionMask(const CCharString& defName) { reactionName = defName.PStringData; reactionIndex = -1; ++calls; }
    void SetOpinionDeedMask(long defIndex) { deedIndex = defIndex; deedName = 0; ++calls; }
    void SetOpinionDeedMask(const CCharString& defName) { deedName = defName.PStringData; deedIndex = -1; ++calls; }
};
struct CTCShopKeeper {
    virtual void v00() { }
    CThing* ParentThing;
    bool RepressedOpinions;
    void RepressOpinions(bool repress);
};

void CTCShopKeeper::RepressOpinions(bool repress)
{
    if (repress != RepressedOpinions) {
        CThing* thing = ParentThing;
        RepressedOpinions = repress;
        if (thing->HasTC(0x4f)) {
            CTCCreatureOpinionOfHero* opinion = (CTCCreatureOpinionOfHero*)thing->GetTC(0x4f);
            if (thing->ThingClass == 1) {
                opinion->DeedMask[0x4d] = !repress;
                if (RepressedOpinions) {
                    {
                        CCharString name("OPINION_DEED_MASK_SHOPKEEPER_ON_THE_JOB", -1);
                        opinion->SetOpinionDeedMask(name);
                    }
                    {
                        CCharString name("OPINION_REACTION_MASK_SHOPKEEPER_ON_THE_JOB", -1);
                        opinion->SetOpinionReactionMask(name);
                    }
                } else {
                    opinion->SetOpinionDeedMask(opinion->PDef->DeedMask);
                    opinion->SetOpinionReactionMask(opinion->PDef->ReactionMask);
                }
            }
        }
    }
}

int main()
{
    int bad = 0;
    bool mask[100]; std::memset(mask, 0, sizeof(mask)); mask[0x4d] = true;
    COpinionOfHeroDef def; def.ReactionMask = 11; def.DeedMask = 22;
    CTCCreatureOpinionOfHero op; op.PDef = &def; op.DeedMask._M_start = mask; op.DeedMask._M_finish = mask + 100; op.calls = 0;
    CInterfaceMap::SEntry e[1] = { { 0x4f, (CTCBase*)&op } };
    CThing thing; thing.ThingClass = 1; std::memset(thing.m_presentBits, 0, sizeof(thing.m_presentBits));
    thing.m_presentBits[0x4f >> 5] = 1UL << (0x4f & 31); thing.m_interfaces.first = e; thing.m_interfaces.last = e + 1;
    CTCShopKeeper k; k.ParentThing = &thing; k.RepressedOpinions = false;
    k.RepressOpinions(false);                         // no change: nothing
    if (op.calls != 0 || !mask[0x4d]) bad |= 1;
    k.RepressOpinions(true);
    if (!k.RepressedOpinions || mask[0x4d] || op.calls != 2) bad |= 2;
    if (!op.deedName || std::strcmp(op.deedName, "OPINION_DEED_MASK_SHOPKEEPER_ON_THE_JOB")) bad |= 4;
    if (!op.reactionName || std::strcmp(op.reactionName, "OPINION_REACTION_MASK_SHOPKEEPER_ON_THE_JOB")) bad |= 8;
    k.RepressOpinions(false);
    if (k.RepressedOpinions || !mask[0x4d] || op.calls != 4 || op.deedIndex != 22 || op.reactionIndex != 11) bad |= 16;
    thing.ThingClass = 2;
    k.RepressOpinions(true);                          // not a creature: flag stored, nothing else
    if (!k.RepressedOpinions || !mask[0x4d] || op.calls != 4) bad |= 32;
    thing.ThingClass = 1; thing.m_presentBits[0x4f >> 5] = 0;
    k.RepressOpinions(false);
    if (k.RepressedOpinions || op.calls != 4) bad |= 64;
    if (bad) { std::printf("007996b0_TEST FAIL\n"); return 1; }
    std::printf("007996b0_TEST PASS\n");
    return 0;
}
