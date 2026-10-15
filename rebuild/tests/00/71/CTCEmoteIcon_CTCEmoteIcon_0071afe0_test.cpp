#include <cstdio>

// Model of CTCEmoteIcon::CTCEmoteIcon(CThing*) @ 0x0071AFE0: the emote defaults (type 2, priority 8, alpha 0xff,
// replaceable and enabled) over an empty text and an empty primitive handle.
struct CThing { int id; };
struct CTCBase { CThing* ParentThing; CTCBase(CThing* t) : ParentThing(t) { } virtual ~CTCBase() { } };
struct CWideString { void* PStringData; CWideString() : PStringData(0) { } };
struct CEnginePrimitiveHandle { void* Data; CEnginePrimitiveHandle() : Data(0) { } virtual ~CEnginePrimitiveHandle() { } };
struct CTCEmoteIcon : CTCBase {
    long ActiveTimer; CWideString EmoteText; long EmoteIcon; float EmoteScale; long EmoteType; long EmotePriority;
    unsigned char EmoteAlpha; bool Replaceable; bool Enabled; CEnginePrimitiveHandle PrimitiveHandle;
    CTCEmoteIcon(CThing* thing)
        : CTCBase(thing), ActiveTimer(0), EmoteIcon(0), EmoteScale(0.0f), EmoteType(2), EmotePriority(8),
          EmoteAlpha(0xff), Replaceable(true), Enabled(true) { }
};

int main()
{
    CThing t; t.id = 3;
    CTCEmoteIcon* e = new CTCEmoteIcon(&t);
    bool ok = e->ParentThing == &t && e->ActiveTimer == 0 && e->EmoteText.PStringData == 0 && e->EmoteIcon == 0 &&
              e->EmoteScale == 0.0f && e->EmoteType == 2 && e->EmotePriority == 8 && e->EmoteAlpha == 0xff &&
              e->Replaceable && e->Enabled && e->PrimitiveHandle.Data == 0;
    delete e;
    if (!ok) { std::printf("0071afe0_TEST FAIL\n"); return 1; }
    std::printf("0071afe0_TEST PASS\n");
    return 0;
}