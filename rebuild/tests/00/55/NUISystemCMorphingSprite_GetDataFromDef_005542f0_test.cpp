// Behaviour test for 005542f0: the row body verbatim over fixture models of the shared-header interfaces it calls.
// NUISystem::CMorphingSprite::GetDataFromDef() -- records every state's graphic index in GraphicIndices and starts loading (and forces) each state graphic's first frame
#include <stdio.h>
#include <string.h>
#include <vector>
#include <map>
#include <utility>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// Fixtures: a counted def handle (CDefPointer: Refs on the pointee, released on destruction); the UI def's States vector of
// CUIStateDef (GraphicIndex); the manager singleton whose GetGraphicsBank returns a counted bank handle by value; the bank's
// PeekGraphicInfoFrame(index, frame) returning an info-frame record that logs BeginLoading / GetFrame calls (order, arguments);
// CGraphicFrame counting constructions / destructions; the sprite's virtual LoadAndReturnDef handing out its def.
static int g_log[64][4]; static int g_logN = 0;
static void Log(int kind, int a, int b, int c) { if (g_logN < 64) { g_log[g_logN][0] = kind; g_log[g_logN][1] = a; g_log[g_logN][2] = b; g_log[g_logN][3] = c; } ++g_logN; }
static int g_frameCtor = 0, g_frameDtor = 0;
class CGraphicFrame { public: CGraphicFrame() { ++g_frameCtor; } ~CGraphicFrame() { --g_frameCtor; ++g_frameDtor; } };
class CGraphicInfoFrame { public:
    int Index;
    void BeginLoading(unsigned long a, unsigned long b) const { Log(1, Index, (int)a, (int)b); }
    bool GetFrame(CGraphicFrame&, unsigned long a, unsigned long b) const { Log(2, Index, (int)a, (int)b); return true; }
};
class CGraphicDataBank { public:
    int Refs; CGraphicInfoFrame Frames[16];
    const CGraphicInfoFrame* PeekGraphicInfoFrame(unsigned long index, unsigned long frame) const { Log(3, (int)index, (int)frame, 0); return &Frames[index]; }
};
template <class T> class CDefPointer { public:
    T* P;
    CDefPointer(T* p) : P(p) { if (P) ++P->Refs; }
    CDefPointer(const CDefPointer& o) : P(o.P) { if (P) ++P->Refs; }
    ~CDefPointer() { if (P) --P->Refs; }
    T* operator->() const { return P; }
};
template <class T> class CCountedPointer { public:
    T* P;
    CCountedPointer(T* p) : P(p) { ++P->Refs; }
    CCountedPointer(const CCountedPointer& o) : P(o.P) { ++P->Refs; }
    ~CCountedPointer() { --P->Refs; }
    T* operator->() const { return P; }
};
static CGraphicDataBank g_bank;
namespace NUISystem {
struct CUIStateDef { unsigned long GraphicIndex; };
struct CUIDef { mutable int Refs; std::vector<CUIStateDef> States; };
class CManager { public:
    static CManager* GetInstance() { static CManager m; return &m; }
    CCountedPointer<CGraphicDataBank> GetGraphicsBank() { return CCountedPointer<CGraphicDataBank>(&g_bank); }
};
class CMorphingSprite { public:
    CUIDef* Def; std::map<unsigned long, unsigned long> GraphicIndices; mutable int Loads;
    virtual CDefPointer<const CUIDef> LoadAndReturnDef() const { ++Loads; return CDefPointer<const CUIDef>(Def); }
    void GetDataFromDef();
};
// ---- row body (verbatim) ----
void CMorphingSprite::GetDataFromDef()
{
    CDefPointer<const CUIDef> pdefinition = LoadAndReturnDef();
    for (unsigned long count = 0; count < pdefinition->States.size(); ++count) {
        std::pair<const unsigned long, unsigned long> map_entry(count, pdefinition->States[count].GraphicIndex);
        GraphicIndices.insert(GraphicIndices.begin(), map_entry);
        const CGraphicInfoFrame* pinfo = CManager::GetInstance()->GetGraphicsBank()->PeekGraphicInfoFrame(pdefinition->States[count].GraphicIndex, 0);
        pinfo->BeginLoading(0, 0x10);
        CGraphicFrame frame;
        pinfo->GetFrame(frame, 0, 1);
    }
}
}
// ---- harness ----
int main()
{
    using namespace NUISystem;
    // (all class objects declared before the first loop: VC7.1 /Od never finishes this TU when a for loop precedes them)
    CUIDef def; CUIDef empty; CMorphingSprite sprite;
    for (int f = 0; f < 16; ++f) g_bank.Frames[f].Index = f;
    g_bank.Refs = 1;
    def.Refs = 1;
    unsigned long idx[3] = { 5, 2, 7 };
    for (int n = 0; n < 3; ++n) { CUIStateDef st; st.GraphicIndex = idx[n]; def.States.push_back(st); }
    sprite.Def = &def; sprite.Loads = 0;
    sprite.GraphicIndices[1] = 99;                     // an existing entry: insert keeps it
    sprite.GetDataFromDef();
    CHECK(sprite.Loads == 1 && def.Refs == 1 && g_bank.Refs == 1);
    CHECK(sprite.GraphicIndices.size() == 3 && sprite.GraphicIndices[0] == 5 && sprite.GraphicIndices[1] == 99 && sprite.GraphicIndices[2] == 7);
    // per state: Peek(index, 0), BeginLoading(0, 0x10), GetFrame(frame, 0, 1), in state order
    CHECK(g_logN == 9);
    for (int i = 0; i < 3; ++i) {
        CHECK(g_log[3 * i][0] == 3 && g_log[3 * i][1] == (int)idx[i] && g_log[3 * i][2] == 0);
        CHECK(g_log[3 * i + 1][0] == 1 && g_log[3 * i + 1][1] == (int)idx[i] && g_log[3 * i + 1][2] == 0 && g_log[3 * i + 1][3] == 0x10);
        CHECK(g_log[3 * i + 2][0] == 2 && g_log[3 * i + 2][1] == (int)idx[i] && g_log[3 * i + 2][2] == 0 && g_log[3 * i + 2][3] == 1);
    }
    CHECK(g_frameDtor == 3 && g_frameCtor == 0);
    // a def with no states: nothing touched
    empty.Refs = 1; sprite.Def = &empty; g_logN = 0;
    sprite.GetDataFromDef();
    CHECK(g_logN == 0 && sprite.GraphicIndices.size() == 3 && empty.Refs == 1 && sprite.Loads == 2);
    if (fails == 0) printf("005542f0_TEST PASS\n");
    return fails;
}