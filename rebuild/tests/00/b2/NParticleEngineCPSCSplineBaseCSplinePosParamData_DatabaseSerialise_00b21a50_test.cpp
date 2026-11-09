// Behaviour test for 0x00B21A50 NParticleEngine::CPSCSplineBase::CSplinePosParamData::DatabaseSerialise (C++03): a saving
// context stops after MaintainSplineShape; a loading context reads "SplinePosParamSize", sizes the array to
// max(count, point count) filling new entries with CRC("") (= 0 with seed 0), stores CRC(name) of each "SplinePosParamSize_<i>"
// entry, and clears the array when every loaded CRC is zero. CCRC::Calc is the reflected table CRC-32 (seed in, no inversion).
#include <cstdio>
#include <cstring>
#include <map>
#include <string>
#include <vector>

static unsigned long g_CRCTable[256];
static void BuildTable()
{
    for (unsigned long n = 0; n < 256; ++n) {
        unsigned long c = n;
        for (int k = 0; k < 8; ++k) c = (c & 1) ? 0xEDB88320UL ^ (c >> 1) : c >> 1;
        g_CRCTable[n] = c;
    }
}
static unsigned long Calc(unsigned long seed, const void* data, unsigned long byteCount)
{
    if (byteCount != 0)
        for (unsigned long index = 0; index < byteCount; ++index)
            seed = g_CRCTable[static_cast<const unsigned char*>(data)[index] ^ (seed & 0xFF)] ^ (seed >> 8);
    return seed;
}
static unsigned long BitCRC(const std::string& s)            // independent bitwise form of the same CRC
{
    unsigned long c = 0;
    for (size_t i = 0; i < s.size(); ++i) {
        c ^= (unsigned char)s[i];
        for (int k = 0; k < 8; ++k) c = (c & 1) ? 0xEDB88320UL ^ (c >> 1) : c >> 1;
    }
    return c & 0xFFFFFFFFUL;
}
static unsigned long GetCRC(const std::string& s) { return Calc(0, s.c_str(), (unsigned long)s.size()); }

struct Context {
    int Mode;                                                // 1 / 3 save, 0 / 2 load
    bool StoredFlag; long StoredCount; std::map<std::string, std::string> Names;
    bool IsSaving() const { return Mode == 1 || Mode == 3; }
    void Transfer(const char*, bool& value, const bool&) { if (!IsSaving()) value = StoredFlag; }
    void Transfer(const char*, long& value, const long&) { value = StoredCount; }
    void Transfer(const std::string& name, std::string& value)
    {
        std::map<std::string, std::string>::const_iterator it = Names.find(name);
        value = it == Names.end() ? std::string() : it->second;
    }
};
struct Data { std::vector<int> Points; bool MaintainSplineShape; std::vector<unsigned long> SplinePosParam; };

static void DatabaseSerialise(Data& d, Context& context)
{
    context.Transfer("MaintainSplineShape", d.MaintainSplineShape, false);
    if (context.IsSaving())
        return;
    long count;
    context.Transfer("SplinePosParamSize", count, 0L);
    long points = (long)d.Points.size();
    d.SplinePosParam.resize(count > points ? count : points, GetCRC(std::string()));
    bool anySet = false;
    for (long i = 0; i < count; ++i) {
        std::string name;
        char key[64]; std::sprintf(key, "SplinePosParamSize_%ld", i);
        context.Transfer(key, name);
        d.SplinePosParam[i] = GetCRC(name);
        anySet |= d.SplinePosParam[i] != 0;
    }
    if (!anySet)
        d.SplinePosParam.clear();
}

int main()
{
    BuildTable();
    int fails = 0;
    if (GetCRC("SplinePos") != BitCRC("SplinePos") || GetCRC("") != 0) { std::printf("FAIL crc\n"); ++fails; }
    // saving: flag untouched by load path, array untouched
    { Data d; d.Points.resize(4); d.MaintainSplineShape = true; d.SplinePosParam.assign(2, 7UL);
      Context c; c.Mode = 3; c.StoredFlag = false; c.StoredCount = 9;
      DatabaseSerialise(d, c);
      if (!d.MaintainSplineShape || d.SplinePosParam.size() != 2 || d.SplinePosParam[0] != 7) { std::printf("FAIL save\n"); ++fails; } }
    // loading, count < points: size = points, tail = CRC("") = 0
    { Data d; d.Points.resize(5); d.MaintainSplineShape = false;
      Context c; c.Mode = 0; c.StoredFlag = true; c.StoredCount = 3;
      c.Names["SplinePosParamSize_0"] = "Start"; c.Names["SplinePosParamSize_2"] = "End";
      DatabaseSerialise(d, c);
      if (!d.MaintainSplineShape || d.SplinePosParam.size() != 5 || d.SplinePosParam[0] != BitCRC("Start") ||
          d.SplinePosParam[1] != 0 || d.SplinePosParam[2] != BitCRC("End") || d.SplinePosParam[3] != 0 || d.SplinePosParam[4] != 0)
      { std::printf("FAIL load small\n"); ++fails; } }
    // loading, count > points: size = count
    { Data d; d.Points.resize(2); d.MaintainSplineShape = false;
      Context c; c.Mode = 2; c.StoredFlag = false; c.StoredCount = 4; c.Names["SplinePosParamSize_3"] = "Mid";
      DatabaseSerialise(d, c);
      if (d.SplinePosParam.size() != 4 || d.SplinePosParam[3] != BitCRC("Mid") || d.SplinePosParam[0] != 0)
      { std::printf("FAIL load large\n"); ++fails; } }
    // loading, every name empty: cleared
    { Data d; d.Points.resize(3); d.SplinePosParam.assign(6, 5UL);
      Context c; c.Mode = 0; c.StoredFlag = false; c.StoredCount = 2;
      DatabaseSerialise(d, c);
      if (!d.SplinePosParam.empty()) { std::printf("FAIL clear\n"); ++fails; } }
    // loading, count 0: cleared even with points
    { Data d; d.Points.resize(3);
      Context c; c.Mode = 0; c.StoredFlag = false; c.StoredCount = 0;
      DatabaseSerialise(d, c);
      if (!d.SplinePosParam.empty()) { std::printf("FAIL zero\n"); ++fails; } }
    if (fails) return 1;
    std::printf("00b21a50_TEST PASS\n");
    return 0;
}