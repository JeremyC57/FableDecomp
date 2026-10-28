// Behaviour test for CSubtitleRenderer::GetTextFromTextTag (0x0048F6BD): a text tag gives its text; a group tag gives
// its members in turn (index advances, "entries remain" until the last); an unknown or exhausted tag copies the tag.
#include <cstdio>
#include <cstring>

class CWideString { public:
    char buf[32];
    CWideString() { buf[0] = 0; }
    CWideString(const char* t) { std::strcpy(buf, t); }
    const CWideString& operator=(const CWideString& o) { std::strcpy(buf, o.buf); return *this; }
};
struct CArray { long items[4]; unsigned long n; unsigned long size() const { return n; } };
struct CGroupEntry { long RefCount; CArray Entries; };
struct CTextEntry { long RefCount; const char* text; CWideString GetText() const { return CWideString(text); } };
static long g_released = 0;
template <class T> struct CPtr {
    T* Data;
    CPtr() : Data(0) { }
    CPtr(const CPtr& o) : Data(o.Data) { if (Data) ++Data->RefCount; }
    ~CPtr() { if (Data) { if (--Data->RefCount == 0) ++g_released; Data = 0; } }
    T* operator->() const { return Data; }
};
static CTextEntry g_texts[8];
static CGroupEntry g_group;
struct CDataBank {
    unsigned long LookupValue(const char* name) const
    {
        if (!std::strcmp(name, "TEXT_A")) return 1;
        if (!std::strcmp(name, "TEXT_B")) return 2;
        if (!std::strcmp(name, "GROUP")) return 5;
        return 0;
    }
    unsigned long GetEntryDataType(unsigned long id) const { return id == 5 ? 1 : 0; }
    CPtr<CGroupEntry> GetPGroupEntry(unsigned long, bool) const { CPtr<CGroupEntry> p; p.Data = &g_group; ++g_group.RefCount; return p; }
    CPtr<CTextEntry> GetPTextEntry(unsigned long id, bool) const { CPtr<CTextEntry> p; p.Data = &g_texts[id]; ++g_texts[id].RefCount; return p; }
};
static CDataBank g_bank;

class CSubtitleRenderer { public:
    bool        TextGroupHasEntriesRemaining;
    long        TextGroupIndex;
    CWideString TextGroupTag;
    bool GetTextFromTextTag(const CWideString& tag, CWideString& text);
};

bool CSubtitleRenderer::GetTextFromTextTag(const CWideString& tag, CWideString& text)
{
    CDataBank* bank = &g_bank;
    unsigned long id = bank->LookupValue(tag.buf);
    if (id > 0) {
        if (bank->GetEntryDataType(id)) {
            const CArray* entries = &bank->GetPGroupEntry(id, true)->Entries;
            TextGroupTag = tag;
            ++TextGroupIndex;
            TextGroupHasEntriesRemaining = false;
            if ((unsigned long)TextGroupIndex >= entries->size())
                goto fail;
            if (TextGroupIndex < (long)entries->size() - 1)
                TextGroupHasEntriesRemaining = true;
            const long* items = entries->items;
            text = bank->GetPTextEntry(items[TextGroupIndex], true)->GetText();
        } else
            text = bank->GetPTextEntry(id, true)->GetText();
        return true;
    }
fail:
    text = tag;
    return false;
}

int main()
{
    int ok = 1;
    g_texts[1].text = "Hello"; g_texts[2].text = "World";
    g_texts[1].RefCount = 1; g_texts[2].RefCount = 1; g_group.RefCount = 1;
    g_group.Entries.items[0] = 2; g_group.Entries.items[1] = 1; g_group.Entries.n = 2;
    CSubtitleRenderer r; r.TextGroupHasEntriesRemaining = false; r.TextGroupIndex = -1;
    CWideString out;
    ok &= r.GetTextFromTextTag(CWideString("TEXT_A"), out) && !std::strcmp(out.buf, "Hello");
    ok &= !r.GetTextFromTextTag(CWideString("NOPE"), out) && !std::strcmp(out.buf, "NOPE");
    ok &= r.GetTextFromTextTag(CWideString("GROUP"), out) && !std::strcmp(out.buf, "World");
    ok &= r.TextGroupIndex == 0 && r.TextGroupHasEntriesRemaining && !std::strcmp(r.TextGroupTag.buf, "GROUP");
    ok &= r.GetTextFromTextTag(CWideString("GROUP"), out) && !std::strcmp(out.buf, "Hello");
    ok &= r.TextGroupIndex == 1 && !r.TextGroupHasEntriesRemaining;
    ok &= !r.GetTextFromTextTag(CWideString("GROUP"), out) && !std::strcmp(out.buf, "GROUP");
    ok &= r.TextGroupIndex == 2 && !r.TextGroupHasEntriesRemaining;
    ok &= g_texts[1].RefCount == 1 && g_texts[2].RefCount == 1 && g_group.RefCount == 1 && g_released == 0;
    std::printf(ok ? "0048f6bd_TEST PASS\n" : "0048f6bd_TEST FAIL\n");
    return ok ? 0 : 1;
}