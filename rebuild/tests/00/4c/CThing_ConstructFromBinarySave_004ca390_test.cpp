// Behaviour test for CThing::ConstructFromBinarySave (0x004CA390): a host model of the same control flow over a little binary save
// (identity unless only the hero state loads, the length-prefixed game-state block, the map number, the def / editor / map-who
// components, then component records up to "END", the script name / data and the two persistence flags).
#include <cstdio>
#include <cstring>

struct Stream {                                   // CMemoryDataInputStream: StreamPos / StreamSize over a byte buffer
    const unsigned char* data; unsigned long pos, size;
    Stream(const unsigned char* d, unsigned long n) : data(d), pos(0), size(n) {}
    long ReadSLONG() { long v = 0; if (pos + 4 <= size) { std::memcpy(&v, data + pos, 4); pos += 4; } return v; }
    bool ReadEBOOL() { unsigned char c = 0; if (pos + 1 <= size) c = data[pos++]; return c != 0; }
    const unsigned char* ReadConstData(long n) {  // the inline body: no data for n <= 0
        if (n <= 0) return 0;
        if ((long)(pos + n) < 0) return 0;
        const unsigned char* r = data + pos; pos += n; return r;
    }
    bool IsEOF() const { return pos == size; }
    void ReadString(char* out) { long n = ReadSLONG(); std::memcpy(out, data + pos, n); out[n] = 0; pos += n; }
};

struct TC { char name[16]; long serialised; long bytes; };

struct Thing {
    unsigned long ThingTypeFlags; bool hero_world; bool editor;
    bool CreatedBySerialisation, GameStatePersisted, LevelPersistent, AddingTCsPermitted, info_used;
    long CurrentMapNumber, state_bytes, created_from_def; bool has_def;
    char section[16], script_name[16], script_data[16];
    TC comps[8]; long ncomps;
    TC* AddTC(const char* name) { TC* t = &comps[ncomps++]; std::strcpy(t->name, name); t->serialised = 0; t->bytes = 0; return t; }
    void Construct(Stream& s) {
        bool hero_state_only = false;
        if ((ThingTypeFlags & 2) && hero_world) hero_state_only = true;
        else { CreatedBySerialisation = true; std::strcpy(section, "NULL"); info_used = true; }
        long state_length = s.ReadSLONG();
        s.ReadConstData(state_length); state_bytes = state_length;
        if (!hero_state_only) {
            CurrentMapNumber = (short)s.ReadSLONG();
            if (has_def) ++created_from_def;
            if (editor) AddTC("CTCEditor");
            AddTC("CTCMapwho");
        }
        while (!s.IsEOF()) {
            char class_name[16]; s.ReadString(class_name);
            if (std::strcmp(class_name, "END") == 0) break;
            bool creatable = s.ReadEBOOL();
            TC* tc = 0;
            if (creatable) tc = AddTC(class_name);
            else for (long i = 0; i < ncomps; ++i) if (std::strcmp(comps[i].name, class_name) == 0) { tc = &comps[i]; break; }
            long length = s.ReadSLONG();
            s.ReadConstData(length);
            if (tc && length) { AddingTCsPermitted = false; ++tc->serialised; tc->bytes = length; AddingTCsPermitted = true; }
        }
        s.ReadString(script_name); s.ReadString(script_data);
        bool g = s.ReadEBOOL(); bool l = s.ReadEBOOL();
        GameStatePersisted = g; LevelPersistent = l;
    }
};

static unsigned char buf[256]; static unsigned long n;
static void L(long v) { std::memcpy(buf + n, &v, 4); n += 4; }
static void B(bool b) { buf[n++] = b ? 1 : 0; }
static void Str(const char* t) { long k = (long)std::strlen(t); L(k); std::memcpy(buf + n, t, k); n += k; }
static void Build(bool hero) {
    n = 0; L(3); buf[n++] = 1; buf[n++] = 2; buf[n++] = 3;   // game-state block
    if (!hero) L(7);                                         // map number
    Str("CTCMapwho"); B(false); L(2); buf[n++] = 9; buf[n++] = 9;   // existing component, serialised
    Str("CTCHealth"); B(true); L(1); buf[n++] = 5;                 // creatable component
    Str("CTCGone"); B(false); L(0);                                // unknown, empty
    Str("END");
    Str("Scr"); Str("Dat"); B(true); B(false);
}

int main()
{
    Thing t; std::memset(&t, 0, sizeof(t)); t.editor = true; t.has_def = true; t.AddingTCsPermitted = true;
    Build(false); Stream s(buf, n); t.Construct(s);
    if (!t.CreatedBySerialisation || !t.info_used || std::strcmp(t.section, "NULL") != 0 || t.CurrentMapNumber != 7 || t.state_bytes != 3
        || t.created_from_def != 1 || t.ncomps != 3 || std::strcmp(t.comps[0].name, "CTCEditor") != 0 || t.comps[1].serialised != 1
        || t.comps[1].bytes != 2 || std::strcmp(t.comps[2].name, "CTCHealth") != 0 || t.comps[2].serialised != 1
        || std::strcmp(t.script_name, "Scr") != 0 || std::strcmp(t.script_data, "Dat") != 0 || !t.GameStatePersisted || t.LevelPersistent
        || !t.AddingTCsPermitted) { std::printf("004ca390_TEST FAIL\n"); return 1; }
    Thing h; std::memset(&h, 0, sizeof(h)); h.ThingTypeFlags = 2; h.hero_world = true; h.AddTC("CTCMapwho");
    Build(true); Stream s2(buf, n); h.Construct(s2);
    if (h.CreatedBySerialisation || h.info_used || h.CurrentMapNumber != 0 || h.ncomps != 2 || h.comps[0].serialised != 1
        || std::strcmp(h.script_data, "Dat") != 0) { std::printf("004ca390_TEST FAIL\n"); return 1; }
    std::printf("004ca390_TEST PASS\n");
    return 0;
}