// Behaviour test for 0x004fb0a0 CWorldMap::GetThingFilenameFromWorldFilename(path): the path is split into drive, directory and
// name (the old extension dropped) and rebuilt with the extension L"gtng".
#include <cstdio>
#include <cstring>

struct StrModel {                                   // CWideString stand-in (narrow text is enough for the logic)
    char Text[128];
    StrModel() { Text[0] = 0; }
    StrModel(const char* t) { std::strcpy(Text, t); }
};

static void split_path(const StrModel& path, StrModel* drive, StrModel* directory, StrModel* name, StrModel* extension)
{
    const char* p = path.Text;
    const char* colon = std::strchr(p, ':');
    const char* start = p;
    if (colon) { if (drive) { std::memcpy(drive->Text, p, colon + 1 - p); drive->Text[colon + 1 - p] = 0; } start = colon + 1; }
    const char* slash = std::strrchr(start, '\\');
    const char* base = slash ? slash + 1 : start;
    if (directory) { std::memcpy(directory->Text, start, base - start); directory->Text[base - start] = 0; }
    const char* dot = std::strrchr(base, '.');
    const char* end = dot ? dot : base + std::strlen(base);
    if (name) { std::memcpy(name->Text, base, end - base); name->Text[end - base] = 0; }
    if (extension) std::strcpy(extension->Text, dot ? dot + 1 : "");
}

static StrModel make_path(const StrModel& drive, const StrModel& directory, const StrModel& name, const StrModel& extension)
{
    StrModel r;
    std::strcpy(r.Text, drive.Text); std::strcat(r.Text, directory.Text); std::strcat(r.Text, name.Text);
    if (extension.Text[0]) { std::strcat(r.Text, "."); std::strcat(r.Text, extension.Text); }
    return r;
}

static StrModel model(const StrModel& source)
{
    StrModel drive;
    StrModel name;
    StrModel directory;
    StrModel extension("gtng");
    split_path(source, &drive, &directory, &name, 0);
    StrModel path = make_path(drive, directory, name, extension);
    return path;
}

int main()
{
    StrModel a = model(StrModel("C:\\Fable\\Data\\Levels\\FinalAlbion\\FinalAlbion.wld"));
    if (std::strcmp(a.Text, "C:\\Fable\\Data\\Levels\\FinalAlbion\\FinalAlbion.gtng") != 0) { std::printf("004fb0a0_TEST FAIL\n"); return 1; }
    StrModel b = model(StrModel("FinalAlbion"));
    if (std::strcmp(b.Text, "FinalAlbion.gtng") != 0) { std::printf("004fb0a0_TEST FAIL\n"); return 1; }
    std::printf("004fb0a0_TEST PASS\n");
    return 0;
}