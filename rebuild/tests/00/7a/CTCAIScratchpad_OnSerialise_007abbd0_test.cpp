// Behaviour test for CTCAIScratchpad::OnSerialise (0x007ABBD0): only the hero's scratchpad persists the statics; each
// category -> phrases table is saved as "<Table>_num_vectors" = count, "<Table><i>" = key and "_<Table><i>" = the phrase vector,
// in map (sorted key) order, and loaded back by reading the count and then each key and vector into table[key]. A model with a
// string-keyed store standing in for CPersistContext.
#include <cstdio>
#include <cstring>
#include <map>
#include <vector>

struct Str {
    char Text[64];
    Str() { Text[0] = 0; }
    Str(const char* text) { std::strcpy(Text, text); }
    bool operator<(const Str& other) const { return std::strcmp(Text, other.Text) < 0; }
    bool operator==(const Str& other) const { return std::strcmp(Text, other.Text) == 0; }
};
static Str Cat(const Str& a, const Str& b) { Str r(a.Text); std::strcat(r.Text, b.Text); return r; }
static Str Num(long i) { Str r; std::sprintf(r.Text, "%ld", i); return r; }

typedef std::map<Str, std::vector<Str> > Table;
struct Store {
    bool Saving;
    std::map<Str, long> Longs;
    std::map<Str, Str> Strings;
    std::map<Str, std::vector<Str> > Vectors;
    void Transfer(const Str& name, long& v) { if (Saving) Longs[name] = v; else v = Longs[name]; }
    void Transfer(const Str& name, Str& v) { if (Saving) Strings[name] = v; else v = Strings[name]; }
    void TransferVector(const Str& name, std::vector<Str>& v) { if (Saving) Vectors[name] = v; else v = Vectors[name]; }
};

static void SerialiseTable(Store& context, const char* tableName, Table& table)
{
    Str base(tableName);
    long num;
    if (context.Saving) {
        num = (long)table.size();
        context.Transfer(Cat(base, "_num_vectors"), num);
        long i = 0;
        for (Table::iterator it = table.begin(); it != table.end(); ++it, ++i) {
            Str key = (*it).first;
            context.Transfer(Cat(base, Num(i)), key);
            context.TransferVector(Cat(Cat("_", base), Num(i)), (*it).second);
        }
    } else {
        context.Transfer(Cat(base, "_num_vectors"), num);
        for (long i = 0; i < num; ++i) {
            Str key;
            context.Transfer(Cat(base, Num(i)), key);
            context.TransferVector(Cat(Cat("_", base), Num(i)), table[key]);
        }
    }
}

static void OnSerialise(Store& context, bool isHero, Table& gossip, Table& villages)
{
    if (!isHero)
        return;
    SerialiseTable(context, "Gossip", gossip);
    SerialiseTable(context, "GossipVillages", villages);
}

int main()
{
    Table gossip, villages;
    gossip[Str("WEATHER")].push_back(Str("RAIN"));
    gossip[Str("HERO")].push_back(Str("TALL"));
    gossip[Str("HERO")].push_back(Str("STRONG"));
    villages[Str("OAKVALE")].push_back(Str("QUIET"));
    Store store; store.Saving = true;
    OnSerialise(store, false, gossip, villages);
    bool ok = store.Longs.empty();
    OnSerialise(store, true, gossip, villages);
    ok = ok && store.Longs[Str("Gossip_num_vectors")] == 2 && store.Longs[Str("GossipVillages_num_vectors")] == 1
        && store.Strings[Str("Gossip0")] == Str("HERO") && store.Strings[Str("Gossip1")] == Str("WEATHER")
        && store.Vectors[Str("_Gossip0")].size() == 2 && store.Vectors[Str("_GossipVillages0")][0] == Str("QUIET");
    Table gossip2, villages2;
    store.Saving = false;
    OnSerialise(store, true, gossip2, villages2);
    ok = ok && gossip2.size() == 2 && gossip2[Str("HERO")].size() == 2 && gossip2[Str("HERO")][1] == Str("STRONG")
        && gossip2[Str("WEATHER")][0] == Str("RAIN") && villages2[Str("OAKVALE")][0] == Str("QUIET");
    std::printf(ok ? "007abbd0_TEST PASS\n" : "007abbd0_TEST FAIL\n");
    return ok ? 0 : 1;
}