// Behaviour test for NProgressDisplay::GetProgressDisplay (0x009EA060): the returned counted
// pointer carries the global's object and count block and holds one extra reference; a global
// with no count block comes back as a plain copy.
#include <stdio.h>

class CAProgressDisplay { public: int marker; };

typedef void (__fastcall *FableDestroyReferencedObject)(void* object);

struct FableReferenceCount
{
    long owners;
    FableDestroyReferencedObject destroy;
    void* object;
};

static int g_destroyed = 0;
static void __fastcall DestroyDisplay(void*) { ++g_destroyed; }

template <class T>
class CCountedPointer
{
public:
    T* Data;
    FableReferenceCount* Info;

    CCountedPointer(const CCountedPointer& other)
        : Data(other.Data), Info(other.Info)
    {
        if (Info)
            ++Info->owners;
    }
    CCountedPointer(T* data, FableReferenceCount* info) : Data(data), Info(info) {}
    ~CCountedPointer()
    {
        if (Info && --Info->owners == 0)
            Info->destroy(Info->object);
    }
};

static CAProgressDisplay g_display;
static FableReferenceCount g_block = { 1, DestroyDisplay, &g_display };

namespace NProgressDisplay
{
    CCountedPointer<CAProgressDisplay> PProgressDisplay(&g_display, &g_block);

    CCountedPointer<CAProgressDisplay> __fastcall GetProgressDisplay();

    CCountedPointer<CAProgressDisplay> __fastcall GetProgressDisplay()
    {
        return PProgressDisplay;
    }
}

int main()
{
    {
        CCountedPointer<CAProgressDisplay> got = NProgressDisplay::GetProgressDisplay();
        if (got.Data != &g_display || got.Info != &g_block) { printf("FAIL copy\n"); return 1; }
        if (g_block.owners != 2) { printf("FAIL addref %ld\n", g_block.owners); return 1; }
    }
    if (g_block.owners != 1 || g_destroyed != 0) { printf("FAIL release\n"); return 1; }

    NProgressDisplay::PProgressDisplay.Info = 0;
    {
        CCountedPointer<CAProgressDisplay> plain = NProgressDisplay::GetProgressDisplay();
        if (plain.Data != &g_display || plain.Info != 0) { printf("FAIL null block\n"); return 1; }
    }
    if (g_block.owners != 1) { printf("FAIL untouched block\n"); return 1; }
    printf("FABLETLC_GET_PROGRESS_DISPLAY_BEHAVIOR PASS\n");
    return 0;
}
