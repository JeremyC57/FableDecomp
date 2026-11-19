// Behaviour test for GetMarkerFileName (0x009C33A0), a portable model: the destination path is split into drive, directory, name and
// extension (the extension is the part from the last '.' of the file name), every '.' in the extension becomes '_', and ".mkf" is
// appended: C:\d\t.big -> C:\d\t_big.mkf; a name without an extension just gains ".mkf"; a dot in a directory is not an extension.
#include <stdio.h>
#include <string>
static std::string GetMarkerFileName(const std::string& dst_name)
{
    size_t slash = dst_name.find_last_of("\\/");
    size_t name_start = slash == std::string::npos ? (dst_name.size() > 1 && dst_name[1] == ':' ? 2 : 0) : slash + 1;
    std::string marker_file_drive = name_start >= 2 && dst_name[1] == ':' ? dst_name.substr(0, 2) : "";
    std::string marker_file_dir = dst_name.substr(marker_file_drive.size(), name_start - marker_file_drive.size());
    std::string file = dst_name.substr(name_start);
    size_t dot = file.find_last_of('.');
    std::string marker_file_name = dot == std::string::npos ? file : file.substr(0, dot);
    std::string marker_file_ext = dot == std::string::npos ? "" : file.substr(dot);
    for (size_t i = 0; i < marker_file_ext.size(); ++i) if (marker_file_ext[i] == '.') marker_file_ext[i] = '_';
    return marker_file_drive + marker_file_dir + marker_file_name + marker_file_ext + ".mkf";
}
int main()
{
    int bad = 0;
    if (GetMarkerFileName("C:\\d\\t.big") != "C:\\d\\t_big.mkf") ++bad;
    if (GetMarkerFileName("C:\\data\\levels\\a.b.wad") != "C:\\data\\levels\\a.b_wad.mkf") ++bad;   // only the last dot starts the extension
    if (GetMarkerFileName("D:\\a.b\\c") != "D:\\a.b\\c.mkf") ++bad;                               // dot in a directory: no extension
    if (GetMarkerFileName("x") != "x.mkf") ++bad;
    printf(bad ? "009c33a0_TEST FAIL\n" : "009c33a0_TEST PASS\n");
    return bad ? 1 : 0;
}
