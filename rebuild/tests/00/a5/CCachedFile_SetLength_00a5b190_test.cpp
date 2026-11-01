// Behaviour test for CCachedFile::SetLength (0x00A5B190): a cached file is read-only, so the call always throws a CBBBFileException of type 8
// whose message is "" + "File is not open for writing" (the message is handed to the exception constructor by value).
#include <cstdio>
#include <cstring>
#include <cstdlib>
static int g_live;
class CCharString {
public:
    char* Text;
    CCharString(const char* text, long length) { (void)length; Text = (char*)std::malloc(std::strlen(text) + 1); std::strcpy(Text, text); ++g_live; }
    CCharString(const CCharString& other) { Text = (char*)std::malloc(std::strlen(other.Text) + 1); std::strcpy(Text, other.Text); ++g_live; }
    ~CCharString() { std::free(Text); --g_live; }
    operator const char*() const { return Text; }
};
CCharString operator+(const CCharString& left, const char* right)
{
    char buffer[128];
    std::strcpy(buffer, left.Text);
    std::strcat(buffer, right);
    return CCharString(buffer, -1);
}
static char g_reported[128];
class CBBBFileException {
public:
    enum EFileExceptionType { NotOpenForWrite = 8 };
    EFileExceptionType Type;
    CBBBFileException(CCharString message, EFileExceptionType type) : Type(type) { std::strcpy(g_reported, (const char*)message); }
};
class CASuspendableProcess;
class CCachedFile {
public:
    void SetLength(long length);
};
void CCachedFile::SetLength(long length)
{
    CCharString message = CCharString("", -1) + "File is not open for writing";
    throw CBBBFileException(message, CBBBFileException::NotOpenForWrite);
}
int main()
{
    CCachedFile cached;
    CCachedFile* file = &cached;
    char buffer[4] = { 0, 0, 0, 0 };
    (void)buffer;
    int caught = 0;
    try {
        file->SetLength(64);
    } catch (const CBBBFileException& e) {
        caught = (e.Type == 8);
    }
    if (!caught || std::strcmp(g_reported, "File is not open for writing") != 0) { std::printf("00a5b190_TEST FAIL\n"); return 1; }
    std::printf("00a5b190_TEST PASS\n");
    return 0;
}