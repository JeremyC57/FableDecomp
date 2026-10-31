// Behaviour test for DecodeAdpcmToPcm (0x00C34630): only codec-1 records decode; the destination must hold the PCM size;
// a failed decode returns -1; a good one rewrites the record as PCM (codec 0, decoded length, loop points re-quantised).
#include <stdio.h>
struct Fmt { unsigned long length, loopStart, loopLength; unsigned short channels, codec; unsigned long rate;
    unsigned long BytesToSamples(unsigned long b) { return b / 36 * 64; }
    unsigned long SamplesToBytes(unsigned long s) { return s * 2; } };
static long decodeResult;
static long Decode(const void*, unsigned long, unsigned long, void*, unsigned long) { return decodeResult; }
static long DecodeAdpcmToPcm(Fmt* format, const void* source, void* dest, unsigned long destSize)
{
    if (format->codec != 1) return -1;
    unsigned long needed = format->BytesToSamples(format->length) * 2;
    if (destSize < needed) return -1;
    long written = Decode(source, format->length, format->channels, dest, needed);
    if (written <= 0) return -1;
    const Fmt& original = *format;
    format->channels = original.channels;
    format->codec = 0;
    format->length = written;
    format->loopStart = format->SamplesToBytes(format->BytesToSamples(format->loopStart));
    format->loopLength = format->SamplesToBytes(format->BytesToSamples(format->loopLength));
    format->rate = original.rate;
    return 0;
}
int main()
{
    Fmt f = { 360, 72, 108, 2, 1, 44100 };
    if (DecodeAdpcmToPcm(&f, 0, 0, 100) != -1) { printf("00c34630_TEST FAIL\n"); return 1; }
    decodeResult = 0;
    if (DecodeAdpcmToPcm(&f, 0, 0, 2000) != -1 || f.codec != 1) { printf("00c34630_TEST FAIL\n"); return 1; }
    decodeResult = 1280;
    if (DecodeAdpcmToPcm(&f, 0, 0, 2000) != 0 || f.codec != 0 || f.length != 1280 || f.loopStart != 256 || f.loopLength != 384
        || f.channels != 2 || f.rate != 44100) { printf("00c34630_TEST FAIL\n"); return 1; }
    Fmt g = { 360, 0, 0, 1, 0, 22050 };
    if (DecodeAdpcmToPcm(&g, 0, 0, 2000) != -1) { printf("00c34630_TEST FAIL\n"); return 1; }
    printf("00c34630_TEST PASS\n"); return 0;
}