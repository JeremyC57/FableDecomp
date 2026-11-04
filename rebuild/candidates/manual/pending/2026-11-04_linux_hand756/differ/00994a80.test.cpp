// Behaviour test for CCompressedMemoryDataInputStream::SetPosition (0x00994A80): a model of the row -- a target inside the
// current source chunk only moves the chunk cursor; a target behind the position (outside the chunk) is ignored; a forward
// target inflates min(BufferSize, distance) bytes at a time and then re-synchronises through the base SetPosition.
#include <stdio.h>
#include <vector>
struct Stream {
    unsigned long StreamPos, SourceChunkStreamPos; long Remaining; long Cursor; long BufferSize;
    std::vector<long> Reads; unsigned long BaseSet;
    bool CanSeek(long p) { return (unsigned long)p >= SourceChunkStreamPos && (unsigned long)p <= StreamPos + Remaining; }
    void Assume(unsigned long p) { Cursor += p - StreamPos; Remaining += StreamPos - p; StreamPos = p; }
    void SetPosition(unsigned long position)
    {
        unsigned long current = StreamPos;
        if (position == current) return;
        if (CanSeek(position)) { Assume(position); return; }
        if (position > current) {
            while (position > current) {
                unsigned long chunk = (unsigned long)BufferSize < position - current ? BufferSize : position - current;
                Reads.push_back(chunk); current += chunk;
            }
            BaseSet = position;
        }
    }
};
int main()
{
    Stream s = { 100, 90, 20, 0, 16, std::vector<long>(), 0 };
    s.SetPosition(110);                      // inside [90, 120]
    int ok = s.StreamPos == 110 && s.Cursor == 10 && s.Remaining == 10 && s.Reads.empty();
    s.SetPosition(50);                       // behind the chunk: ignored
    ok = ok && s.StreamPos == 110 && s.Reads.empty() && s.BaseSet == 0;
    s.SetPosition(150);                      // forward: 16 + 16 + 8
    ok = ok && s.Reads.size() == 3 && s.Reads[0] == 16 && s.Reads[2] == 8 && s.BaseSet == 150;
    printf(ok ? "00994a80_TEST PASS\n" : "00994a80_TEST FAIL\n");
    return ok ? 0 : 1;
}
