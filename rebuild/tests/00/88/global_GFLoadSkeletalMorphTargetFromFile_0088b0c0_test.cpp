#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __stdcall
#define __int64 long long
#define FABLE_RETAIL_PASS23_HOST_TEST_ABI
#define FABLE_PORTABLE
#include <strings.h>
#define _stricmp strcasecmp
#endif
// 0088b0c0 GFLoadSkeletalMorphTargetFromFile
// Flags: /Gr
// Leaf: ?GFLoadSkeletalMorphTargetFromFile@@YIXAAVCAFile@@AAVC3DSkeletalMorphTarget@@@Z
#define FABLE_RETAIL_SKELETAL_MORPH_FILE_PASS30
#define FABLE_RETAIL_CSTRINGPARSER
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CAFILE_SPLITPATH_WIDE
#define FABLE_RETAIL_CARRAY_ORIGINAL_USER_SPECIAL_MEMBERS_PASS22
#define FABLE_RETAIL_CARRAY_PUSHBACK
#define FABLE_RETAIL_STLP_FILL_CHAR
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_STLP_LIST_DTOR_DECL_ONLY
#include "engine/CStringParser.h"
#include "engine/CAFile.h"
#include "engine/C3DSkeletalMorph.h"
#include "fable_assert.h"

void FABLE_FASTCALL GFLoadSkeletalMorphTargetFromFile(CAFile& file, C3DSkeletalMorphTarget& skeletal_morph_target)
{
    std::vector<char> bone_data(file.GetLength() + 1);
    file.SetPosition(0);
    file.Read(&bone_data[0], file.GetLength(), 0);
    bone_data[file.GetLength()] = 0;

    CStringParser parser(&bone_data[0]);
    if (!parser.SkipPastString("#Start_Bone_data\r\n")) {
        FABLE_RETAIL_ASSERT(!"Invalid bone file.");
        return;
    }
    CArray<C3DSkeletalBoneMorph> morph_list;
    while (true) {
        if (parser.PeekNextItemType() == PARSED_ITEM_SYMBOL)
            break;

        CCharString bone_name = parser.ReadAsStringUntilString(": ");
        if (!parser.SkipPastNextItemIfIs(": ")) {
            FABLE_RETAIL_ASSERT(!"Invalid bone file.");
            return;
        }
        C3DVector scaling;
        scaling.X = parser.ReadAsFloat();
        if (!parser.SkipPastNextItemIfIs(", ")) {
            FABLE_RETAIL_ASSERT(!"Invalid bone file.");
            return;
        }
        scaling.Y = parser.ReadAsFloat();
        if (!parser.SkipPastNextItemIfIs(", ")) {
            FABLE_RETAIL_ASSERT(!"Invalid bone file.");
            return;
        }
        scaling.Z = parser.ReadAsFloat();
        if (!parser.SkipPastNextItemIfIs(";\r\n")) {
            FABLE_RETAIL_ASSERT(!"Invalid bone file.");
            return;
        }
        morph_list.push_back(C3DSkeletalBoneMorph(bone_name, scaling));
    }
    if (!parser.SkipPastNextItemIfIs("#End_bone_data\r\n")) {
        FABLE_RETAIL_ASSERT(!"Invalid bone file.");
        return;
    }
    CWideString name;
    CAFile::SplitPath(file.GetPathname(), 0, 0, &name, 0);
    skeletal_morph_target.Initialise(name.ToCharString(), morph_list);
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "engine/CFileInformation.h"

namespace {
int failed, completed, reads, seeks, lengths, paths, splits, float_reads, bone_ctors;
char output_name[90], names[16][40];
float components[16][3];
unsigned int count;
struct TextEntry { const void* owner; char value[256]; };
TextEntry texts[128];
void require(bool condition) { if (!condition) ++failed; }
const char* get(const void* p)
{
    for (int i = 0; i < 128; ++i) if (texts[i].owner == p) return texts[i].value;
    require(false);
    return "";
}
void set(const void* p, const char* s, long length = -1)
{
    int i;
    for (i = 0; i < 128 && texts[i].owner != p; ++i) { }
    if (i == 128) for (i = 0; i < 128 && texts[i].owner; ++i) { }
    if (i == 128) { require(false); return; }
    if (length < 0) length = (long)strlen(s);
    if (length > 255) { require(false); return; }
    texts[i].owner = p;
    memmove(texts[i].value, s, length);
    texts[i].value[length] = 0;
}
void remove_text(const void* p)
{
    for (int i = 0; i < 128; ++i) if (texts[i].owner == p) { texts[i].owner = 0; return; }
    require(false);
}
const char* file_bytes;
const char* file_path;
}

CBaseClass::CBaseClass() { }
CBaseClass::~CBaseClass() { }
CBaseClassNonCopyable::CBaseClassNonCopyable() { }
CBaseClassNonCopyable::~CBaseClassNonCopyable() { }
CCharString::CCharString() : PStringData(0) { set(this, ""); }
CCharString::CCharString(const char* s, long n) : PStringData(0) { set(this, s, n); }
CCharString::CCharString(const CCharString& s) : PStringData(0) { set(this, get(&s)); }
CCharString::~CCharString() { remove_text(this); }
CWideString::CWideString() : PStringData(0) { set(this, ""); }
CWideString::CWideString(const CWideString& s) : PStringData(0) { set(this, get(&s)); }
CWideString::~CWideString() { remove_text(this); }
CCharString CWideString::ToCharString() const { return CCharString(get(this)); }
CFileInformation::CFileInformation() { }
bool CAFile::IsIOFinished() const { return true; }
long CAFile::GetAlignment() const { return 1; }
CFileInformation CAFile::GetFileInformation() { return CFileInformation(); }
void FABLE_FASTCALL CAFile::SplitPath(const CWideString& path, CWideString* drive, CWideString* directory,
                                      CWideString* name, CWideString* extension)
{
    ++splits;
    require(!drive && !directory && name && !extension && strcmp(get(&path), file_path) == 0);
    const char* s = get(&path);
    const char* slash = strrchr(s, '/');
    if (slash) s = slash + 1;
    const char* dot = strrchr(s, '.');
    set(name, s, dot ? (long)(dot - s) : -1);
}

class MemoryFile : public CAFile {
public:
    virtual void Open(const CWideString&, FileAccessType, unsigned long) { require(false); }
    virtual void Close() { require(false); }
    virtual void Read(void* dest, long bytes, CASuspendableProcess* process)
    {
        ++reads;
        require(seeks == 1 && lengths == 2 && bytes == (long)strlen(file_bytes) && process == 0);
        memcpy(dest, file_bytes, bytes);
    }
    virtual void Write(const void*, long, CASuspendableProcess*) { require(false); }
    virtual void SetPosition(long p) { ++seeks; require(p == 0 && lengths == 1); }
    virtual void MovePosition(long) { require(false); }
    virtual long GetPosition() const { require(false); return 0; }
    virtual void SetLength(long) { require(false); }
    virtual long GetLength() const { ++lengths; return (long)strlen(file_bytes); }
    virtual bool IsOpen() const { return true; }
    virtual CWideString GetPathname() const { ++paths; CWideString s; set(&s, file_path); return s; }
    virtual CAFile* Clone() const { require(false); return 0; }
};
namespace std {
template<class T, class A> list<T,A>::~list() { }
template<> _List_base<CStringParserCommentBoundaries, allocator<CStringParserCommentBoundaries> >::~_List_base() {
    clear();
    _M_node.deallocate(_M_node._M_data,1);
}
template<> vector<C3DSkeletalBoneMorph>::~vector() { _Destroy(this->begin(), this->end()); }
}
void CStringParser::Init(const char* text, void (FABLE_FASTCALL* callback)(const CCharString&),
                         const CCharString& filename, long base_line)
{
    require(lengths == 3 && reads == 1 && callback == 0 && get(&filename)[0] == 0 && base_line == 0);
    require(strcmp(text, file_bytes) == 0);
    String = text;
    CursorPos.StringPos = 0;
}
bool CStringParser::SkipPastString(const CCharString& item)
{
    const char* s = get(&item);
    require(strcmp(s, "#Start_Bone_data\r\n") == 0);
    const char* found = strstr(String + CursorPos.StringPos, s);
    if (!found) return false;
    CursorPos.StringPos = (unsigned long)(found - String + strlen(s));
    return true;
}
EParsedItemType CStringParser::PeekNextItemType()
{
    char c = String[CursorPos.StringPos];
    return c == '#' || c == 0 ? PARSED_ITEM_SYMBOL : PARSED_ITEM_STRING;
}
CCharString CStringParser::ReadAsStringUntilString(const CCharString& delimiter)
{
    require(strcmp(get(&delimiter), ": ") == 0);
    const char* first = String + CursorPos.StringPos;
    const char* last = strstr(first, ": ");
    long n = last ? (long)(last - first) : (long)strlen(first);
    CursorPos.StringPos += n;
    return CCharString(first, n);
}
bool CStringParser::SkipPastNextItemIfIs(const CCharString& item)
{
    const char* s = get(&item);
    unsigned long n = (unsigned long)strlen(s);
    if (strncmp(String + CursorPos.StringPos, s, n) != 0) return false;
    CursorPos.StringPos += n;
    return true;
}
float CStringParser::ReadAsFloat()
{
    ++float_reads;
    char* end = 0;
    double value = strtod(String + CursorPos.StringPos, &end);
    CursorPos.StringPos = (unsigned long)(end - String);
    return (float)value;
}
C3DSkeletalBoneMorph::C3DSkeletalBoneMorph(const CCharString& n, const C3DVector& s)
    : BoneName(n), BoneNameCRC(0), Scaling(s) { ++bone_ctors; }
void C3DSkeletalMorphTarget::Initialise(const CCharString& n, const CArray<C3DSkeletalBoneMorph>& bones)
{
    ++completed;
    strcpy(output_name, get(&n));
    count = (unsigned int)bones.size();
    require(count <= 16);
    for (unsigned int i = 0; i < count && i < 16; ++i) {
        strcpy(names[i],get(&bones[i].GetName()));
        const C3DVector& s = bones[i].GetScaling();
        components[i][0] = s.X; components[i][1] = s.Y; components[i][2] = s.Z;
    }
}

namespace {
void check(const char* input, const char* path, const char* expected_name, int bones, int floats, int created)
{
    completed = reads = seeks = lengths = paths = splits = float_reads = bone_ctors = 0;
    file_path = path;
    file_bytes = input;
    { MemoryFile f; C3DSkeletalMorphTarget target; GFLoadSkeletalMorphTargetFromFile(f, target); }
    require(reads == 1 && seeks == 1 && lengths == 3 && bone_ctors == created && float_reads == floats);
    require(completed == (expected_name != 0) && paths == completed && splits == completed);
    if (expected_name) require(strcmp(output_name,expected_name) == 0 && count == (unsigned int)bones);
    for (int i = 0; i < 128; ++i) require(texts[i].owner == 0);
}
}
int main()
{
    const char* path = "data/hero.bone";
    const char* start = "#Start_Bone_data\r\n";
    const char* finish = "#End_bone_data\r\n";
    char input[1024];
    check("",path,0,0,0,0);
    check("#start_Bone_data\r\n",path,0,0,0,0);
    check("#Start_Bone_data\n",path,0,0,0,0);
    const char* invalid[] = {"Arm 1", "Arm: 1; ", "Arm: 1, 2; ", "Arm: 1, 2, 3;\n", "#End_Bone_data\r\n"};
    const int reads_before_failure[] = {0,1,2,3,0};
    for (int prior = 0; prior <= 2; ++prior) for (int j = 0; j < 5; ++j) {
        strcpy(input,start);
        for (int i = 0; i < prior; ++i) strcat(input,"Arm: 1, -2, 0.5;\r\n");
        strcat(input,invalid[j]);
        check(input,path,0,0,prior*3 + reads_before_failure[j],prior);
    }
    for (int n = 0; n <= 12; ++n) for (int variant = 0; variant < 2; ++variant) {
        strcpy(input, variant ? "comment\r\n" : "");
        strcat(input,start);
        for (int i = 0; i < n; ++i)
            strcat(input,i%2 ? "Leg: 0, 2.5, -8;\r\n" : "Arm: 1, -2, 0.5;\r\n");
        strcat(input,finish);
        if (variant) strcat(input,"ignored trailing data");
        check(input,variant ? "a/b/morph.bin" : path,variant ? "morph" : "hero",n,n*3,n);
        for (int i = 0; i < n; ++i) {
            require(strcmp(names[i],i%2 ? "Leg" : "Arm") == 0);
            require(components[i][0] == (i%2 ? 0.0f : 1.0f));
            require(components[i][1] == (i%2 ? 2.5f : -2.0f));
            require(components[i][2] == (i%2 ? -8.0f : 0.5f));
        }
    }
    if (failed) { printf("failures %d\n",failed); return 1; }
    puts("0088b0c0_TEST PASS");
    return 0;
}