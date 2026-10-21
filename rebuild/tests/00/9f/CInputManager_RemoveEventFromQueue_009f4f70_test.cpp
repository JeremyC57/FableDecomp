// CInputManager::RemoveEventFromQueue(const CInputEvent& event) -- find the first stored event that matches the given one (same type and device number; for the mouse the button of a button event, for the keyboard the character of a character event or else the key; any joystick event) and clear it
// Leaf: RemoveEventFromQueue
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009F4F70, length 295 bytes (+ int3 padding), `ret 4`.
// Signature: engine_api.tsv __fastcall void (CInputManager*, CInputEvent*).
// Callees (CInputEvent's out-of-line accessors): GetType @ 0x00A03B40 (+0x28), GetDeviceType @ 0x00A03B50 (+0x20),
//   GetDeviceNumber @ 0x00A03B60 (+0x24), GetKey @ 0x00A03B70 (+0x00), GetChar @ 0x00A03B80 (+0x00, wchar_t),
//   GetButton @ 0x00A03B90 (+0x08, movzx), Clear @ 0x00A03BE0 (Type = DeviceType = 0). Catalog labels on these
//   (DrawGetEnvironment x3, GetCanItemBeQuickAssigned, CWADFile::Close) are propagated from byte-identical bodies.
// Layout (retail, 12-byte STLport vectors; Ego_r PDB names): ProcessedEvents +0x04, EventStore +0x10, NoStoredEvents +0x1c.
// Shape: the match test indexes EventStore[i] afresh for every accessor call (retail reloads the vector start each
//   time: `mov eax,[edi+10h] ; lea ecx,[ebx+eax]`, which needs operator[] written over an inline begin()), so it is
//   written in the loop body: type, then device number with the given event first (`cmp eax,ebp`), then a switch on
//   the given event's device type whose cases `goto found` on each comparison (given event first) -- a `same` flag is
//   materialised (`sete al`) -- and case 3 jumps straight to found (threaded into the shared `je` through the switch's
//   `dec eax` flags). The match (Clear) is laid after the loop's own epilogue and recomputes `i * 0x34`.
struct CInputEvent {
    unsigned char _body[0x34];
    long           GetType() const;                            // 0x00A03B40
    long           GetDeviceType() const;                      // 0x00A03B50
    long           GetDeviceNumber() const;                    // 0x00A03B60
    long           GetKey() const;                             // 0x00A03B70
    unsigned short GetChar() const;                     // 0x00A03B80 (wchar_t; VC7.1 without /Zc:wchar_t)
    long           GetButton() const;                          // 0x00A03B90
    void           Clear();                                    // 0x00A03BE0
};

template <class T>
struct vector {
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T* begin() { return _M_start; }
    T& operator[](unsigned int n) { return *(begin() + n); }
};

struct CInputManager {
    void*               __vftable;                      // +0x00
    vector<CInputEvent> ProcessedEvents;                // +0x04
    vector<CInputEvent> EventStore;                     // +0x10
    long                NoStoredEvents;                 // +0x1c
    void RemoveEventFromQueue(const CInputEvent& event);
};

void CInputManager::RemoveEventFromQueue(const CInputEvent& event)
{
    long i;
    for (i = 0; i < NoStoredEvents; ++i) {
        if (EventStore[i].GetType() == event.GetType() && event.GetDeviceNumber() == EventStore[i].GetDeviceNumber()) {
            switch (event.GetDeviceType()) {
            case 1:
                if (event.GetType() == 0x13 || event.GetType() == 0x14 || event.GetType() == 0x15) {
                    if (event.GetButton() == EventStore[i].GetButton())
                        goto found;
                } else
                    goto found;
                break;
            case 2:
                if (event.GetType() == 0xf || event.GetType() == 0x10) {
                    if (event.GetChar() == EventStore[i].GetChar())
                        goto found;
                } else if (event.GetKey() == EventStore[i].GetKey())
                    goto found;
                break;
            case 3:
                goto found;
            }
        }
    }
    return;
found:
    EventStore[i].Clear();
}

