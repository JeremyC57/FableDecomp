// test: CGraphicDataBank::CloseGraphics -- clear the graphic and frame arrays, then CBankFile::Close
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
    g.CloseGraphics();
    if (strcmp(trace, "cF") != 0 || !g.GraphicList.empty() || !g.GraphicFrameBuffer.empty() || g.open) { printf("009feb80_TEST FAIL %s\n", trace); return 1; }
    printf("009feb80_TEST PASS\n");
    return 0;
}