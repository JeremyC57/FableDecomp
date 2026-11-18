#include <cassert>
#include <cstdio>
#include <sstream>
#include <string>

typedef std::string CCharString;

class CThingPhysical
{
public:
    void Save(CCharString& script) { script += "base\r\n"; }
};

class CThingManager
{
public:
    mutable unsigned remaps;
    unsigned map_number;
    bool level_less;
    CThingManager() : remaps(0), map_number(0), level_less(false) { }
    unsigned RemapUIDForSerialisation(bool serialise,bool level,unsigned map,unsigned uid) const
    {
        assert(serialise && map==map_number && level==level_less);
        ++remaps;
        return 1000+map*10+uid;
    }
};

class CThingTrackNode : public CThingPhysical
{
public:
    const CThingTrackNode* PreviousNode;
    const CThingTrackNode* NextNode;
    unsigned id;
    bool IsStart, IsEnd;
    CCharString TrackName;
    CThingManager manager;
    CThingTrackNode() : PreviousNode(0), NextNode(0), id(0), IsStart(false), IsEnd(false) { }
    unsigned GetUniqueID() const { return id; }
    unsigned GetCurrentMapNumber() const { return manager.map_number; }
    bool IsLevelLess() const { return manager.level_less; }
    const CThingManager* GetPCurrentThingManager() const { return &manager; }
    void Save(CCharString& script);
};

CCharString GFUHUGEToCharString(unsigned uid)
{
    std::ostringstream stream;
    stream << uid;
    return stream.str();
}

// The selected implementation is reproduced verbatim for this standalone fixture.
void CThingTrackNode::Save(CCharString& script)
{
    CThingPhysical::Save(script);
    script += "LinkedToUID1 " + GFUHUGEToCharString(PreviousNode ? GetPCurrentThingManager()->RemapUIDForSerialisation(true, IsLevelLess(), GetCurrentMapNumber(), PreviousNode->GetUniqueID()) : 0) + ";\r\n";
    script += "LinkedToUID2 " + GFUHUGEToCharString(NextNode ? GetPCurrentThingManager()->RemapUIDForSerialisation(true, IsLevelLess(), GetCurrentMapNumber(), NextNode->GetUniqueID()) : 0) + ";\r\n";
    script += CCharString("") + "Start " + (IsStart ? "TRUE" : "FALSE") + ";\r\n";
    script += CCharString("") + "End " + (IsEnd ? "TRUE" : "FALSE") + ";\r\n";
    script += "ScriptName " + (TrackName == "" ? CCharString("NULL") : TrackName) + ";\r\n";
}

int main()
{
    CThingTrackNode prev, next;
    prev.id = 7;
    next.id = 12;
    for (unsigned flags = 0; flags < 64; ++flags) {
        CThingTrackNode node;
        node.PreviousNode = flags & 1 ? &prev : 0;
        node.NextNode = flags & 2 ? &next : 0;
        node.IsStart = (flags & 4) != 0;
        node.IsEnd = (flags & 8) != 0;
        node.TrackName = flags & 16 ? "Bowerstone track" : "";
        node.manager.level_less = (flags & 32) != 0;
        node.manager.map_number = 13;
        std::string script("existing;\r\n");
        node.Save(script);
        std::ostringstream expected;
        expected << "existing;\r\nbase\r\nLinkedToUID1 " << (flags & 1 ? 1137 : 0)
                 << ";\r\nLinkedToUID2 " << (flags & 2 ? 1142 : 0)
                 << ";\r\nStart " << (flags & 4 ? "TRUE" : "FALSE")
                 << ";\r\nEnd " << (flags & 8 ? "TRUE" : "FALSE")
                 << ";\r\nScriptName " << (flags & 16 ? "Bowerstone track" : "NULL") << ";\r\n";
        assert(script == expected.str());
        assert(node.manager.remaps == unsigned((flags & 1 ? 1 : 0) + (flags & 2 ? 1 : 0)));
    }
    std::puts("00837570_TEST PASS");
    return 0;
}
