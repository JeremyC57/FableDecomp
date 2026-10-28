// behaviour test for 0x004EC8C2 GFSerialiseMapBinaryIn<CObjectAugmentationParticleSet, EObjectAugmentationType> (model)
#include <cstdio>
#include <cstring>
#include <map>
#include <vector>
struct Stream {                                          // model of the chunked memory stream's 4-byte reads
    const unsigned char* Data; unsigned long Pos, Size;
    bool Read(void* out, long n) { if ((unsigned long)(Pos + n) > 0x7fffffffUL || Pos + n > Size) return false; std::memcpy(out, Data + Pos, n); Pos += n; return true; }
    long ReadSLONG() { long v = 0; Read(&v, 4); return v; }
};
struct ParticleSet { float PeriodSecs; long Effect; std::vector<long> StartPoint, EndPoint; ParticleSet() : PeriodSecs(0.0f), Effect(0) { } };
static void ReadSet(Stream& s, ParticleSet& p) { s.Read(&p.PeriodSecs, 4); p.Effect = s.ReadSLONG(); long n = s.ReadSLONG(); for (long i = 0; i < n; ++i) p.StartPoint.push_back(s.ReadSLONG()); }
static void GFSerialiseMapBinaryIn(Stream& stream, std::map<int, ParticleSet>& out)
{
    out.clear();
    long count = stream.ReadSLONG();
    for (; count > 0; --count) {
        std::pair<int, ParticleSet> value;
        value.first = (int)stream.ReadSLONG();
        ReadSet(stream, value.second);
        out.insert(std::map<int, ParticleSet>::value_type(value.first, value.second));
    }
}
static void put(std::vector<unsigned char>& b, const void* p) { const unsigned char* c = (const unsigned char*)p; b.insert(b.end(), c, c + 4); }
int main()
{
    std::vector<unsigned char> buf;
    long two = 2, k1 = 3, k2 = 1, e1 = 40, e2 = 41, one = 1, zero = 0, sp = 77; float p1 = 0.25f, p2 = 2.0f;
    put(buf, &two);
    put(buf, &k1); put(buf, &p1); put(buf, &e1); put(buf, &one); put(buf, &sp);
    put(buf, &k2); put(buf, &p2); put(buf, &e2); put(buf, &zero);
    Stream s = { &buf[0], 0, (unsigned long)buf.size() };
    std::map<int, ParticleSet> m; m[9] = ParticleSet();
    GFSerialiseMapBinaryIn(s, m);
    bool ok = m.size() == 2 && m.count(9) == 0 && m[3].PeriodSecs == 0.25f && m[3].Effect == 40 && m[3].StartPoint.size() == 1
              && m[3].StartPoint[0] == 77 && m[1].Effect == 41 && m[1].StartPoint.empty() && s.Pos == buf.size();
    std::printf(ok ? "004ec8c2_TEST PASS\n" : "004ec8c2_TEST FAIL\n");
    return ok ? 0 : 1;
}