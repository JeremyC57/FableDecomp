#include <cstdio>
#include <cstring>
// Behaviour test for CNavBlockedQuadTreeNode::SaveToFile (0x00A76950).
struct File { unsigned char buf[16]; int n; void Write(const void* d, long size, void*) { std::memcpy(buf + n, d, size); n += size; } };
struct Node {
    bool Blocked;
    int GetNodeType() const { return 2; }
    void SaveToFile(File& f, bool writeHeader) const
    {
        if (writeHeader) {
            unsigned char type = (unsigned char)GetNodeType();
            f.Write(&type, 1, 0); f.Write(&writeHeader, 1, 0); f.Write(&Blocked, 1, 0);
        }
    }
};
int main()
{
    Node n; n.Blocked = true;
    File f; f.n = 0; n.SaveToFile(f, false);
    bool ok = f.n == 0;
    n.SaveToFile(f, true);
    ok = ok && f.n == 3 && f.buf[0] == 2 && f.buf[1] == 1 && f.buf[2] == 1;
    std::printf(ok ? "00a76950_TEST PASS\n" : "00a76950_TEST FAIL\n");
    return ok ? 0 : 1;
}