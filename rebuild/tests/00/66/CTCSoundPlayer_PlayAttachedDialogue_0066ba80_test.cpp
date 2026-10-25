// Behaviour test for 0x0066BA80: CTCSoundPlayer::PlayAttachedDialogue(unsigned long textID, bool flag1, bool flag2, bool altFader, bool flag4) -- play a dialogue line attached to the owner: resolve the text entry (a group entry directly, else the entry for the owner's talk voice), find its sound bank and the "SND_"-prefixed sample, then play it with the dialogue fader (0x20 when altFader, else 0x8040); 0 when anything is missing
// A copy of the source model and logic (compiled separately from src/0066ba80.cpp) with test stand-ins for the callees.
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <new>
#if !defined(_MSC_VER)
#define __fastcall
#define __forceinline inline
#define __declspec(x)
#define __cdecl
#define __fastcall
#define __int64 long long
#endif
class CCharString {
public:
    void* PStringData;
    CCharString() : PStringData((void*)"") { }                                   // test-only constructors
    explicit CCharString(const char* text) : PStringData((void*)text) { }
    CCharString(const CCharString& other);             // 0x0099EC30
    ~CCharString();                                    // 0x0099EAE0
};
CCharString __fastcall operator+(const char* left, const CCharString& right);   // 0x0099F690

class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) const;        // 0x004365B0
};
#pragma auto_inline(off)
// Same-TU body of the CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound const instantiation.
__declspec(noinline) CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type) const
{
    SEntry* it = first;
    long count = last - first;
    while (count > 0) {
        long half = count / 2;
        if (it[half].type < *type) {
            it += half + 1;
            count -= half + 1;
        } else
            count = half;
    }
    return it;
}
#pragma auto_inline(on)
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                    // +0x20
    CInterfaceMap m_interfaces;                        // +0x44
    template <class T> __forceinline bool GetInterface(T** ppOut, long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            const CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)(it == map->last || interfaceType < it->type ? map->last : it)->component;
            return true;
        }
        return false;
    }
};
class CTCTalk {                                        // interface 0xF
public:
    unsigned char _pad00[0x38];
    CCharString   DialogueVoice;                       // +0x38
    CCharString GetDialogueVoice() const { return DialogueVoice; }
};

class CIVCountedPointeeVirtual {
public:
    virtual void Destroy(int flags);                   // slot 0
    virtual void Delete();                             // slot 1
};
class CIVRefCounted {
public:
    unsigned long IVRefCount;                          // +0x04
    void DecRef() { --IVRefCount; }
    unsigned long GetRefCount() const { return IVRefCount; }
};
class CIVCountedPointeeBase : public CIVCountedPointeeVirtual, public CIVRefCounted {
public:
    void ReleaseTemporary()
    {
        DecRef();
        if (GetRefCount() == 0)
            Delete();
    }
};
template <class T> class CIVCountedPointer {
public:
    T* Object;
    explicit CIVCountedPointer(T* object) : Object(object) { }                    // test-only constructor
    ~CIVCountedPointer()
    {
        if (Object)
            Object->ReleaseTemporary();
    }
};
namespace NGameText {
class CTextEntry : public CIVCountedPointeeBase {
public:
    unsigned char _pad08[0x1c - 0x08];
    CCharString BankName;                              // +0x1c
    unsigned char _pad20[0x24 - 0x20];
    CCharString SoundName;                             // +0x24
};
class CGroupEntry : public CTextEntry { };
class CDataBank {
public:
    unsigned long GetEntryDataType(unsigned long id) const;   // 0x009CC210 (CBankFile)
    CIVCountedPointer<CGroupEntry> GetPGroupEntry(unsigned long id, bool load) const;   // 0x009C9070
    const CTextEntry* GetTextEntryFromGroup(unsigned long id, const CCharString& group, bool& found, long* index, bool load) const;   // 0x009C9280
    const CTextEntry* GetTextEntryFromGroup(unsigned long id, const CCharString& group) const
    {
        bool found = false;
        return GetTextEntryFromGroup(id, group, found, 0, true);
    }
};
}
class CMainGameComponent {
public:
    unsigned char _pad00[0x14];
    NGameText::CDataBank* PTextBank;                   // +0x14
};
extern CMainGameComponent* g_pMainGameComponent;      // 0x013B86A0

class CASoundBank {
public:
    long GetSoundIndexFromSymbol(const CCharString& symbol) const;   // 0x00A01920
};
class CSymbolMap {
public:
    CASoundBank* FindFirstSymbolWithVal(const CCharString* value) const;   // 0x009917E0
};
extern CSymbolMap* g_pSoundBankMap;                    // 0x013B8394

