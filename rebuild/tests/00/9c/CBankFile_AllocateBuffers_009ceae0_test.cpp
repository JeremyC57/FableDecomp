// Behaviour test for CBankFile::AllocateBuffers 0x009CEAE0 (NF68: restated after the port onto engine/CBankFile.h): a copy of the
// row's logic over std containers. Size is set; the runtime array is released and refilled with n zeroed records; the symbol,
// checksum and update arrays are released and refilled with n entries only when OpenFlags bit 3 / 4 / 5 is set; both symbol trees
// are emptied.
#include <cstdio>
#include <string>
#include <vector>
#include <map>

struct RuntimeRecord { unsigned long DataOffset, DataSize; unsigned char DataType; bool Valid;
    RuntimeRecord() : DataOffset(0), DataSize(0), DataType(0), Valid(false) {} };
struct UpdateRecord;
template<class T> void Release(std::vector<T>& v) { if (v.capacity() > 0) { v.clear(); std::vector<T> empty; v.swap(empty); } }

struct Bank {
    unsigned long Size, OpenFlags;
    std::vector<std::string> Symbols; std::vector<unsigned long> Checksums; std::vector<RuntimeRecord> Runtime;
    std::vector<UpdateRecord*> Updates;
    std::map<std::string, unsigned long> SymbolIndices; std::multimap<std::string, unsigned long> FilenameIndices;
    void AllocateBuffers(unsigned long size)
    {
        Size = size;
        Release(Runtime); Runtime.resize(size, RuntimeRecord());
        Release(Symbols); if (OpenFlags & 8) Symbols.resize(size);
        Release(Checksums); if (OpenFlags & 16) Checksums.resize(size, 0);
        Release(Updates); if (OpenFlags & 32) Updates.resize(size, 0);
        SymbolIndices.clear(); FilenameIndices.clear();
    }
};

int main()
{
    for (unsigned bits = 0; bits < 8; ++bits) {
        Bank b; b.Size = 0; b.OpenFlags = bits << 3;
        b.SymbolIndices["a"] = 1; b.FilenameIndices.insert(std::make_pair(std::string("f"), 1ul));
        RuntimeRecord dirty; dirty.DataOffset = 7; dirty.Valid = true; b.Runtime.push_back(dirty);
        b.Symbols.push_back("x"); b.Checksums.push_back(9); b.Updates.push_back(0);
        b.AllocateBuffers(3);
        if (b.Size != 3 || b.Runtime.size() != 3 || b.Symbols.size() != ((bits & 1) ? 3u : 0u) || b.Checksums.size() != ((bits & 2) ? 3u : 0u)
            || b.Updates.size() != ((bits & 4) ? 3u : 0u) || !b.SymbolIndices.empty() || !b.FilenameIndices.empty()) { std::printf("009ceae0_TEST FAIL\n"); return 1; }
        for (unsigned i = 0; i < 3; ++i)
            if (b.Runtime[i].DataOffset || b.Runtime[i].DataSize || b.Runtime[i].DataType || b.Runtime[i].Valid) { std::printf("009ceae0_TEST FAIL\n"); return 1; }
        b.AllocateBuffers(0);
        if (b.Size != 0 || !b.Runtime.empty() || !b.Symbols.empty()) { std::printf("009ceae0_TEST FAIL\n"); return 1; }
    }
    std::printf("009ceae0_TEST PASS\n");
    return 0;
}
