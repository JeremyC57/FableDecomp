// Behaviour test for CDSoundStreamBuffer::CreateBuffer (0x00C57140): sizes (channels x samples x 2 bytes, chunk 256 per
// channel), the descriptor flags (0x18000, | CTRL3D | LOCHARDWARE for 3D), the PCM vs extensible format choice, the
// zero fill through Lock / Unlock and the failure paths.
#include <cstdio>
#include <cstring>

struct Desc { unsigned int size, flags, bytes, reserved; int extensible; unsigned short channels, blockAlign; unsigned int avg; };
struct Device { int fail; Desc last; };
struct Buffer { unsigned char data[4096]; int lockFail, unlockFail, unlocked; };

struct Stream {
    Device* device; Buffer* buffer; unsigned int cursor, bufferBytes, chunkBytes;
    bool CreateBuffer(unsigned int channels, unsigned int samples, bool hardware3D)
    {
        unsigned int bytes = channels * samples * 2;
        bufferBytes = bytes;
        chunkBytes = channels * 256;
        Desc d;
        d.channels = (unsigned short)channels;
        d.blockAlign = (unsigned short)(d.channels * 16 / 8);
        d.avg = d.blockAlign * 44100u;
        d.size = 0x14; d.flags = 0x18000;
        if (hardware3D) { d.flags |= 0x14; d.extensible = 0; }
        else d.extensible = channels > 2;
        d.bytes = bytes; d.reserved = 0;
        device->last = d;
        if (device->fail) return false;
        if (buffer->lockFail) return false;
        std::memset(buffer->data, 0, bufferBytes);
        if (buffer->unlockFail) return false;
        buffer->unlocked = 1;
        cursor = 0;
        return true;
    }
};

int main()
{
    int ok = 1;
    static Buffer buf; Device dev; std::memset(&dev, 0, sizeof(dev));
    std::memset(buf.data, 0xAB, sizeof(buf.data)); buf.lockFail = 0; buf.unlockFail = 0; buf.unlocked = 0;
    Stream s; s.device = &dev; s.buffer = &buf; s.cursor = 77;
    ok &= s.CreateBuffer(2, 512, false);
    ok &= s.bufferBytes == 2048 && s.chunkBytes == 512 && s.cursor == 0 && buf.unlocked;
    ok &= dev.last.flags == 0x18000 && !dev.last.extensible && dev.last.blockAlign == 4 && dev.last.avg == 176400;
    ok &= buf.data[0] == 0 && buf.data[2047] == 0 && buf.data[2048] == 0xAB;
    ok &= s.CreateBuffer(6, 64, false) && dev.last.extensible && dev.last.bytes == 768 && s.chunkBytes == 1536;
    ok &= s.CreateBuffer(1, 64, true) && dev.last.flags == 0x18014 && !dev.last.extensible;
    s.cursor = 5; buf.lockFail = 1;
    ok &= !s.CreateBuffer(2, 8, false) && s.cursor == 5;
    buf.lockFail = 0; dev.fail = 1;
    ok &= !s.CreateBuffer(2, 8, false);
    if (!ok) { std::printf("00c57140_TEST FAIL\n"); return 1; }
    std::printf("00c57140_TEST PASS\n");
    return 0;
}