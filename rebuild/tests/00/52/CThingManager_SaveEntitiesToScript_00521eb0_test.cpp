// Behaviour test for CThingManager::SaveEntitiesToScript (0x00521EB0): nothing is written for no things; otherwise each
// section (in sorted order) is one "XXXSectionStart <name>;" ... "XXXSectionEnd;" block holding a "NewThing <type>;"
// <Save text> "EndThing;" record per thing in it; names receives the sorted section names and the version is stored
// -- through a copy of the body over std::string.
#include <cstdio>
#include <set>
#include <string>
#include <vector>

struct Thing {
    std::string SerialisationSectionName;
    std::string TypeName;
    std::string Body;
    void Save(std::string& script) { script += Body; }
};

struct Manager {
    int SerialiseVersionNumber;
    Manager() : SerialiseVersionNumber(-1) { }
    void SaveEntitiesToScript(std::string& script, int version, const std::vector<Thing*>& things,
                              std::vector<std::string>& names)
    {
        if (things.empty())
            return;
        SerialiseVersionNumber = version;
        std::set<std::string> sections;
        for (unsigned int i = 0; i < things.size(); ++i)
            sections.insert(things[i]->SerialisationSectionName);
        names.clear();
        for (std::set<std::string>::const_iterator n = sections.begin(); n != sections.end(); ++n)
            names.push_back(*n);                       // the range insert at the end
        SerialiseVersionNumber = version;
        for (std::set<std::string>::iterator s = sections.begin(); s != sections.end(); ++s) {
            std::string section(*s);
            script += "\r\nXXXSectionStart " + section + ";\r\n";
            for (unsigned int i = 0; i < things.size(); ++i) {
                Thing* thing = things[i];
                if (thing->SerialisationSectionName == section) {
                    std::string thingScript;
                    thingScript.reserve(0x4000);
                    thingScript += "\r\n";
                    thingScript += "NewThing ";
                    thingScript += thing->TypeName;
                    thingScript += ";\r\n";
                    thing->Save(thingScript);
                    thingScript += "EndThing;\r\n";
                    script += thingScript;
                }
            }
            script += "\r\nXXXSectionEnd;\r\n\r\n";
        }
    }
};

int main()
{
    Manager manager;
    std::string empty;
    std::vector<Thing*> none;
    std::vector<std::string> names;
    names.push_back("stale");
    manager.SaveEntitiesToScript(empty, 5, none, names);
    if (!empty.empty() || names.size() != 1 || manager.SerialiseVersionNumber != -1) {
        std::printf("00521eb0_TEST FAIL\n");
        return 1;
    }
    Thing a = { "Town", "Villager", "Health 1;\r\n" };
    Thing b = { "Forest", "Wolf", "" };
    Thing c = { "Town", "Guard", "Rank 2;\r\n" };
    std::vector<Thing*> things;
    things.push_back(&a);
    things.push_back(&b);
    things.push_back(&c);
    std::string script("Header;");
    manager.SaveEntitiesToScript(script, 7, things, names);
    std::string expected = "Header;"
        "\r\nXXXSectionStart Forest;\r\n"
        "\r\nNewThing Wolf;\r\nEndThing;\r\n"
        "\r\nXXXSectionEnd;\r\n\r\n"
        "\r\nXXXSectionStart Town;\r\n"
        "\r\nNewThing Villager;\r\nHealth 1;\r\nEndThing;\r\n"
        "\r\nNewThing Guard;\r\nRank 2;\r\nEndThing;\r\n"
        "\r\nXXXSectionEnd;\r\n\r\n";
    if (script != expected || manager.SerialiseVersionNumber != 7 || names.size() != 2 || names[0] != "Forest" ||
        names[1] != "Town") {
        std::printf("00521eb0_TEST FAIL\n");
        return 1;
    }
    std::printf("00521eb0_TEST PASS\n");
    return 0;
}