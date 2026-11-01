// Behaviour test for CPersistContext::TransferVectorOfSubComponents<CFeatDef> @ 0x00F34330: save writes the count then
// each element's Transfer, binary load reads the count, resizes and transfers, reset transfers in place, text load delegates.
#include <cstdio>
#include <vector>
namespace model {
enum EMode { MODE_RESET = 0, MODE_SAVE_BINARY = 1, MODE_LOAD_BINARY = 2, MODE_LOAD_TEXT = 3 };
struct Stream { std::vector<long> words; unsigned pos; Stream() : pos(0) {} void WriteLong(long v) { words.push_back(v); } long ReadLong() { return words[pos++]; } };
struct CPersistContext;
template <class T> struct Item { long value; void Transfer(CPersistContext& ctx); };
struct CPersistContext {
    EMode Mode; Stream* PLoadStream; Stream* PSaveStream; int crcChecks; int textLoads;
    CPersistContext() : Mode(MODE_RESET), PLoadStream(0), PSaveStream(0), crcChecks(0), textLoads(0) {}
    void CheckCRC(const char*) { ++crcChecks; }
    template <class T> void TransferVectorOfSubComponents(const char* name, std::vector<T>& value)
    {
        CheckCRC(name);
        switch (Mode) {
        case MODE_RESET: for (unsigned i = 0; i < value.size(); ++i) value[i].Transfer(*this); break;
        case MODE_SAVE_BINARY: { long count = (long)value.size(); PSaveStream->WriteLong(count); for (unsigned i = 0; i < value.size(); ++i) value[i].Transfer(*this); break; }
        case MODE_LOAD_BINARY: { long count = PLoadStream->ReadLong(); value.resize(count); for (unsigned i = 0; i < value.size(); ++i) value[i].Transfer(*this); break; }
        case MODE_LOAD_TEXT: ++textLoads; break;
        }
    }
};
template <class T> void Item<T>::Transfer(CPersistContext& ctx)
{
    if (ctx.Mode == MODE_SAVE_BINARY) ctx.PSaveStream->WriteLong(value);
    else if (ctx.Mode == MODE_LOAD_BINARY) value = ctx.PLoadStream->ReadLong();
    else value = 0;
}
}
template <class T> bool RunTransferTest()
{
    using namespace model;
    Stream stream; CPersistContext ctx; ctx.PSaveStream = &stream; ctx.PLoadStream = &stream;
    std::vector<Item<T> > v(3); v[0].value = 7; v[1].value = 11; v[2].value = 13;
    ctx.Mode = MODE_SAVE_BINARY; ctx.TransferVectorOfSubComponents("Key", v);
    bool ok = stream.words.size() == 4 && stream.words[0] == 3 && stream.words[3] == 13 && ctx.crcChecks == 1;
    std::vector<Item<T> > w; ctx.Mode = MODE_LOAD_BINARY; ctx.TransferVectorOfSubComponents("Key", w);
    if (w.size() != 3 || w[1].value != 11) ok = false;
    ctx.Mode = MODE_RESET; ctx.TransferVectorOfSubComponents("Key", w);
    if (w[2].value != 0) ok = false;
    ctx.Mode = MODE_LOAD_TEXT; ctx.TransferVectorOfSubComponents("Key", w);
    if (ctx.textLoads != 1 || ctx.crcChecks != 4) ok = false;
    return ok;
}

class CFeatDef { public:};
int main()
{
    if (!RunTransferTest<CFeatDef>()) { std::printf("00f34330_TEST FAIL\n"); return 1; }
    std::printf("00f34330_TEST PASS\n");
    return 0;
}