// Behaviour test for CThingManager::SaveEntitiesBinary (0x00522160): nothing is written for no things; otherwise the
// stream holds the section count, then per section (in sorted order) its name and a length-prefixed section stream of
// length-prefixed (type name + SaveBinary bytes) records for the things in it, closed by a 0 ULONG; names receives the
// sorted section names and the version is stored -- through a copy of the body over byte-vector streams.
#include <cstdio>
#include <set>
#include <string>
#include <vector>

struct Stream {
    std::vector<unsigned char> bytes;
    void WriteULONG(unsigned int value)
    {
        for (int i = 0; i < 4; ++i)
            bytes.push_back((unsigned char)(value >> (8 * i)));
    }
    void WriteNullTerminatedString(const std::string& text)
    {
        for (unsigned int i = 0; i < text.size(); ++i)
            bytes.push_back((unsigned char)text[i]);
        bytes.push_back(0);
    }
    void Write(const unsigned char* data, unsigned int length)
    {
        for (unsigned int i = 0; i < length; ++i)
            bytes.push_back(data[i]);
    }
    void Write(const Stream& other)
    {
        WriteULONG((unsigned int)other.bytes.size());
        if (!other.bytes.empty())
            Write(&other.bytes[0], (unsigned int)other.bytes.size());
    }
};

struct Thing {
    std::string SerialisationSectionName;
    std::string TypeName;
    unsigned char Payload;
    void SaveBinary(Stream& stream) { stream.bytes.push_back(Payload); }
};

struct Manager {
    int SerialiseVersionNumber;
    Manager() : SerialiseVersionNumber(-1) { }
    void SaveEntitiesBinary(Stream& stream, int version, const std::vector<Thing*>& things,
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
        stream.WriteULONG((unsigned int)sections.size());
        SerialiseVersionNumber = version;
        for (std::set<std::string>::iterator s = sections.begin(); s != sections.end(); ++s) {
            std::string section(*s);
            stream.WriteNullTerminatedString(section);
            Stream sectionStream;
            for (unsigned int i = 0; i < things.size(); ++i) {
                Thing* thing = things[i];
                if (thing->SerialisationSectionName == section) {
                    Stream thingStream;
                    thingStream.WriteNullTerminatedString(thing->TypeName);
                    thing->SaveBinary(thingStream);
                    sectionStream.WriteULONG((unsigned int)thingStream.bytes.size());
                    sectionStream.Write(&thingStream.bytes[0], (unsigned int)thingStream.bytes.size());
                }
            }
            sectionStream.WriteULONG(0);
            stream.Write(sectionStream);
        }
    }
};

static unsigned int ReadULONG(const std::vector<unsigned char>& b, unsigned int& pos)
{
    unsigned int v = b[pos] | (b[pos + 1] << 8) | (b[pos + 2] << 16) | ((unsigned int)b[pos + 3] << 24);
    pos += 4;
    return v;
}
static std::string ReadString(const std::vector<unsigned char>& b, unsigned int& pos)
{
    std::string s;
    while (b[pos] != 0)
        s += (char)b[pos++];
    ++pos;
    return s;
}

int main()
{
    Manager manager;
    Stream empty;
    std::vector<Thing*> none;
    std::vector<std::string> names;
    names.push_back("stale");
    manager.SaveEntitiesBinary(empty, 5, none, names);
    if (!empty.bytes.empty() || names.size() != 1 || manager.SerialiseVersionNumber != -1) {
        std::printf("00522160_TEST FAIL\n");
        return 1;
    }
    Thing a = { "Town", "Villager", 0x11 };
    Thing b = { "Forest", "Wolf", 0x22 };
    Thing c = { "Town", "Guard", 0x33 };
    std::vector<Thing*> things;
    things.push_back(&a);
    things.push_back(&b);
    things.push_back(&c);
    Stream out;
    manager.SaveEntitiesBinary(out, 7, things, names);
    if (manager.SerialiseVersionNumber != 7 || names.size() != 2 || names[0] != "Forest" || names[1] != "Town") {
        std::printf("00522160_TEST FAIL\n");
        return 1;
    }
    unsigned int pos = 0;
    bool ok = ReadULONG(out.bytes, pos) == 2;
    ok = ok && ReadString(out.bytes, pos) == "Forest";
    ok = ok && ReadULONG(out.bytes, pos) == 4 + 6 + 4;               // one record (4 + "Wolf\0" + 1) and the 0
    ok = ok && ReadULONG(out.bytes, pos) == 6 && ReadString(out.bytes, pos) == "Wolf" && out.bytes[pos++] == 0x22;
    ok = ok && ReadULONG(out.bytes, pos) == 0;
    ok = ok && ReadString(out.bytes, pos) == "Town";
    ok = ok && ReadULONG(out.bytes, pos) == (4 + 10) + (4 + 7) + 4;
    ok = ok && ReadULONG(out.bytes, pos) == 10 && ReadString(out.bytes, pos) == "Villager" && out.bytes[pos++] == 0x11;
    ok = ok && ReadULONG(out.bytes, pos) == 7 && ReadString(out.bytes, pos) == "Guard" && out.bytes[pos++] == 0x33;
    ok = ok && ReadULONG(out.bytes, pos) == 0 && pos == out.bytes.size();
    if (!ok) {
        std::printf("00522160_TEST FAIL\n");
        return 1;
    }
    std::printf("00522160_TEST PASS\n");
    return 0;
}