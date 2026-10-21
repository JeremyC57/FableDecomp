#include <cstdio>
#include <cstring>
// Behaviour model of CReactionFrequencyTraits_PeriodicFrequency::TransferBinaryIn (0x006FFC30):
// five 4-byte reads (two floats, three frame counts) from a chunked memory stream; the fast path copies from the
// current chunk, the slow path goes through the chunk-overflow reader; positions past 0x7fffffff are skipped.
struct Stream {
    unsigned int position;
    const unsigned char* data;
    int remaining;
    const unsigned char* overflow;   // next chunk used by the overflow reader
    int overflowCalls;
    void ReadWithSrcChunkOverflow(unsigned char* buffer, int size)
    {
        int first = remaining;
        std::memcpy(buffer, data, first);
        std::memcpy(buffer + first, overflow, size - first);
        data = overflow + (size - first);
        remaining = 64;
        position += size;
        ++overflowCalls;
    }
    void Read(void* buffer, int size)
    {
        double newPosition = (double)position + size;
        if (!(newPosition >= 0 && newPosition <= 2147483647.0))
            return;
        if (remaining >= size) {
            std::memcpy(buffer, data, size);
            data += size;
            remaining -= size;
            position += size;
        }
        else
            ReadWithSrcChunkOverflow((unsigned char*)buffer, size);
    }
    void ReadValue(float& out) { float value = 0; Read(&value, sizeof(value)); out = value; }
    void ReadValue(unsigned int& out) { unsigned int value = 0; Read(&value, sizeof(value)); out = value; }
};
struct Traits {
    float LowFreqPerMin, HighLowFreqInterval;
    unsigned int FramesPerMinute, LastCutoffFrame, RampUpTimeFrames, CutoffIntervalFrames, LastHappenedFrame;
    void TransferBinaryIn(Stream& stream)
    {
        stream.ReadValue(LowFreqPerMin);
        stream.ReadValue(HighLowFreqInterval);
        stream.ReadValue(FramesPerMinute);
        stream.ReadValue(RampUpTimeFrames);
        stream.ReadValue(CutoffIntervalFrames);
    }
};
int main()
{
    unsigned char buf[20];
    float a = 1.5f, b = -2.25f;
    unsigned int c = 30, d = 7, e = 900;
    std::memcpy(buf, &a, 4); std::memcpy(buf + 4, &b, 4); std::memcpy(buf + 8, &c, 4);
    std::memcpy(buf + 12, &d, 4); std::memcpy(buf + 16, &e, 4);
    // whole record in one chunk
    Stream s1 = { 0, buf, 20, 0, 0 };
    Traits t1; t1.LastCutoffFrame = 11; t1.LastHappenedFrame = 12;
    t1.TransferBinaryIn(s1);
    bool ok = t1.LowFreqPerMin == 1.5f && t1.HighLowFreqInterval == -2.25f && t1.FramesPerMinute == 30
        && t1.RampUpTimeFrames == 7 && t1.CutoffIntervalFrames == 900 && t1.LastCutoffFrame == 11
        && t1.LastHappenedFrame == 12 && s1.position == 20 && s1.remaining == 0 && s1.overflowCalls == 0;
    // record split across two chunks: third value straddles the boundary
    Stream s2 = { 100, buf, 10, buf + 10, 0 };
    Traits t2; t2.LastCutoffFrame = 0; t2.LastHappenedFrame = 0;
    t2.TransferBinaryIn(s2);
    ok = ok && t2.FramesPerMinute == 30 && t2.CutoffIntervalFrames == 900 && s2.overflowCalls == 1 && s2.position == 120;
    // position out of range: the stream is not consumed (retail still stores the unread local, so the
    // members' values are unspecified there and are not checked)
    Stream s3 = { 0x7ffffffeu, buf, 20, 0, 0 };
    Traits t3;
    t3.TransferBinaryIn(s3);
    ok = ok && s3.remaining == 20 && s3.position == 0x7ffffffeu && s3.data == buf && s3.overflowCalls == 0;
    if (!ok) { std::printf("006ffc30_TEST FAIL\n"); return 1; }
    std::printf("006ffc30_TEST PASS\n");
    return 0;
}