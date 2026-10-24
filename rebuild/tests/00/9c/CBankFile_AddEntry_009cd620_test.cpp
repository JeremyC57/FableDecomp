// Behaviour test for CBankFile::AddEntry (0x009CD620): one-element filename array forwarded to AddEntryMultipleFiles.
#include <cstdio>
#include <cstring>

static int g_liveStrings = 0;
struct Str {
    char Text[16];
    Str() { Text[0] = 0; ++g_liveStrings; }
    Str(const char* t) { std::strcpy(Text, t); ++g_liveStrings; }
    Str(const Str& o) { std::strcpy(Text, o.Text); ++g_liveStrings; }
    ~Str() { --g_liveStrings; }
    Str& operator=(const Str& o) { std::strcpy(Text, o.Text); return *this; }
};

static int g_destroyed = 0;
struct Info { long References; void* Object; };
struct Counted {
    void* Data; Info* Block;
    Counted(void* d, Info* b) : Data(d), Block(b) {}
    Counted(const Counted& o) : Data(o.Data), Block(o.Block) { if (Block) ++Block->References; }
    ~Counted() { if (Block && --Block->References == 0) ++g_destroyed; }
};

struct StrArray {
    Str* Start; Str* Finish;
    explicit StrArray(unsigned int n) : Start(new Str[n]), Finish(Start + n) {}
    ~StrArray() { delete[] Start; }
    Str& operator[](unsigned int i) { return Start[i]; }
};

struct Bank {
    Str LastSymbol; unsigned long LastType; unsigned int LastCount; Str LastFile; long RefsDuringCall;
    virtual ~Bank() {}
    virtual void AddEntryMultipleFiles(const Str& symbol, unsigned long type, const StrArray& files, Counted state)
    {
        LastSymbol = symbol; LastType = type; LastCount = (unsigned int)(files.Finish - files.Start);
        LastFile = files.Start[0]; RefsDuringCall = state.Block ? state.Block->References : -1;
    }
    void AddEntry(const Str& symbol, unsigned long type, const Str& filename, Counted state)
    {
        StrArray filenames(1);
        filenames[0] = filename;
        AddEntryMultipleFiles(symbol, type, filenames, state);
    }
};

int main()
{
    Bank bank;
    Info block = { 1, 0 };
    {
        Counted state(&block, &block);
        Str symbol("SYM_A"), file("a.dat");
        bank.AddEntry(symbol, 7, file, state);
        if (std::strcmp(bank.LastSymbol.Text, "SYM_A") != 0 || bank.LastType != 7 || bank.LastCount != 1 ||
            std::strcmp(bank.LastFile.Text, "a.dat") != 0 || bank.RefsDuringCall != 3 || block.References != 1)
        { std::printf("009cd620_TEST FAIL\n"); return 1; }
    }
    if (g_destroyed != 1 || g_liveStrings != 2) { std::printf("009cd620_TEST FAIL\n"); return 1; }
    std::printf("009cd620_TEST PASS\n");
    return 0;
}