#include <cstdio>
#ifdef _MSC_VER
typedef unsigned __int64 UID64;
#else
typedef unsigned long long UID64;
#endif

struct CThing {
    UID64 UniqueID;
    bool Dead;
    UID64 GetUID() const { return UniqueID; }
};

struct CBaseIntelligentPointer {
    CThing* PItem;
    CThing* GetPItem() const { return PItem; }
};

struct CGroupMemberData {
    CBaseIntelligentPointer PMember;
    bool IsActive;
};

struct SGroupMemberNode {
    UID64 Key;
    CGroupMemberData Value;
};

struct CGroupMemberMap {                    // a linear stand-in for the STLport map: end() is the sentinel slot
    SGroupMemberNode Nodes[5];
    int Count;
    SGroupMemberNode* end() { return &Nodes[4]; }
    SGroupMemberNode* find(const UID64& key)
    {
        for (int i = 0; i < Count; ++i)
            if (Nodes[i].Key == key) return &Nodes[i];
        return end();
    }
    void add(CThing* t, bool active)
    {
        Nodes[Count].Key = t->UniqueID; Nodes[Count].Value.PMember.PItem = t; Nodes[Count].Value.IsActive = active; ++Count;
    }
};

static long g_frame = 0;
long GetWorldFrame() { return g_frame; }

struct CTCCreatureGroup {
    CGroupMemberMap Members;
    int RemoveCalls, DeactivateCalls;
    bool BaseResult;
    CTCCreatureGroup() : RemoveCalls(0), DeactivateCalls(0), BaseResult(true) { Members.Count = 0; }
    bool IsActiveMember(const CThing* member)
    {
        if (member != 0 && !member->Dead) {
            SGroupMemberNode* it = Members.find(member->GetUID());
            if (it != Members.end())
                return it->Value.IsActive;
        }
        return false;
    }
    bool RemoveMember(const CThing*) { ++RemoveCalls; return BaseResult; }
    bool DeactivateMember(const CThing*) { ++DeactivateCalls; return BaseResult; }
};

struct CTCCreatureGroupTag : CTCCreatureGroup {
    UID64 TaggerUID;
    unsigned long TaggedFrame;
    int ChooseCalls;
    CTCCreatureGroupTag() : TaggerUID(0), TaggedFrame(0), ChooseCalls(0) {}
    bool ChooseNewTagger() { ++ChooseCalls; return true; }
    bool SetTagger(const CThing* tagger)
    {
        if (IsActiveMember(tagger)) {
            TaggerUID = tagger->UniqueID;
            TaggedFrame = GetWorldFrame();
        }
        return false;
    }
    bool RemoveMember(const CThing* member)
    {
        if (CTCCreatureGroup::RemoveMember(member)) {
            if (TaggerUID == member->UniqueID)
                ChooseNewTagger();
            return true;
        }
        return false;
    }
    bool DeactivateMember(const CThing* member)
    {
        if (CTCCreatureGroup::DeactivateMember(member)) {
            if (TaggerUID == member->UniqueID)
                ChooseNewTagger();
            return true;
        }
        return false;
    }
    CThing* GetTagger()
    {
        if (TaggerUID > 0) {
            SGroupMemberNode* it = Members.find(TaggerUID);
            if (it != Members.end() && it->Value.IsActive)
                return it->Value.PMember.GetPItem();
        }
        return 0;
    }
};

#define ADDR "008137b0"
#define CHECK(c) if (!(c)) { std::printf("%s_TEST FAIL\n", ADDR); return 1; }

static UID64 MakeUID(unsigned long hi, unsigned long lo) { return ((UID64)hi << 32) | lo; }

int main()
{
    CTCCreatureGroupTag g;
    CThing a = { MakeUID(2, 7), false }, b = { 8, false };
    g.Members.add(&a, true); g.Members.add(&b, false);
    g_frame = 1234;
    CHECK(g.SetTagger(&b) == false); CHECK(g.TaggerUID == 0 && g.TaggedFrame == 0);
    CHECK(g.SetTagger(&a) == false); CHECK(g.TaggerUID == a.UniqueID && g.TaggedFrame == 1234);
    CHECK(g.SetTagger(0) == false); CHECK(g.TaggerUID == a.UniqueID);
    std::printf("008137b0_TEST PASS\n");
    return 0;
}