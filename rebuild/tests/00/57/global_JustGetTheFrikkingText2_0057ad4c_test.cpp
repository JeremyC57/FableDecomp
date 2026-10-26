// Behaviour test for 0x0057AD4C JustGetTheFrikkingText2: a tag whose CRC the data bank knows returns that entry's
// text (and releases the counted entry exactly once); an unknown tag returns the tag itself widened.
#include <cstdio>
#include <cstring>

static int g_destroyed = 0;
class CWideString { public: wchar_t text[32]; };
class CCharString { public:
    const char* s;
    unsigned int GetCRC() const { unsigned int h = 0; for (const char* p = s; *p; ++p) h = h * 31 + (unsigned char)*p; return h; }
    CWideString ToWideString() const { CWideString w; std::memset(&w, 0, sizeof w); for (int i = 0; s[i] && i < 31; ++i) w.text[i] = (wchar_t)s[i]; return w; }
};
struct CTextEntry {
    int RefCount; const wchar_t* Text;
    void Release() { if (--RefCount == 0) ++g_destroyed; }
    CWideString GetText() const { CWideString w; std::memset(&w, 0, sizeof w); for (int i = 0; Text[i] && i < 31; ++i) w.text[i] = Text[i]; return w; }
};
struct CCountedText {
    CTextEntry* Data;
    explicit CCountedText(CTextEntry* d) : Data(d) { if (Data) ++Data->RefCount; }
    CCountedText(const CCountedText& o) : Data(o.Data) { if (Data) ++Data->RefCount; }
    ~CCountedText() { if (Data) { Data->Release(); Data = 0; } }
    CTextEntry* operator->() const { return Data; }
};
static CTextEntry g_entry;
struct CDataBank {
    unsigned int knownCrc;
    unsigned int LookupValue(unsigned int crc) { return crc == knownCrc ? 77u : 0u; }
    CCountedText GetPTextEntry(unsigned int id, bool) const { return CCountedText(id == 77u ? &g_entry : 0); }
};
struct CWorldGlobal { CDataBank* PDataBank; CDataBank* GetPDataBank() const { return PDataBank; } };
static CDataBank g_bank;
static CWorldGlobal g_worldObj = { &g_bank };
static CWorldGlobal* g_pWorld = &g_worldObj;

static CWideString JustGetTheFrikkingText2(const CCharString& textId)
{
    CDataBank* bank = g_pWorld->PDataBank;
    unsigned int id = bank->LookupValue(textId.GetCRC());
    if (!id)
        return textId.ToWideString();
    return g_pWorld->GetPDataBank()->GetPTextEntry(id, true)->GetText();
}

int main()
{
    CCharString known = { "TEXT_HELLO" }, unknown = { "TEXT_MISSING" };
    g_bank.knownCrc = known.GetCRC();
    g_entry.RefCount = 0; g_entry.Text = L"Hello";
    CWideString a = JustGetTheFrikkingText2(known);
    CWideString b = JustGetTheFrikkingText2(unknown);
    bool ok = a.text[0] == L'H' && a.text[4] == L'o' && a.text[5] == 0 && g_destroyed == 1 && g_entry.RefCount == 0 &&
              b.text[0] == L'T' && b.text[11] == L'G' && b.text[12] == 0 && g_destroyed == 1;
    std::printf(ok ? "0057ad4c_TEST PASS\n" : "0057ad4c_TEST FAIL\n");
    return ok ? 0 : 1;
}