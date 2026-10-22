#include <cstdio>
#include <cstring>
// Behaviour test for 00f33b90: CPersistContext::TransferVectorOfSubComponents<CFireHeartPatternDef> mode dispatch on a model context.
struct Ctx;
struct Def {
    int hits;
    virtual void Transfer(Ctx& context);
};
struct Vec {
    Def* _M_start; Def* _M_finish; Def* _M_end_of_storage;
    long size() const { return (long)(_M_finish - _M_start); }
    Def& operator[](long index) { return _M_start[index]; }
    void resize(long count) { _M_finish = _M_start + count; }
};
struct Ctx {
    long Mode;
    long readValue;            // what the load stream yields
    long written;              // what the save stream received
    int crcChecks;
    int textLoads;
    int transfers;
    long ReadLong() { return readValue; }
    void WriteLong(long v) { written = v; }
    void CheckCRC(const char*) { ++crcChecks; }
    void LoadText(const char*, Vec&) { ++textLoads; }
    void TransferVectorOfSubComponents(const char* name, Vec& value)
    {
        CheckCRC(name);
        switch (Mode) {
        case 4: { long count = value.size(); for (long i = 0; i < count; ++i) value[i].Transfer(*this); break; }
        case 3: { long count = value.size(); WriteLong(count); for (long i = 0; i < count; ++i) value[i].Transfer(*this); break; }
        case 2: { long count = ReadLong(); value.resize(count); for (long i = 0; i < count; ++i) value[i].Transfer(*this); break; }
        case 0: { LoadText(name, value); break; }
        }
    }
};
void Def::Transfer(Ctx& context) { ++hits; ++context.transfers; }
int main()
{
    Def defs[6];
    for (int i = 0; i < 6; ++i) defs[i].hits = 0;
    Vec v; v._M_start = defs; v._M_finish = defs + 3; v._M_end_of_storage = defs + 6;
    Ctx c; std::memset(&c, 0, sizeof(c));
    int ok = 1;
    c.Mode = 4; c.TransferVectorOfSubComponents("K", v);
    if (c.transfers != 3 || c.crcChecks != 1) ok = 0;
    c.Mode = 3; c.TransferVectorOfSubComponents("K", v);
    if (c.written != 3 || c.transfers != 6) ok = 0;
    c.Mode = 2; c.readValue = 5; c.TransferVectorOfSubComponents("K", v);
    if (v.size() != 5 || c.transfers != 11 || defs[4].hits != 1 || defs[0].hits != 3) ok = 0;
    c.Mode = 0; c.TransferVectorOfSubComponents("K", v);
    if (c.textLoads != 1 || c.transfers != 11 || c.crcChecks != 4) ok = 0;
    if (!ok) { std::printf("00f33b90_TEST FAIL\n"); return 1; }
    std::printf("00f33b90_TEST PASS\n");
    return 0;
}
