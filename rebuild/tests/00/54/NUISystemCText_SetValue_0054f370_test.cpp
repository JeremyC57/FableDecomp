// Behaviour test for 0x0054F370 NUISystem::CText::SetValue(const CCharString&, bool, bool): with lookUpTag the text's CRC is turned into a game-text tag through the main game component's bank, else the new front end's, else 0; then the (tag, wide text, keepSize) overload runs
#include <cstdio>
#include <cstring>
unsigned long Calc(unsigned long seed, const void* data, unsigned long length)
{
    const unsigned char* p = (const unsigned char*)data;
    unsigned long i;
    for (i = 0; i < length; ++i)
        seed = seed * 31 + p[i];
    return seed;
}
struct CCharStringData {
    char* PData;
    unsigned long StringLength;
};
class CWideString;
class CCharString {
public:
    CCharStringData* PStringData;
    CWideString ToWideString() const;
    unsigned long GetLength() const { return PStringData == 0 ? 0 : PStringData->StringLength; }
    const char* GetString() const { return PStringData == 0 ? "" : PStringData->PData; }
    unsigned long GetCRC() const { return Calc(0, GetString(), GetLength()); }
};
class CWideString {
public:
    void* PStringData;
    CCharString ConvertToCharString() const { CCharString s; s.PStringData = (CCharStringData*)PStringData; return s; }
};
CWideString CCharString::ToWideString() const { CWideString w; w.PStringData = PStringData; return w; }
namespace NGameText {
class CDataBank {
public:
    unsigned long Salt;
    unsigned long GetTagFromCRC(unsigned long crc) const { return crc ^ Salt; }
};
}
struct CMainGameComponent {
    NGameText::CDataBank* PGameTextDataBank;
    NGameText::CDataBank* GetGameTextDataBank() const { return PGameTextDataBank; }
};
struct CNewFrontendGameComponent {
    NGameText::CDataBank* PGameTextDataBank;
    NGameText::CDataBank* GetGameTextDataBank() const { return PGameTextDataBank; }
};
CMainGameComponent* g_pMainGameComponent = 0;
CNewFrontendGameComponent* g_pNewFrontendGameComponent = 0;
class CText {
public:
    virtual void SetValue(const CCharString& text, bool lookUpTag, bool keepSize);
    virtual void SetValue(const CWideString& text, bool lookUpTag, bool keepSize);
    virtual void SetValue(unsigned long tag, const CWideString& text, bool keepSize);
    virtual ~CText() { }
    unsigned long LastTag;
    void* LastText;
    bool LastKeepSize;
};
void CText::SetValue(unsigned long tag, const CWideString& text, bool keepSize)
{
    LastTag = tag; LastText = text.PStringData; LastKeepSize = keepSize;
}
void CText::SetValue(const CWideString&, bool, bool) { LastTag = 0xdeadu; }
void CText::SetValue(const CCharString& text, bool lookUpTag, bool keepSize)
{
    unsigned long tag = 0;
    if (lookUpTag) {
        if (g_pMainGameComponent) {
            tag = g_pMainGameComponent->GetGameTextDataBank()->GetTagFromCRC(text.GetCRC());
        }
        else if (g_pNewFrontendGameComponent) {
            tag = g_pNewFrontendGameComponent->GetGameTextDataBank()->GetTagFromCRC(text.GetCRC());
        }
    }
    SetValue(tag, text.ToWideString(), keepSize);
}
int main()
{
    char hello[] = "hello";
    CCharStringData data; data.PData = hello; data.StringLength = 5;
    CCharString text; text.PStringData = &data;
    unsigned long crc = Calc(0, "hello", 5);
    NGameText::CDataBank mainBank; mainBank.Salt = 0x100;
    NGameText::CDataBank frontBank; frontBank.Salt = 0x200;
    CMainGameComponent mainGame; mainGame.PGameTextDataBank = &mainBank;
    CNewFrontendGameComponent frontEnd; frontEnd.PGameTextDataBank = &frontBank;
    CText t;
    CText* p = &t;
    p->SetValue(text, false, true);
    if (t.LastTag != 0 || t.LastText != &data || !t.LastKeepSize) { std::printf("0054f370_TEST FAIL\n"); return 1; }
    p->SetValue(text, true, false);
    if (t.LastTag != 0 || t.LastKeepSize) { std::printf("0054f370_TEST FAIL\n"); return 1; }
    g_pNewFrontendGameComponent = &frontEnd;
    p->SetValue(text, true, false);
    if (t.LastTag != (crc ^ 0x200)) { std::printf("0054f370_TEST FAIL\n"); return 1; }
    g_pMainGameComponent = &mainGame;
    p->SetValue(text, true, true);
    if (t.LastTag != (crc ^ 0x100) || t.LastText != &data) { std::printf("0054f370_TEST FAIL\n"); return 1; }
    text.PStringData = 0;
    p->SetValue(text, true, true);
    if (t.LastTag != (Calc(0, "", 0) ^ 0x100) || t.LastText != 0) { std::printf("0054f370_TEST FAIL\n"); return 1; }
    std::printf("0054f370_TEST PASS\n");
    return 0;
}