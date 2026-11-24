#include <assert.h>
#include <stdio.h>
#include <string>
#include <vector>

class CCharString
{
public:
    CCharString(const char* value = "") : Text(value) {}
    unsigned long GetLength() const { return (unsigned long)Text.size(); }
    bool EndsWith(const CCharString& suffix) const
    {
        return Text.size() >= suffix.Text.size() &&
               Text.compare(Text.size() - suffix.Text.size(), suffix.Text.size(), suffix.Text) == 0;
    }
    CCharString GetSubString(unsigned long start, unsigned long length) const
    {
        return CCharString(Text.substr(start, length).c_str());
    }
    unsigned long GetCRC() const
    {
        unsigned long key = 5381;
        for (std::string::const_iterator i = Text.begin(); i != Text.end(); ++i)
            key = 33 * key + (unsigned char)*i;
        return key;
    }
    bool operator!=(const char* value) const { return Text != value; }
    CCharString& operator+=(const char* value) { Text += value; return *this; }
    CCharString operator+(const char* value) const { return CCharString((Text + value).c_str()); }

private:
    std::string Text;
};

namespace NGameText
{
class CDataBank
{
public:
    CDataBank() { Entries.push_back(Entry()); }
    void Add(const char* tag, unsigned long type)
    {
        Entry entry;
        entry.Key = CCharString(tag).GetCRC();
        entry.Type = type;
        Entries.push_back(entry);
    }
    unsigned long FindIndexBySymbolCRC(unsigned long key) const
    {
        for (unsigned long i = 1; i < Entries.size(); ++i)
            if (Entries[i].Key == key)
                return i;
        return 0;
    }
    unsigned long GetEntryDataType(unsigned long index) const
    {
        assert(index > 0 && index < Entries.size());
        return Entries[index].Type;
    }

private:
    struct Entry { unsigned long Key; unsigned long Type; };
    std::vector<Entry> Entries;
};
}

class CMainGameComponent
{
public:
    explicit CMainGameComponent(const NGameText::CDataBank* bank) : Bank(bank) {}
    static CMainGameComponent* Get() { return Current; }
    bool IsTextBankInitialised() const { return Bank != 0; }
    const NGameText::CDataBank& GetTextBank() { return *Bank; }
    static CMainGameComponent* Current;
private:
    const NGameText::CDataBank* Bank;
};
CMainGameComponent* CMainGameComponent::Current = 0;

class CNewFrontendGameComponent
{
public:
    explicit CNewFrontendGameComponent(const NGameText::CDataBank* bank) : Bank(bank) {}
    static CNewFrontendGameComponent* Get() { return Current; }
    bool IsTextBankInitialised() const { return Bank != 0; }
    const NGameText::CDataBank& GetTextBank() { return *Bank; }
    static CNewFrontendGameComponent* Current;
private:
    const NGameText::CDataBank* Bank;
};
CNewFrontendGameComponent* CNewFrontendGameComponent::Current = 0;

class CProgressDisplay
{
public:
    CProgressDisplay() : DisplayText(true), TextTagIndex(-1) {}
    void CalculateNextTextTag();

    bool DisplayText;
    long TextTagIndex;
    CCharString TextTag;
    std::vector<CCharString> TextTagsPC;

private:
    const NGameText::CDataBank* GetPTextBank() const;
};

// The selected definitions are reproduced verbatim between these markers.
// BEGIN SELECTED DEFINITIONS
inline const NGameText::CDataBank* CProgressDisplay::GetPTextBank() const
{
    if (CMainGameComponent::Get() && CMainGameComponent::Get()->IsTextBankInitialised())
        return &CMainGameComponent::Get()->GetTextBank();
    if (CNewFrontendGameComponent::Get() && CNewFrontendGameComponent::Get()->IsTextBankInitialised())
        return &CNewFrontendGameComponent::Get()->GetTextBank();
    return 0;
}

