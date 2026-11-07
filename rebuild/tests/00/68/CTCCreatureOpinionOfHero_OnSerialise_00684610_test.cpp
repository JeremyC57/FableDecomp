// Behaviour test for CTCCreatureOpinionOfHero::OnSerialise (0x00684610): the pending deeds persist as "count,deed,deed..." and
// the known-deed lists as "(index:count:,deed,deed)" groups for every non-empty index of the 92; loading parses them back
// (count, then a separator symbol before each integer). A model of the string format with a tiny symbol/integer reader.
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <list>

static void AppendLong(char* out, long v) { char b[16]; std::sprintf(b, "%ld", v); std::strcat(out, b); }

static void SavePending(const std::list<long>& deeds, char* out)
{
    out[0] = 0; AppendLong(out, (long)deeds.size());
    for (std::list<long>::const_iterator it = deeds.begin(); it != deeds.end(); ++it) { std::strcat(out, ","); AppendLong(out, *it); }
}

static void SaveKnown(const std::list<long>* known, char* out)
{
    out[0] = 0;
    for (long i = 0; i < 92; ++i) {
        if (known[i].empty()) continue;
        std::strcat(out, "("); AppendLong(out, i); std::strcat(out, ":");
        AppendLong(out, (long)known[i].size()); std::strcat(out, ":");
        for (std::list<long>::const_iterator it = known[i].begin(); it != known[i].end(); ++it) { std::strcat(out, ","); AppendLong(out, *it); }
        std::strcat(out, ")");
    }
}

struct Parser {
    const char* P;
    bool AtEnd() const { return *P == 0; }
    long ReadInteger() { char* e; long v = std::strtol(P, &e, 10); P = e; return v; }
    char ReadSymbol() { return *P ? *P++ : 0; }
};

static void LoadPending(const char* text, std::list<long>& deeds)
{
    Parser p = { text };
    long count = p.ReadInteger();
    for (long i = 0; i < count; ++i) { p.ReadSymbol(); deeds.push_back(p.ReadInteger()); }
}

static void LoadKnown(const char* text, std::list<long>* known)
{
    Parser p = { text };
    while (!p.AtEnd() && p.ReadSymbol() == '(') {
        long index = p.ReadInteger(); p.ReadSymbol();
        long count = p.ReadInteger(); p.ReadSymbol();
        for (long i = 0; i < count; ++i) { p.ReadSymbol(); known[index].push_back(p.ReadInteger()); }
        p.ReadSymbol();
    }
}

int main()
{
    std::list<long> pending; pending.push_back(7); pending.push_back(12);
    char buf[256]; SavePending(pending, buf);
    bool ok = std::strcmp(buf, "2,7,12") == 0;
    std::list<long> back; LoadPending(buf, back);
    ok = ok && back.size() == 2 && back.front() == 7 && back.back() == 12;
    std::list<long> known[92]; known[3].push_back(5); known[40].push_back(1); known[40].push_back(9);
    SaveKnown(known, buf);
    ok = ok && std::strcmp(buf, "(3:1:,5)(40:2:,1,9)") == 0;
    std::list<long> known2[92]; LoadKnown(buf, known2);
    ok = ok && known2[3].size() == 1 && known2[3].front() == 5 && known2[40].size() == 2 && known2[40].back() == 9 && known2[0].empty();
    std::printf(ok ? "00684610_TEST PASS\n" : "00684610_TEST FAIL\n");
    return ok ? 0 : 1;
}