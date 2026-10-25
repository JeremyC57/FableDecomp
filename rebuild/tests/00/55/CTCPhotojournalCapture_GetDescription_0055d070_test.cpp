// Behaviour test for CTCPhotojournalCapture::GetDescription (0x0055D070): the caption length is the file's last dword,
// the caption (UTF-16, null-terminated) sits just before it; lengths outside 0..512 give the empty string -- through a
// copy of the model over an in-memory file.
#include <cstdio>
#include <cstring>

typedef unsigned short wchar16;

struct MemoryFile {
    const unsigned char* data; int Length; int Position; int closes;
    MemoryFile(const unsigned char* d, int n) : data(d), Length(n), Position(0), closes(0) {}
    void SetPosition(int p) { Position = p; }
    void Read(void* out, int size) { std::memcpy(out, data + Position, size); Position += size; }
    void Close() { ++closes; }
};

static int Describe(MemoryFile& file, wchar16* out)          // returns the caption length in characters, -1 = empty string
{
    int fileLength = file.Length;
    file.SetPosition(fileLength - 4);
    int descriptionLength;
    file.Read(&descriptionLength, 4);
    if (descriptionLength >= 0 && descriptionLength <= 0x200) {
        file.SetPosition(fileLength - descriptionLength - 4);
        wchar16* buffer = new wchar16[descriptionLength];
        file.Read(buffer, descriptionLength);
        int n = 0;
        while (buffer[n]) { out[n] = buffer[n]; ++n; }
        out[n] = 0;
        delete[] buffer;
        file.Close();
        return n;
    }
    file.Close();
    out[0] = 0;
    return -1;
}

int main()
{
    unsigned char image[64];
    std::memset(image, 0xAB, sizeof(image));                 // picture bytes
    const wchar16 caption[] = { 'H', 'i', '!', 0 };
    int bytes = sizeof(caption);
    std::memcpy(image + 64 - 4 - bytes, caption, bytes);
    std::memcpy(image + 60, &bytes, 4);
    MemoryFile f(image, 64);
    wchar16 text[64];
    if (Describe(f, text) != 3 || text[0] != 'H' || text[2] != '!' || f.closes != 1) { std::printf("0055d070_TEST FAIL\n"); return 1; }

    int bad = 0x201;
    std::memcpy(image + 60, &bad, 4);
    MemoryFile g(image, 64);
    if (Describe(g, text) != -1 || text[0] != 0 || g.closes != 1) { std::printf("0055d070_TEST FAIL\n"); return 1; }
    int negative = -2;
    std::memcpy(image + 60, &negative, 4);
    MemoryFile h(image, 64);
    if (Describe(h, text) != -1) { std::printf("0055d070_TEST FAIL\n"); return 1; }
    std::printf("0055d070_TEST PASS\n");
    return 0;
}