void CProgressDisplay::CalculateNextTextTag()
{
    if (DisplayText)
    {
        const NGameText::CDataBank* ptext_bank = GetPTextBank();
        if (ptext_bank)
        {
            bool moved_to_next_in_group = false;
            if (TextTagIndex != -1)
            {
                if (TextTag.EndsWith("_10"))
                    TextTag = TextTag.GetSubString(0, TextTag.GetLength() - 3) + "_20";
                else if (TextTag.EndsWith("_20"))
                    TextTag = TextTag.GetSubString(0, TextTag.GetLength() - 3) + "_30";
                else if (TextTag.EndsWith("_30"))
                    TextTag = TextTag.GetSubString(0, TextTag.GetLength() - 3) + "_40";
                else if (TextTag.EndsWith("_40"))
                    TextTag = TextTag.GetSubString(0, TextTag.GetLength() - 3) + "_50";
                else
                    TextTag = "";
                if (TextTag != "")
                    if (ptext_bank->FindIndexBySymbolCRC(TextTag.GetCRC()) != 0)
                        moved_to_next_in_group = true;
            }
            if (!moved_to_next_in_group)
            {
                const std::vector<CCharString>& text_tags = TextTagsPC;
                if (!text_tags.empty())
                {
                    ++TextTagIndex;
                    if (static_cast<unsigned long>(TextTagIndex) >= text_tags.size())
                        TextTagIndex = 0;
                    TextTag = text_tags[TextTagIndex];
                    unsigned long index = ptext_bank->FindIndexBySymbolCRC(TextTag.GetCRC());
                    if (index > 0 && ptext_bank->GetEntryDataType(index) == 1)
                        TextTag += "_10";
                }
            }
        }
    }
}
// END SELECTED DEFINITIONS

int main()
{
    NGameText::CDataBank full_bank;
    full_bank.Add("HINT", 1);
    full_bank.Add("HINT_10", 2);
    full_bank.Add("HINT_20", 2);
    full_bank.Add("HINT_30", 2);
    full_bank.Add("HINT_40", 2);
    full_bank.Add("HINT_50", 2);
    full_bank.Add("SINGLE", 2);
    NGameText::CDataBank partial_bank;
    partial_bank.Add("HINT", 1);
    partial_bank.Add("HINT_10", 2);
    partial_bank.Add("SINGLE", 2);
    CMainGameComponent live(&full_bank);
    CMainGameComponent uninitialised_game(0);
    CNewFrontendGameComponent front(&partial_bank);
    CNewFrontendGameComponent uninitialised_front(0);
    CProgressDisplay display;
    display.TextTagsPC.push_back("HINT");
    display.TextTagsPC.push_back("SINGLE");
    display.TextTagsPC.push_back("UNKNOWN");
    display.TextTag = "OLD";

    display.DisplayText = false;
    CMainGameComponent::Current = &live;
    display.CalculateNextTextTag();
    assert(display.TextTagIndex == -1 && !(display.TextTag != "OLD"));

    display.DisplayText = true;
    CMainGameComponent::Current = 0;
    display.CalculateNextTextTag();
    assert(display.TextTagIndex == -1 && !(display.TextTag != "OLD"));
    CMainGameComponent::Current = &uninitialised_game;
    CNewFrontendGameComponent::Current = &uninitialised_front;
    display.CalculateNextTextTag();
    assert(display.TextTagIndex == -1 && !(display.TextTag != "OLD"));

    CMainGameComponent::Current = &live;
    CNewFrontendGameComponent::Current = &front;
    const char* hints[] = { "HINT_10", "HINT_20", "HINT_30", "HINT_40", "HINT_50" };
    for (unsigned long i = 0; i < sizeof(hints) / sizeof(hints[0]); ++i)
    {
        display.CalculateNextTextTag();
        assert(display.TextTagIndex == 0 && !(display.TextTag != hints[i]));
    }
    display.CalculateNextTextTag();
    assert(display.TextTagIndex == 1 && !(display.TextTag != "SINGLE"));
    display.CalculateNextTextTag();
    assert(display.TextTagIndex == 2 && !(display.TextTag != "UNKNOWN"));
    display.CalculateNextTextTag();
    assert(display.TextTagIndex == 0 && !(display.TextTag != "HINT_10"));

    CMainGameComponent::Current = &uninitialised_game;
    display.CalculateNextTextTag();
    assert(display.TextTagIndex == 1 && !(display.TextTag != "SINGLE"));

    CProgressDisplay empty;
    empty.TextTag = "UNCHANGED";
    empty.CalculateNextTextTag();
    assert(empty.TextTagIndex == -1 && !(empty.TextTag != "UNCHANGED"));
    puts("00497b60_TEST PASS");
}