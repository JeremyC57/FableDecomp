// Behaviour test for 0x009C53A0: CFileInstaller::ProcessReads retires a fully read current file (closing its handle), then drains the to-copy list until a copy starts: files needing a copy become active (read started, queued to finish), others are recorded as completed; Completed is set when both lists empty; the current file then gets a read step.
#include <cstdio>
#include <list>
#include <set>
#include <string>
struct CFileInfo { std::string Source; bool Needed; };
struct CActiveFile { const CFileInfo* Info; int Handle; unsigned int Read, Size; int Reads; };
class CFileInstaller { public:
    std::list<const CFileInfo*> FilesToCopy, FilesToFinish;
    std::list<CActiveFile> ActiveFiles;
    std::set<std::string> CompletedFiles;
    CActiveFile* PCurrentReadFile;
    int NoActiveReads, NoActiveWrites, Closed;
    bool Completed;
    CFileInstaller() : PCurrentReadFile(0), NoActiveReads(0), NoActiveWrites(0), Closed(0), Completed(false) { }
    bool IsFileCopyNecessary(const CFileInfo* info) { return info->Needed; }
    void StartRead(CActiveFile* file) { file->Handle = 7; }
    void ProcessRead(CActiveFile* file) { ++file->Reads; }
    void ProcessReads()
    {
        if (NoActiveWrites + NoActiveReads >= 1)
            return;
        CActiveFile* current = PCurrentReadFile;
        if (current && current->Read >= current->Size) {
            if (current->Handle != -1) {
                ++Closed;
                current->Handle = -1;
            }
            PCurrentReadFile = 0;
        }
        while (PCurrentReadFile == 0 && !FilesToCopy.empty()) {
            const CFileInfo* info = FilesToCopy.front();
            if (IsFileCopyNecessary(info)) {
                CActiveFile file = { info, -1, 0, 10, 0 };
                ActiveFiles.push_back(file);
                StartRead(&ActiveFiles.back());
                PCurrentReadFile = &ActiveFiles.back();
                FilesToFinish.push_back(info);
            } else
                CompletedFiles.insert(info->Source);
            FilesToCopy.pop_front();
            if (FilesToFinish.empty() && FilesToCopy.empty())
                Completed = true;
        }
        if (PCurrentReadFile)
            ProcessRead(PCurrentReadFile);
    }
};
int main()
{
    CFileInfo a = { "a", false }, b = { "b", true }, c = { "c", false };
    CFileInstaller inst;
    inst.FilesToCopy.push_back(&a);
    inst.FilesToCopy.push_back(&b);
    inst.FilesToCopy.push_back(&c);
    int ok = 1;
    inst.NoActiveReads = 1;
    inst.ProcessReads();
    ok &= inst.FilesToCopy.size() == 3;
    inst.NoActiveReads = 0;
    inst.ProcessReads();
    ok &= inst.CompletedFiles.count("a") == 1 && inst.FilesToCopy.size() == 1 && inst.PCurrentReadFile && inst.PCurrentReadFile->Info == &b;
    ok &= inst.PCurrentReadFile->Handle == 7 && inst.PCurrentReadFile->Reads == 1 && inst.FilesToFinish.size() == 1 && !inst.Completed;
    inst.PCurrentReadFile->Read = 10;
    inst.FilesToFinish.clear();
    inst.ProcessReads();
    ok &= inst.Closed == 1 && inst.PCurrentReadFile == 0 && inst.CompletedFiles.count("c") == 1 && inst.Completed && inst.FilesToCopy.empty();
    std::printf(ok ? "009c53a0_TEST PASS\n" : "009c53a0_TEST FAIL\n");
    return ok ? 0 : 1;
}