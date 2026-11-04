// Behaviour test for 0x00477CD2 (parked row): the row's logic over a flat model.
#include <cstdio>

#include <vector>
struct Stream { std::vector<long> Data; size_t Pos; };
struct Def { long Value; void Transfer(Stream& s, int mode) { if (mode == 2) Value = s.Data[s.Pos++]; else s.Data.push_back(Value); } };
static void TransferVector(std::vector<Def>& v, Stream& s, int mode) {
    if (mode == 2) { long count = s.Data[s.Pos++]; v.resize(count, Def()); for (long i = 0; i < count; ++i) v[i].Transfer(s, mode); }
    else { long count = (long)v.size(); s.Data.push_back(count); for (long i = 0; i < count; ++i) v[i].Transfer(s, mode); }
}
int main() {
    std::vector<Def> out(3); out[0].Value = 5; out[1].Value = 6; out[2].Value = 7;
    Stream s; s.Pos = 0; TransferVector(out, s, 1);
    std::vector<Def> in; TransferVector(in, s, 2);
    bool ok = s.Data.size() == 4 && in.size() == 3 && in[2].Value == 7 && in[0].Value == 5;
    std::printf(ok ? "00477cd2_TEST PASS\n" : "00477cd2_TEST FAIL\n");
    return ok ? 0 : 1;
}
