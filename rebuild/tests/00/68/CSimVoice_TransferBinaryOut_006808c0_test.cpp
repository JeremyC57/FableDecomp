// Behaviour test for 006808c0: final.cpp copied verbatim, the output stream stubbed so the written count, the data
// pointer and the byte count are observable for an empty and a filled voice map.
#include <cstdio>
#include <cstring>
#include <cstdlib>
#ifndef _MSC_VER
#define __forceinline inline
#define __declspec(x)
#define __fastcall
#define __cdecl
#define _snprintf snprintf
#endif
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
#define CHECKLOG(e) do { if (strcmp(g_log, e) != 0) { ++fails; printf("FAIL %d: log=%s\n  expected=%s\n", __LINE__, g_log, e); } } while (0)
static char g_log[4096];
static void note(const char* s) { if (strlen(g_log) + strlen(s) + 2 < sizeof(g_log)) { strcat(g_log, s); strcat(g_log, ";"); } }
// ---- source (verbatim) ----
// CSimVoice::TransferBinaryOut(CDataOutputStream& out) -- writes the Voices map: its entry count, then the raw entry block (the vector's data, or the map object itself when empty) of count * 8 bytes
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006808C0, length 73 bytes.
// Leaf: TransferBinaryOut
// Callees: CDataOutputStream::WriteLong @ 0x00993EB0; CDataOutputStream::Write(const void*, unsigned long), virtual slot 4 (+0x10).
// Layout as the landed CSimVoice::HasVoice 0x00680910: Voices (a sorted vector {first, last, end}, comparator, dirty
// flag) at +8. Same data/size idiom as the landed CDataOutputStream::Write(const CByteBlock&) 0x00995EB0.
// Shape: the map's address is kept in esi; the count is `(last - first) >> 3`, the data pointer an inline GetData()
// (`test ; ja` then `mov eax,esi` for the empty map) taken into a local FIRST, then the byte count re-derived as
// `((last - first) >> 3) << 3` from fresh loads: the second Size() goes through a may-alias pointer reference
// (`const CVoiceMap* const& voices = &Voices;`) so it is not CSE'd with GetData()'s count across the branch.
enum EReactionSpeechType { REACTION_SPEECH_NONE = 0 };

struct SVoiceEntry {                            // pair<EReactionSpeechType, long>
    EReactionSpeechType first;
    long                second;
};

struct CDataOutputStream {
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void Write(const void* data, unsigned long size);   // slot 4 (+0x10)
    void WriteLong(long value);                                 // 0x00993EB0
};

struct CKeyPairCompareLess { };                 // empty comparator (one byte)

struct CVoiceMap {                              // CVectorMap<EReactionSpeechType,long,CKeyPairCompareLess<..>>
    SVoiceEntry*        m_pFirst;               // +0x00
    SVoiceEntry*        m_pLast;                // +0x04
    SVoiceEntry*        m_pEnd;                 // +0x08
    CKeyPairCompareLess m_compare;              // +0x0c
    bool                m_dirty;                // +0x0d
    unsigned long Size() const { return m_pLast - m_pFirst; }
    const SVoiceEntry* GetData() const { return Size() > 0 ? m_pFirst : (const SVoiceEntry*)this; }
    void TransferBinaryOut(CDataOutputStream& out) const
    {
        out.WriteLong(Size());
        out.Write(GetData(), Size() * sizeof(SVoiceEntry));
    }
};

struct CInitBaseClass {                         // vtable 0x0129A7CC
    virtual ~CInitBaseClass();
    bool m_initialised;                         // +0x04
};

struct CSimVoice : CInitBaseClass {             // vtable 0x012337AC
    CVoiceMap Voices;                           // +0x08
    void TransferBinaryOut(CDataOutputStream& out) const;
};

void CSimVoice::TransferBinaryOut(CDataOutputStream& out) const
{
    out.WriteLong(Voices.Size());
    const SVoiceEntry* data = Voices.GetData();
    const CVoiceMap* const& voices = &Voices;
    out.Write(data, voices->Size() * sizeof(SVoiceEntry));
}

// ---- stubs ----
static char g_buf[256];
static const void* g_lastData = 0;
static unsigned long g_lastSize = 0;
struct CTestStream : CDataOutputStream {
    virtual void v00() { }
    virtual void v01() { }
    virtual void v02() { }
    virtual void v03() { }
    virtual void Write(const void* data, unsigned long size) { g_lastData = data; g_lastSize = size; _snprintf(g_buf, sizeof(g_buf), "Write(%lu bytes)", size); note(g_buf); }
};
void CDataOutputStream::v00() { }
void CDataOutputStream::v01() { }
void CDataOutputStream::v02() { }
void CDataOutputStream::v03() { }
void CDataOutputStream::Write(const void*, unsigned long) { note("base Write"); }
void CDataOutputStream::WriteLong(long value) { _snprintf(g_buf, sizeof(g_buf), "WriteLong(%ld)", value); note(g_buf); }
CInitBaseClass::~CInitBaseClass() { }

int main()
{
    CTestStream out;
    CSimVoice voice;
    SVoiceEntry entries[3];
    entries[0].first = REACTION_SPEECH_NONE; entries[0].second = 5;
    entries[1].first = (EReactionSpeechType)3; entries[1].second = 7;
    entries[2].first = (EReactionSpeechType)9; entries[2].second = 11;

    // 1. empty map: count 0, the raw block is the map object itself, 0 bytes
    voice.Voices.m_pFirst = entries; voice.Voices.m_pLast = entries; voice.Voices.m_pEnd = entries + 3;
    g_log[0] = 0;
    voice.TransferBinaryOut(out);
    CHECKLOG("WriteLong(0);Write(0 bytes);");
    CHECK(g_lastData == (const void*)&voice.Voices);
    CHECK(g_lastSize == 0);

    // 2. three entries: count 3, the entry block, 24 bytes
    voice.Voices.m_pLast = entries + 3;
    g_log[0] = 0;
    voice.TransferBinaryOut(out);
    CHECKLOG("WriteLong(3);Write(24 bytes);");
    CHECK(g_lastData == (const void*)entries);
    CHECK(g_lastSize == 3 * sizeof(SVoiceEntry));

    // 3. one entry
    voice.Voices.m_pLast = entries + 1;
    g_log[0] = 0;
    voice.TransferBinaryOut(out);
    CHECKLOG("WriteLong(1);Write(8 bytes);");
    CHECK(g_lastData == (const void*)entries && g_lastSize == 8);

    if (fails == 0) printf("006808c0_TEST PASS\n");
    else printf("006808c0_TEST FAIL (%d)\n", fails);
    return fails ? 1 : 0;
}