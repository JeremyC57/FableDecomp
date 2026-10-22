// Behaviour test for CDrawAutoPickups::OnVisibilityChange (0x00C43FD0): body verbatim, SelectAndDrawPrimitive stubbed,
// every branch exercised (idle with/without a primitive, selection failure, state 8, visible flips both ways, dirty bit).
#include <stdio.h>
#include <string.h>

struct CDrawAutoPickups;
typedef void (__stdcall *TChangeCallback)(CDrawAutoPickups* component, unsigned long flags, void* userData);

union UComponentFlags {
    unsigned long Raw;
    struct {
        unsigned long State   : 7;              // bits 0..6
        unsigned long Unused7 : 1;
        unsigned long Visible : 1;              // bit 8
    } Bits;
};

struct CDrawAutoPickups {
    void*           __vftable;
    unsigned char   _pad04[0x10 - 4];
    UComponentFlags Flags;                      // +0x10
    unsigned long   DirtyFlags;                 // +0x14
    unsigned char   _pad18[0x50 - 0x18];
    void*           SelectedPrimitive;          // +0x50
    unsigned char   _pad54[0xdc - 0x54];
    TChangeCallback ChangeCallback;             // +0xdc
    void*           CallbackData;               // +0xe0
    long SelectAndDrawPrimitive();              // 0x00C43DE0
    long OnVisibilityChange(bool visible);
    unsigned long GetState() const { return Flags.Raw & 0x7f; }
    void RaiseChange()
    {
        TChangeCallback callback = ChangeCallback;
        if (callback)
            callback(this, Flags.Raw & 0xffff, CallbackData);
    }
    void SetState(unsigned long newState)
    {
        const unsigned long& state = GetState();
        if (state != newState) {
            Flags.Bits.State = newState;
            RaiseChange();
        }
    }
};

long CDrawAutoPickups::OnVisibilityChange(bool visible)
{
    unsigned long state = GetState();
    switch (state) {
    case 0:
        if (SelectedPrimitive == 0) {
            long error = SelectAndDrawPrimitive();
            if (error)
                return error;
        }
        SetState(1);
        break;
    case 8:
        SetState(16);
        break;
    }
    if (visible != (bool)Flags.Bits.Visible) {
        if (visible)
            Flags.Raw |= 0x100;
        else
            Flags.Raw &= ~0x100UL;
        if (state != 0)
            DirtyFlags |= 1;
        RaiseChange();
    }
    return 0;
}

static long g_selectResult = 0;
static int g_selectCalls = 0;
static int g_callbackCalls = 0;
static unsigned long g_lastFlags = 0;
static void* g_lastData = 0;

long CDrawAutoPickups::SelectAndDrawPrimitive()
{
    ++g_selectCalls;
    if (g_selectResult == 0)
        SelectedPrimitive = (void*)0x1234;
    return g_selectResult;
}

static void __stdcall ChangeCallback(CDrawAutoPickups* component, unsigned long flags, void* userData)
{
    ++g_callbackCalls;
    g_lastFlags = flags;
    g_lastData = userData;
}

static int fail(const char* why) { printf("FAIL: %s\n", why); return 1; }

int main()
{
    CDrawAutoPickups c;
    memset(&c, 0, sizeof(c));
    c.ChangeCallback = ChangeCallback;
    c.CallbackData = (void*)0x77;

    // Idle, no primitive, selection fails: returns the error, nothing else changes.
    g_selectResult = 5;
    if (c.OnVisibilityChange(true) != 5) return fail("error return");
    if (g_selectCalls != 1 || g_callbackCalls != 0 || c.Flags.Raw != 0) return fail("error path side effects");

    // Idle, selection succeeds: state 0 -> 1 (callback), then Visible flips on (callback, no dirty bit: state was idle).
    g_selectResult = 0;
    if (c.OnVisibilityChange(true) != 0) return fail("idle return");
    if (g_selectCalls != 2 || c.SelectedPrimitive != (void*)0x1234) return fail("primitive selected");
    if ((c.Flags.Raw & 0x7f) != 1 || !(c.Flags.Raw & 0x100)) return fail("state 1 / visible");
    if (g_callbackCalls != 2 || g_lastData != (void*)0x77 || g_lastFlags != (c.Flags.Raw & 0xffff)) return fail("callbacks after idle");
    if (c.DirtyFlags != 0) return fail("dirty after idle");

    // Visible already on: no change, no callback.
    if (c.OnVisibilityChange(true) != 0 || g_callbackCalls != 2) return fail("unchanged visible");

    // State 8: 8 -> 16 (callback), visible off (callback), dirty bit set (state not idle).
    c.Flags.Raw = 8 | 0x100;
    c.SelectedPrimitive = 0;
    if (c.OnVisibilityChange(false) != 0) return fail("state 8 return");
    if (g_selectCalls != 2) return fail("no selection from state 8");
    if ((c.Flags.Raw & 0x7f) != 16 || (c.Flags.Raw & 0x100)) return fail("state 16 / invisible");
    if (g_callbackCalls != 4 || c.DirtyFlags != 1) return fail("callbacks / dirty after state 8");

    // Another state (3) with a primitive: no state change, visible on, dirty bit, one callback.
    c.Flags.Raw = 3;
    c.DirtyFlags = 0;
    c.ChangeCallback = 0;
    if (c.OnVisibilityChange(true) != 0) return fail("state 3 return");
    if (c.Flags.Raw != (3 | 0x100) || c.DirtyFlags != 1 || g_callbackCalls != 4) return fail("state 3 flip without callback");

    printf("00c43fd0_TEST PASS\n");
    return 0;
}