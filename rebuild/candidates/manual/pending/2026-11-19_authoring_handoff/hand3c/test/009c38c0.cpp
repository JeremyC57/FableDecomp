// Behaviour test for CFileInstaller::CActiveFile::OnWriteFinished (0x009C38C0), a portable model over a fake file system: nothing
// happens when no destination is open; otherwise the destination is trimmed to the output size and given the source's write time,
// both handles end closed (-1), and a wanted marker "<drive><dir><name><ext with '.' -> '_'>.mkf" is created; when it cannot be
// created the dirty-disc error is raised and the installer thread told to exit.
#include <stdio.h>
#include <string>
#include <map>
struct File { long size; unsigned long long time; };
struct FS { std::map<std::string, File> files; bool markerFails; };
static std::string MarkerName(const std::string& path)
{
    size_t slash = path.find_last_of("\\/"), dot = path.find_last_of('.');
    if (dot != std::string::npos && slash != std::string::npos && dot < slash) dot = std::string::npos;
    std::string base = dot == std::string::npos ? path : path.substr(0, dot);
    std::string ext = dot == std::string::npos ? "" : path.substr(dot);
    for (size_t i = 0; i < ext.size(); ++i) if (ext[i] == '.') ext[i] = '_';
    return base + ext + ".mkf";
}
struct Active { int src, dst; long outputSize; std::string srcPath, dstPath; bool marker; };
static void OnWriteFinished(Active& a, FS& fs, bool& dirtyDisc, bool& exitThread)
{
    if (a.dst == -1) return;
    a.dst = -1;
    File& d = fs.files[a.dstPath];
    d.size = a.outputSize;
    d.time = fs.files[a.srcPath].time;
    a.src = -1; a.dst = -1;
    if (a.marker) {
        if (fs.markerFails) { dirtyDisc = true; exitThread = exitThread || true; return; }
        File m = { 0, 0 }; fs.files[MarkerName(a.dstPath)] = m;
    }
}
int main()
{
    int bad = 0;
    FS fs; fs.markerFails = false;
    File src = { 100, 42 }, dst = { 4096, 7 }; fs.files["C:\\d\\s.big"] = src; fs.files["C:\\d\\t.big"] = dst;
    Active a = { 1, 2, 100, "C:\\d\\s.big", "C:\\d\\t.big", true };
    bool dirty = false, exitT = false;
    OnWriteFinished(a, fs, dirty, exitT);
    if (fs.files["C:\\d\\t.big"].size != 100 || fs.files["C:\\d\\t.big"].time != 42 || a.src != -1 || a.dst != -1) ++bad;
    if (!fs.files.count("C:\\d\\t_big.mkf") || dirty) ++bad;
    Active b = { 1, -1, 5, "x", "y", true };
    OnWriteFinished(b, fs, dirty, exitT);                                // no destination open: untouched
    if (b.src != 1 || fs.files.count("y")) ++bad;
    fs.markerFails = true;
    Active c = { 3, 4, 1, "C:\\d\\s.big", "C:\\e\\u.dat", true };
    OnWriteFinished(c, fs, dirty, exitT);
    if (!dirty || !exitT || fs.files.count("C:\\e\\u_dat.mkf")) ++bad;
    if (MarkerName("D:\\a.b\\c") != "D:\\a.b\\c.mkf") ++bad;            // a dot in the directory is not the extension
    printf(bad ? "009c38c0_TEST FAIL\n" : "009c38c0_TEST PASS\n");
    return bad ? 1 : 0;
}