class CSoundInit {
public:
    float Volume, Pitch, Pan;
    long NoPlays, PlaybackType;
    unsigned long GainFaderBits;                       // +0x14
    unsigned long PitchFaderBits;
    long OverrideID;
    CSoundInit();                                      // 0x0041C930
};

class CTCSoundPlayer {
public:
    void*   __vftable;                                 // +0x00
    CThing* ParentThing;                               // +0x04
    unsigned long PlayAttachedDialogue(unsigned long textID, bool flag1, bool flag2, bool altFader, bool flag4);
private:
    unsigned long InternalPlayAttachedSample(CASoundBank& bank, long sample, const CSoundInit& init, bool flag1, bool flag2, bool flag4);   // 0x0066B080
};

unsigned long CTCSoundPlayer::PlayAttachedDialogue(unsigned long textID, bool flag1, bool flag2, bool altFader, bool flag4)
{
    if (textID > 0) {
        NGameText::CDataBank* bank = g_pMainGameComponent->PTextBank;
        const NGameText::CTextEntry* entry;
        if (bank->GetEntryDataType(textID) == 0) {
            CIVCountedPointer<NGameText::CGroupEntry> group = bank->GetPGroupEntry(textID, true);
            entry = group.Object;
        } else {
            CTCTalk* talk;
            if (!ParentThing->GetInterface(&talk, 0xf))
                return 0;
            entry = bank->GetTextEntryFromGroup(textID, talk->GetDialogueVoice());
        }
        if (entry) {
            CASoundBank* soundBank = g_pSoundBankMap->FindFirstSymbolWithVal(&entry->BankName);
            if (soundBank) {
                long sample = soundBank->GetSoundIndexFromSymbol("SND_" + entry->SoundName);
                if (sample > 0) {
                    CSoundInit init;
                    init.GainFaderBits = altFader ? 0x20 : 0x8040;
                    return InternalPlayAttachedSample(*soundBank, sample, init, flag1, flag2, flag4);
                }
            }
        }
    }
    return 0;
}

// ---- test stand-ins ----
static int g_strings = 0, g_deletes = 0;
CCharString::CCharString(const CCharString& other) : PStringData(other.PStringData) { ++g_strings; }
CCharString::~CCharString() { }
static const char* Text(const CCharString& s) { return (const char*)s.PStringData; }
static char g_concat[8][64];
static int g_concatNext = 0;
CCharString __fastcall operator+(const char* left, const CCharString& right)
{
    char* out = g_concat[g_concatNext++ & 7];
    std::strcpy(out, left); std::strcat(out, Text(right));
    return CCharString(out);
}
void CIVCountedPointeeVirtual::Destroy(int) { }
void CIVCountedPointeeVirtual::Delete() { ++g_deletes; }

using namespace NGameText;
static CGroupEntry* g_group = 0;
static CTextEntry* g_voiceEntry = 0;
static int g_dataTypeCalls = 0, g_groupCalls = 0, g_fromGroupCalls = 0;
static bool g_lastFoundIn = true, g_lastLoad = false;
static long* g_lastIndex = (long*)1;
unsigned long CDataBank::GetEntryDataType(unsigned long id) const { ++g_dataTypeCalls; return id == 10 ? 0 : 1; }
CIVCountedPointer<CGroupEntry> CDataBank::GetPGroupEntry(unsigned long id, bool load) const
{
    ++g_groupCalls;
    CGroupEntry* object = (id == 10 && load) ? g_group : 0;
    if (object) ++object->IVRefCount;
    return CIVCountedPointer<CGroupEntry>(object);            // an unnamed temporary: VC7.1 has no NRVO
}
const CTextEntry* CDataBank::GetTextEntryFromGroup(unsigned long id, const CCharString& group, bool& found, long* index, bool load) const
{
    ++g_fromGroupCalls; g_lastFoundIn = found; g_lastIndex = index; g_lastLoad = load;
    if (id == 20 && std::strcmp(Text(group), "VOICE_A") == 0) { found = true; return g_voiceEntry; }
    return 0;
}
static CDataBank g_bank;
static CMainGameComponent g_main;
CMainGameComponent* g_pMainGameComponent = &g_main;

static CASoundBank g_soundBank;
long CASoundBank::GetSoundIndexFromSymbol(const CCharString& symbol) const
{
    if (std::strcmp(Text(symbol), "SND_HELLO") == 0) return 7;
    if (std::strcmp(Text(symbol), "SND_BYE") == 0) return 3;
    return 0;
}
CASoundBank* CSymbolMap::FindFirstSymbolWithVal(const CCharString* value) const
{
    return std::strcmp(Text(*value), "BANK_X") == 0 ? &g_soundBank : 0;
}
static CSymbolMap g_symbols;
CSymbolMap* g_pSoundBankMap = &g_symbols;

