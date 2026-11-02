// Behaviour test for 0x00A620D0 CShaderDataBank::~CShaderDataBank(): CloseShaders deletes every entry, then StateBlock and
// the Shaders storage are destroyed, then the CBankFile base.
#include <cstdio>
#include <cstdlib>

static int g_order[8];
static int g_n = 0;
static int g_entriesDeleted = 0;

class CShaderBankEntry {
public:
    virtual ~CShaderBankEntry() { ++g_entriesDeleted; }
};
class CBankFile {
public:
    virtual ~CBankFile() { g_order[g_n++] = 4; }
};
class CBankStateBlock {
public:
    virtual ~CBankStateBlock() { g_order[g_n++] = 2; }
};
class CShaderBankStateBlock : public CBankStateBlock {
public:
    int ShaderTarget;
    bool EnableShaderOptimisation;
};
class CEntryArray {
public:
    CShaderBankEntry** First;
    CShaderBankEntry** Last;
    CShaderBankEntry** End;
    CEntryArray() : First(0), Last(0), End(0) { }
    ~CEntryArray() { g_order[g_n++] = 3; if (First) std::free(First); }
};
class CShaderDataBank : public CBankFile {
public:
    virtual ~CShaderDataBank() { CloseShaders(); }
    CEntryArray Shaders;
    CShaderBankStateBlock StateBlock;
private:
    void CloseShaders()
    {
        g_order[g_n++] = 1;
        for (unsigned int i = 0; i < (unsigned int)(Shaders.Last - Shaders.First); ++i) {
            CShaderBankEntry* entry = Shaders.First[i];
            if (entry)
                delete entry;
        }
        Shaders.Last = Shaders.First;
    }
};

int main()
{
    CBankFile* bank = new CShaderDataBank;
    CShaderDataBank* shaders = (CShaderDataBank*)bank;
    shaders->Shaders.First = (CShaderBankEntry**)std::malloc(3 * sizeof(CShaderBankEntry*));
    shaders->Shaders.First[0] = new CShaderBankEntry;
    shaders->Shaders.First[1] = 0;
    shaders->Shaders.First[2] = new CShaderBankEntry;
    shaders->Shaders.Last = shaders->Shaders.First + 3;
    shaders->Shaders.End = shaders->Shaders.Last;
    delete bank;
    if (g_entriesDeleted != 2 || g_n != 4 || g_order[0] != 1 || g_order[1] != 2 || g_order[2] != 3 || g_order[3] != 4) {
        std::printf("00a620d0_TEST FAIL\n");
        return 1;
    }
    std::printf("00a620d0_TEST PASS\n");
    return 0;
}