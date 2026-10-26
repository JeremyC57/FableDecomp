// Behaviour test for CDefStringTable::SaveBinary (0x009D4CE0): a locked table is not written; otherwise it is written
// (write access, flags 4) and, when locking, the map is cleared (erase called on the root, header reset) and locked.
#include <cstdio>
class CWideString { public: const char* path; };
enum FileAccessType { FILE_ACCESS_READ = 1, FILE_ACCESS_WRITE = 4 };
static int g_opens = 0, g_closes = 0, g_writes = 0, g_erases = 0, g_access = 0; static unsigned long g_flags = 0;
class CDiskFileWin32 { public:
    bool Opened;
    CDiskFileWin32() { Opened = false; }
    ~CDiskFileWin32() { Close(); }
    void Open(const CWideString&, FileAccessType access, unsigned long flags) { ++g_opens; g_access = access; g_flags = flags; Opened = true; }
    void Close() { if (Opened) ++g_closes; Opened = false; }
};
struct CFileDataOutputStream { CDiskFileWin32* file; CFileDataOutputStream(CDiskFileWin32* f) : file(f) {} ~CFileDataOutputStream() {} };
struct SRbTreeNode { long Color; SRbTreeNode* Parent; SRbTreeNode* Left; SRbTreeNode* Right; };
static SRbTreeNode* g_erasedRoot = 0;
struct CStringMap {
    SRbTreeNode* Header; unsigned long NodeCount;
    void Erase(SRbTreeNode* node) { ++g_erases; g_erasedRoot = node; }
    void clear()
    {
        if (NodeCount != 0) {
            Erase(Header->Parent);
            Header->Left = Header;
            Header->Parent = 0;
            Header->Right = Header;
            NodeCount = 0;
        }
    }
};
class CDefStringTable {
public:
    CStringMap StringMap; CWideString TableFilename; bool Locked;
    void WriteTable(CFileDataOutputStream* s) { if (s->file->Opened) ++g_writes; }
    void SaveBinary(bool lock);
};
void CDefStringTable::SaveBinary(bool lock)
{
    if (!Locked) {
        CDiskFileWin32 file;
        file.Open(TableFilename, FILE_ACCESS_WRITE, 4);
        CFileDataOutputStream stream(&file);
        WriteTable(&stream);
        if (lock) {
            StringMap.clear();
            Locked = true;
        }
    }
}
int main()
{
    SRbTreeNode header, root, left, right;
    header.Parent = &root; header.Left = &left; header.Right = &right;
    CDefStringTable t; t.StringMap.Header = &header; t.StringMap.NodeCount = 3; t.Locked = false; t.TableFilename.path = "x";
    int bad = 0;
    t.SaveBinary(false);
    if (g_writes != 1 || g_closes != 1 || g_erases || t.Locked || t.StringMap.NodeCount != 3) bad = 1;
    if (g_access != FILE_ACCESS_WRITE || g_flags != 4) bad = 2;
    t.SaveBinary(true);
    if (g_writes != 2 || g_erases != 1 || g_erasedRoot != &root || !t.Locked) bad = 3;
    if (header.Parent != 0 || header.Left != &header || header.Right != &header || t.StringMap.NodeCount) bad = 4;
    t.SaveBinary(true);
    if (g_opens != 2 || g_writes != 2) bad = 5;   // locked: nothing written
    if (bad) { std::printf("009d4ce0_TEST FAIL\n"); return 1; }
    std::printf("009d4ce0_TEST PASS\n");
    return 0;
}