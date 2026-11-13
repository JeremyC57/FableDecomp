// Behaviour test for C3DPrimitive2::UploadToIndexBuffer (0x00A8F9E0): the same fill logic over a plain copy of the model.
#include <cstdio>
#include <vector>
struct Tri { unsigned short v[3]; };
struct List { std::vector<unsigned short> strip; std::vector<Tri> tris; };
struct Block { unsigned long PrimitiveCount, StartIndex; bool IsStrip; unsigned long VertexCount; };
static unsigned long Upload(const std::vector<List>& lists, long repsField, unsigned long vertexCount, std::vector<Block>& blocks,
                            bool animated, std::vector<unsigned short>& out)
{
    long reps = repsField > 1 ? repsField : 1;
    unsigned long index_count = 0;
    for (unsigned long i = 0; i < lists.size(); ++i)
        index_count += lists[i].strip.size() > 0 ? lists[i].strip.size() : lists[i].tris.size() * 3;
    out.assign(index_count * reps, 0xFFFF);
    unsigned long pos = 0;
    for (long rep = 0; rep < reps; ++rep) {
        long offset = rep * vertexCount;
        for (unsigned long i = 0; i < lists.size(); ++i) {
            Block* b = &blocks[i];
            b->StartIndex = pos;
            if (lists[i].strip.size() > 0) {
                unsigned long n = lists[i].strip.size();
                b->IsStrip = true; b->PrimitiveCount = n - 2;
                for (unsigned long j = 0; j < n; ++j) out[pos++] = (unsigned short)(lists[i].strip[j] + offset);
            } else {
                unsigned long n = lists[i].tris.size();
                b->IsStrip = false; b->PrimitiveCount = n;
                for (unsigned long j = 0; j < n; ++j)
                    for (int k = 0; k < 3; ++k) out[pos++] = (unsigned short)(lists[i].tris[j].v[k] + offset);
            }
            if (animated) offset += blocks[i].VertexCount;
        }
    }
    return index_count * reps * 2;
}
int main()
{
    std::vector<List> lists(2);
    unsigned short s[4] = {0, 1, 2, 3};
    lists[0].strip.assign(s, s + 4);
    Tri t = {{4, 5, 6}};
    lists[1].tris.push_back(t);
    std::vector<Block> blocks(2);
    blocks[0].VertexCount = 10; blocks[1].VertexCount = 20;
    std::vector<unsigned short> out;
    unsigned long len = Upload(lists, 2, 7, blocks, false, out);
    if (len != 28 || out.size() != 14) { std::printf("00a8f9e0_TEST FAIL\n"); return 1; }
    unsigned short want[14] = {0,1,2,3,4,5,6, 7,8,9,10,11,12,13};
    for (int i = 0; i < 14; ++i) if (out[i] != want[i]) { std::printf("00a8f9e0_TEST FAIL\n"); return 1; }
    if (!blocks[0].IsStrip || blocks[0].PrimitiveCount != 2 || blocks[0].StartIndex != 7 || blocks[1].IsStrip || blocks[1].PrimitiveCount != 1 || blocks[1].StartIndex != 11) { std::printf("00a8f9e0_TEST FAIL\n"); return 1; }
    len = Upload(lists, 0, 7, blocks, true, out);
    unsigned short want2[7] = {0,1,2,3,14,15,16};
    if (len != 14) { std::printf("00a8f9e0_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 7; ++i) if (out[i] != want2[i]) { std::printf("00a8f9e0_TEST FAIL\n"); return 1; }
    std::printf("00a8f9e0_TEST PASS\n");
    return 0;
}