// ---- test harness ----
#include <stdio.h>
#include <string.h>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
static long rd(const CInputEvent* e, int off) { long v; memcpy(&v, e->_body + off, 4); return v; }
static void wr(CInputEvent* e, int off, long v) { memcpy(e->_body + off, &v, 4); }
long CInputEvent::GetType() const { return rd(this, 0x28); }
long CInputEvent::GetDeviceType() const { return rd(this, 0x20); }
long CInputEvent::GetDeviceNumber() const { return rd(this, 0x24); }
long CInputEvent::GetKey() const { return rd(this, 0); }
unsigned short CInputEvent::GetChar() const { return (unsigned short)rd(this, 0); }
long CInputEvent::GetButton() const { return _body[8]; }
void CInputEvent::Clear() { wr(this, 0x28, 0); wr(this, 0x20, 0); }

static void make(CInputEvent& e, long type, long dev, long num, long key, unsigned char button)
{
    memset(&e, 0, sizeof e);
    wr(&e, 0x28, type); wr(&e, 0x20, dev); wr(&e, 0x24, num); wr(&e, 0, key); e._body[8] = button;
}

int main()
{
    static CInputEvent store[6];
    static CInputManager m;
    m.EventStore._M_start = store; m.EventStore._M_finish = store + 6; m.EventStore._M_end_of_storage = store + 6;
    m.NoStoredEvents = 6;
    CInputEvent q;
    // mouse button events: match on button for types 0x13..0x15
    make(store[0], 0x13, 1, 0, 0, 1);
    make(store[1], 0x13, 1, 0, 0, 2);
    make(store[2], 0x0f, 2, 0, 0x10041, 0);          // char 'A' (low 16 bits), key differs above
    make(store[3], 0x05, 2, 0, 0x22, 0);             // key event
    make(store[4], 0x07, 3, 1, 0, 0);                // joystick, device 1
    make(store[5], 0x09, 1, 0, 0, 9);                // mouse non-button event
    make(q, 0x13, 1, 0, 0, 2);
    m.RemoveEventFromQueue(q);
    CHECK(store[1].GetType() == 0 && store[0].GetType() == 0x13);
    make(q, 0x13, 1, 0, 0, 7);                       // no button match
    m.RemoveEventFromQueue(q);
    CHECK(store[0].GetType() == 0x13);
    make(q, 0x0f, 2, 0, 0x20041, 0);                 // same char, different high word
    m.RemoveEventFromQueue(q);
    CHECK(store[2].GetType() == 0);
    make(q, 0x05, 2, 0, 0x23, 0);                    // key mismatch
    m.RemoveEventFromQueue(q);
    CHECK(store[3].GetType() == 5);
    make(q, 0x05, 2, 0, 0x22, 0);
    m.RemoveEventFromQueue(q);
    CHECK(store[3].GetType() == 0);
    make(q, 0x07, 3, 0, 0, 0);                       // joystick on another device number
    m.RemoveEventFromQueue(q);
    CHECK(store[4].GetType() == 7);
    make(q, 0x07, 3, 1, 5, 5);                       // any joystick event of that type and device
    m.RemoveEventFromQueue(q);
    CHECK(store[4].GetType() == 0);
    make(q, 0x09, 1, 0, 0, 1);                       // non-button mouse event: matches on type alone
    m.RemoveEventFromQueue(q);
    CHECK(store[5].GetType() == 0 && store[0].GetType() == 0x13);
    make(store[5], 0x09, 4, 0, 0, 0);                // unknown device type never matches
    make(q, 0x09, 4, 0, 0, 0);
    m.RemoveEventFromQueue(q);
    CHECK(store[5].GetType() == 9);
    m.NoStoredEvents = 0;
    make(q, 0x13, 1, 0, 0, 1);
    m.RemoveEventFromQueue(q);
    CHECK(store[0].GetType() == 0x13);
    if (fails == 0) printf("009f4f70_TEST PASS\n");
    return fails;
}