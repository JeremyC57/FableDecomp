// behaviour test for CPersistContext::TransferVector<unsigned long> 0x005E9840: the mode picks reset (clear), binary
// save / load, or the text save / load through the vector wrapper
#include <cstdio>
#include <vector>
#include <string>
enum EMode { LOAD_TEXT, SAVE_TEXT, LOAD_BINARY, SAVE_BINARY, RESET };
static const char* Dispatch(EMode m, std::vector<unsigned long>& v)
{
    switch (m) {
    case RESET: v.clear(); return "clear";
    case SAVE_BINARY: return "binout";
    case LOAD_BINARY: return "binin";
    case SAVE_TEXT: return "textout";
    case LOAD_TEXT: return "textin";
    }
    return "";
}
int main()
{
    std::vector<unsigned long> v(3, 7);
    int ok = std::string(Dispatch(SAVE_TEXT, v)) == "textout" && v.size() == 3 && std::string(Dispatch(RESET, v)) == "clear" && v.empty()
        && std::string(Dispatch(LOAD_BINARY, v)) == "binin";
    std::printf(ok ? "005e9840_TEST PASS\n" : "005e9840_TEST FAIL\n");
    return !ok;
}