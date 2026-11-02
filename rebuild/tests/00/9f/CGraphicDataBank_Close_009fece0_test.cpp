// test: CGraphicDataBank::Close -- CloseGraphics (clear both arrays, CBankFile::Close) then CBankFileAsync::Close
#include <stdio.h>
#include <string.h>
#include <vector>
static char trace[32];
struct BankFile { bool open; virtual ~BankFile() { } virtual void Close() { open = false; strcat(trace, "F"); } };
struct BankFileAsync : BankFile { void Close() { strcat(trace, "A"); BankFile::Close(); } };
struct GraphicDataBank : BankFileAsync {
    std::vector<int> GraphicList; std::vector<int> GraphicFrameBuffer;
    void CloseGraphics() { GraphicList.clear(); GraphicFrameBuffer.clear(); strcat(trace, "c"); BankFile::Close(); }
    void Close() { CloseGraphics(); BankFileAsync::Close(); }
};
int main() {
    GraphicDataBank g; g.open = true; g.GraphicList.push_back(1); g.GraphicList.push_back(2); g.GraphicFrameBuffer.push_back(3);
    BankFile* b = &g; b->Close();
    if (strcmp(trace, "cFAF") != 0 || !g.GraphicList.empty() || !g.GraphicFrameBuffer.empty() || g.open) { printf("009fece0_TEST FAIL %s\n", trace); return 1; }
    printf("009fece0_TEST PASS\n");
    return 0;
}