CSoundInit::CSoundInit() : Volume(1.0f), Pitch(1.0f), Pan(0.0f), NoPlays(1), PlaybackType(0), GainFaderBits(0), PitchFaderBits(0), OverrideID(-1) { }
static int g_plays = 0;
static long g_playSample = 0;
static unsigned long g_playFader = 0;
static bool g_playFlags[3];
static CASoundBank* g_playBank = 0;
unsigned long CTCSoundPlayer::InternalPlayAttachedSample(CASoundBank& bank, long sample, const CSoundInit& init, bool flag1, bool flag2, bool flag4)
{
    ++g_plays; g_playBank = &bank; g_playSample = sample; g_playFader = init.GainFaderBits;
    g_playFlags[0] = flag1; g_playFlags[1] = flag2; g_playFlags[2] = flag4;
    return 0x1234;
}

static int Fail(int line) { std::printf("0066ba80_TEST FAIL (line %d)\n", line); return 1; }
#define CHECK(c) if (!(c)) return Fail(__LINE__)

int main()
{
    g_main.PTextBank = &g_bank;
    CGroupEntry group; group.IVRefCount = 1; group.BankName = CCharString("BANK_X"); group.SoundName = CCharString("HELLO");
    CTextEntry voiced; voiced.IVRefCount = 1; voiced.BankName = CCharString("BANK_X"); voiced.SoundName = CCharString("BYE");
    g_group = &group; g_voiceEntry = &voiced;
    CTCTalk talk; talk.DialogueVoice = CCharString("VOICE_A");
    static CThing parent; std::memset(&parent, 0, sizeof parent);
    CInterfaceMap::SEntry entries[2] = { { 0x0f, (CTCBase*)&talk }, { 0x20, 0 } };
    parent.m_interfaces.first = entries; parent.m_interfaces.last = entries + 2;
    parent.m_presentBits[0] = 1UL << 0x0f;
    CTCSoundPlayer player; player.ParentThing = &parent;

    CHECK(player.PlayAttachedDialogue(0, true, true, true, true) == 0 && g_dataTypeCalls == 0);       // no text id
    CHECK(player.PlayAttachedDialogue(10, true, false, false, true) == 0x1234);                      // a group entry
    CHECK(g_plays == 1 && g_playBank == &g_soundBank && g_playSample == 7 && g_playFader == 0x8040);
    CHECK(g_playFlags[0] && !g_playFlags[1] && g_playFlags[2] && g_groupCalls == 1 && g_fromGroupCalls == 0);
    CHECK(group.IVRefCount == 1 && g_deletes == 0);                                                  // the temporary ref is released
    CHECK(player.PlayAttachedDialogue(20, false, true, true, false) == 0x1234);                      // the talk voice's entry
    CHECK(g_plays == 2 && g_playSample == 3 && g_playFader == 0x20 && !g_playFlags[0] && g_playFlags[1] && !g_playFlags[2]);
    CHECK(g_fromGroupCalls == 1 && !g_lastFoundIn && g_lastIndex == 0 && g_lastLoad);
    CHECK(player.PlayAttachedDialogue(21, true, true, true, true) == 0 && g_plays == 2);             // no entry in the voice group
    talk.DialogueVoice = CCharString("VOICE_B");
    CHECK(player.PlayAttachedDialogue(20, true, true, true, true) == 0 && g_plays == 2);
    talk.DialogueVoice = CCharString("VOICE_A");
    parent.m_presentBits[0] = 0;                                                                     // no talk component
    int before = g_fromGroupCalls;
    CHECK(player.PlayAttachedDialogue(20, true, true, true, true) == 0 && g_fromGroupCalls == before);
    voiced.BankName = CCharString("BANK_Y");                                                         // unknown sound bank
    parent.m_presentBits[0] = 1UL << 0x0f;
    CHECK(player.PlayAttachedDialogue(20, true, true, true, true) == 0 && g_plays == 2);
    group.SoundName = CCharString("SILENT");                                                         // no sample for the symbol
    CHECK(player.PlayAttachedDialogue(10, true, true, true, true) == 0 && g_plays == 2 && group.IVRefCount == 1);
    group.IVRefCount = 0;                                                                            // last reference: deleted
    group.SoundName = CCharString("HELLO");
    CHECK(player.PlayAttachedDialogue(10, true, true, false, true) == 0x1234 && g_deletes == 1);
    std::printf("0066ba80_TEST PASS\n");
    return 0;
}