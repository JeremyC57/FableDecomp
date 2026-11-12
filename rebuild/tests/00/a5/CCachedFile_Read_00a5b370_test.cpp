#include <cstdio>
#include <cstring>
#include <vector>
// Copy of the model: a disk file over a byte array, a cached file with a small buffer.
struct DiskFile {
    const unsigned char* data; long length; long position; int reads;
    void SetPosition(long p) { position = p; }
    long GetPosition() const { return position; }
    long GetLength() const { return length; }
    void Read(void* dst, long n) { std::memcpy(dst, data + position, n); position += n; ++reads; }
};
template <class T> inline const T& Min(const T& a, const T& b) { return a < b ? a : b; }
struct CachedFile {
    long Position, BufferStartPosition; std::vector<unsigned char> DataBuffer; DiskFile* PDiskFile; bool BufferValid;
    int Read(void* destination, long size)   // returns 0 ok, else the exception type
    {
        unsigned char* out = (unsigned char*)destination;
        if (destination == 0) return 1;
        if (size == 0) return 0;
        if (Position + size > PDiskFile->GetLength()) return 3;
        if (!BufferValid) {
            BufferStartPosition = Position;
            PDiskFile->SetPosition(Position);
            long remaining = -PDiskFile->GetPosition() + PDiskFile->GetLength();
            long bufferSize = (long)DataBuffer.size();
            PDiskFile->Read(&DataBuffer[0], Min(remaining, bufferSize));
            BufferValid = true;
        }
        long bytesLeft = size;
        while (bytesLeft > 0) {
            while ((unsigned long)(Position - BufferStartPosition) < (unsigned long)(DataBuffer.end() - DataBuffer.begin()) && Position - BufferStartPosition >= 0 && bytesLeft > 0) {
                *out++ = DataBuffer[Position - BufferStartPosition];
                ++Position;
                --bytesLeft;
            }
            if (bytesLeft > 0) {
                BufferStartPosition = Position;
                PDiskFile->SetPosition(Position);
                long remaining = PDiskFile->GetLength() - PDiskFile->GetPosition();
                long bufferSize = (long)DataBuffer.size();
                PDiskFile->Read(&DataBuffer[0], Min(remaining, bufferSize));
            }
        }
        return 0;
    }
};
int main()
{
    unsigned char src[23];
    for (int i = 0; i < 23; ++i) src[i] = (unsigned char)(i * 7 + 1);
    DiskFile disk = { src, 23, 0, 0 };
    CachedFile f; f.Position = 0; f.BufferStartPosition = 0; f.DataBuffer.resize(5); f.PDiskFile = &disk; f.BufferValid = false;
    unsigned char out[32]; std::memset(out, 0, sizeof out);
    bool ok = true;
    if (f.Read(0, 3) != 1) ok = false;
    if (f.Read(out, 24) != 3) ok = false;
    if (f.Read(out, 0) != 0 || disk.reads != 0) ok = false;
    if (f.Read(out, 7) != 0) ok = false;              // 0..6: two buffer fills
    if (std::memcmp(out, src, 7) != 0 || f.Position != 7 || disk.reads != 2) ok = false;
    if (f.Read(out + 7, 16) != 0) ok = false;         // 7..22, last fill clipped to what is left
    if (std::memcmp(out, src, 23) != 0 || f.Position != 23) ok = false;
    if (f.Read(out, 1) != 3) ok = false;
    if (!ok) { std::printf("00a5b370_TEST FAIL\n"); return 1; }
    std::printf("00a5b370_TEST PASS\n");
    return 0;
}