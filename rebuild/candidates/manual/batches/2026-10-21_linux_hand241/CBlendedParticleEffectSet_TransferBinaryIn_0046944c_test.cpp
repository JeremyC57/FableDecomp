#include <cstdio>
#include <cstring>
// Behaviour copy of CBlendedParticleEffectSet::TransferBinaryIn (0x0046944c): read index, index, value, value from a chunked stream.
struct Stream {
    unsigned long pos; const unsigned char* src; long remaining; const unsigned char* all; long overflowCalls;
    void ReadWithSrcChunkOverflow(unsigned char* dst, long size) { std::memcpy(dst, all + pos, size); pos += size; src = all + pos; remaining = 0; ++overflowCalls; }
    void Read(void* dst, long size) {
        long long np = (long long)(pos + size);
        if (np < 0 || np > 0x7fffffff) return;
        if (remaining >= size) { std::memcpy(dst, src, size); src += size; remaining -= size; pos += size; }
        else ReadWithSrcChunkOverflow((unsigned char*)dst, size);
    }
    int ReadLong() { int v; Read(&v, 4); return v; }
    float ReadFloat() { float v; Read(&v, 4); return v; }
};
struct Set { int Particle1Index; float Particle1Value; int Particle2Index; float Particle2Value;
    void TransferBinaryIn(Stream& in) { Particle1Index = in.ReadLong(); Particle2Index = in.ReadLong(); Particle1Value = in.ReadFloat(); Particle2Value = in.ReadFloat(); } };
int main()
{
    unsigned char buf[16]; int a = 7, b = -3; float c = 0.25f, d = 2.5f;
    std::memcpy(buf, &a, 4); std::memcpy(buf + 4, &b, 4); std::memcpy(buf + 8, &c, 4); std::memcpy(buf + 12, &d, 4);
    Stream s = { 0, buf, 8, buf, 0 };             // the first chunk holds two longs; the floats overflow
    Set set = { 0, 0, 0, 0 };
    set.TransferBinaryIn(s);

    bool ok = set.Particle1Index == 7 && set.Particle2Index == -3 && set.Particle1Value == 0.25f && set.Particle2Value == 2.5f && s.overflowCalls == 2 && s.pos == 16;
    Stream bad = { 0x7ffffffe, buf, 16, buf, 0 }; Set set2 = { 1, 1, 1, 1 };
    set2.TransferBinaryIn(bad);                    // out-of-range position: nothing read, members keep the stale local copies
    ok = ok && bad.overflowCalls == 0 && bad.pos == 0x7ffffffe;
    std::printf(ok ? "0046944c_TEST PASS\n" : "0046944c_TEST FAIL\n");
    return ok ? 0 : 1;
}
