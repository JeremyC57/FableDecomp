// Behaviour test for NGameText::CDataBank::~CDataBank (0x009C87C0): CloseTextAndGroups, members in reverse order, then bases.
#include <cstdio>

static char g_log[32];
static int g_logLength = 0;
static void Log(char c) { if (g_logLength < 31) g_log[g_logLength++] = c; g_log[g_logLength] = 0; }

struct Element { char Tag; ~Element() { Log(Tag); } };

template <class T>
struct Array {
    T Items[2];
    char Tag;
    ~Array() { Log(Tag); }                 // storage freed after the elements die
};

struct BankFileAsync { virtual ~BankFileAsync() { Log('B'); } };
struct SecondaryBase { virtual ~SecondaryBase() { Log('S'); } };

struct DataBank : public BankFileAsync, public SecondaryBase {
    Array<Element> Entries;
    Array<Element> TextEntryBuffer;
    Array<Element> GroupEntryBuffer;
    Array<Element> Narrators;
    DataBank()
    {
        Entries.Tag = 'e'; TextEntryBuffer.Tag = 't'; GroupEntryBuffer.Tag = 'g'; Narrators.Tag = 'n';
        for (int i = 0; i < 2; ++i) {
            Entries.Items[i].Tag = '1'; TextEntryBuffer.Items[i].Tag = '2';
            GroupEntryBuffer.Items[i].Tag = '3'; Narrators.Items[i].Tag = '4';
        }
    }
    void CloseTextAndGroups() { Log('C'); }
    virtual ~DataBank() { CloseTextAndGroups(); }
};

int main()
{
    BankFileAsync* bank = new DataBank;
    delete bank;
    const char* expected = "Cn44g33t22e11SB";
    for (int i = 0; ; ++i) {
        if (g_log[i] != expected[i]) { std::printf("009c87c0_TEST FAIL\n"); return 1; }
        if (!expected[i]) break;
    }
    std::printf("009c87c0_TEST PASS\n");
    return 0;
}