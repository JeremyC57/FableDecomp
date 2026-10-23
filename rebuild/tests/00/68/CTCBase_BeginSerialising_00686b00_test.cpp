#include <cstdio>
#include <cstring>
// Behaviour model of CTCBase::BeginSerialising: save appends "Start<name>;\r\n"; load finds Start<name>; and reads to End<name>;
static const char* ClassName = "CTCTest";
static int Begin(bool saving, char* data, char* body)
{
    char startTag[64], endTag[64];
    if (saving) {
        char header[64]; std::strcpy(header, "Start"); std::strcat(header, ClassName); std::strcat(header, ";\r\n");
        std::strcat(data, header);
        return 1;
    } else {
        std::strcpy(startTag, "Start"); std::strcat(startTag, ClassName); std::strcat(startTag, ";");
        std::strcpy(endTag, "End"); std::strcat(endTag, ClassName); std::strcat(endTag, ";");
        const char* s = std::strstr(data, startTag);
        if (s) {
            s += std::strlen(startTag);
            const char* e = std::strstr(s, endTag);
            unsigned int n = e ? (unsigned int)(e - s) : (unsigned int)std::strlen(s);
            std::memcpy(body, s, n); body[n] = 0;
            return 1;
        }
        return 0;
    }
}
int main()
{
    char data[256] = "Prefix;";
    char body[256];
    if (!Begin(true, data, body) || std::strcmp(data, "Prefix;StartCTCTest;\r\n") != 0) { std::printf("00686b00_TEST FAIL\n"); return 1; }
    std::strcat(data, "Value=1;EndCTCTest;");
    if (!Begin(false, data, body) || std::strcmp(body, "\r\nValue=1;") != 0) { std::printf("00686b00_TEST FAIL\n"); return 1; }
    char other[64] = "StartCOther;EndCOther;";
    if (Begin(false, other, body)) { std::printf("00686b00_TEST FAIL\n"); return 1; }
    std::printf("00686b00_TEST PASS\n");
    return 0;
}