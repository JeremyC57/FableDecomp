// Behaviour test for 007887be: a self-contained copy of the candidate's model and body, run over stub callees.
// CTCTrapBase::UpdateTriggerOnProximity() -- proximity trigger: is any trigger thing inside the trap def's trigger radius; a triggered opening trap with nobody near resets, an untriggered trap triggers when somebody newly comes near
#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __forceinline inline
#define __fastcall
#define __stdcall
#define __cdecl
#define __declspec(x)
#endif
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    __forceinline C3DVector(const C3DVector& other) : X(other.X), Y(other.Y), Z(other.Z) {}
};

struct CThing {
    const C3DVector& GetPosition() const;       // 0x004C73D0
};
struct CBaseIntelligentPointer {                // CIntelligentPointer<CThing>: vptr + item (8 bytes)
    void* __vftable;
    void* PData;
    CThing* GetPItem() const;                   // 0x00A01B50
};
struct CTriggerThingVector {                    // STLport vector<CIntelligentPointer<CThing> > (12 bytes)
    CBaseIntelligentPointer* _M_start;
    CBaseIntelligentPointer* _M_finish;
    CBaseIntelligentPointer* _M_end_of_storage;
    CBaseIntelligentPointer* begin() { return _M_start; }
    CBaseIntelligentPointer* end() { return _M_finish; }
};
struct CFadeInFadeOutBase {                     // 0x1c bytes
    unsigned char _data[0x1c];
    bool IsOpen();                              // 0x0048DA30
};
struct CTrapDef {
    unsigned char _pad[0x34];
    long          TriggerType;                  // +0x34
    long          TrapType;                     // +0x38
    float         TriggerRadius;                // +0x3c
};

struct CTCTrapBase {
    void*         __vftable;                    // +0x00
    CThing*       ParentThing;                  // +0x04 (CTCBase)
    unsigned char _pad_0x08[0x24];
    CTriggerThingVector TriggerThings;          // +0x2c
    CFadeInFadeOutBase  TrapOpener;             // +0x38
    CTrapDef*     PTrapDef_Object;              // +0x54 CDefPointer<const CTrapDef>::Object
    bool          Active;                       // +0x58
    bool          Triggered;                    // +0x59
    bool          LastTriggerOnProximity;       // +0x5a
    long          TrapType;                     // +0x5c ETrapType
    void Reset();                               // 0x00787BF1
    void Trigger();                             // 0x00788229
    void UpdateTriggerOnProximity();
};

void CTCTrapBase::UpdateTriggerOnProximity()
{
    C3DVector position = ParentThing->GetPosition();
    bool inRange = false;
    for (CBaseIntelligentPointer* it = TriggerThings.begin(); it != TriggerThings.end(); ++it) {
        const C3DVector& thing = it->GetPItem()->GetPosition();
        float dx = thing.X - position.X;
        float dy = thing.Y - position.Y;
        float dz = thing.Z - position.Z;
        float radius = PTrapDef_Object->TriggerRadius;
        if (dx * dx + dy * dy + dz * dz < radius * radius)
            inRange = true;
    }
    if (Triggered) {
        if ((TrapType == 1 || TrapType == 4 || TrapType == 2) && TrapOpener.IsOpen() && !inRange) {
            Triggered = false;
            Reset();
        }
    } else if (inRange && !LastTriggerOnProximity)
        Trigger();
    LastTriggerOnProximity = inRange;
}

// ---- stubs and driver ----
static CThing g_things[3];
static C3DVector g_positions[3];
const C3DVector& CThing::GetPosition() const { return g_positions[this - g_things]; }
CThing* CBaseIntelligentPointer::GetPItem() const { return (CThing*)PData; }
static bool g_open;
bool CFadeInFadeOutBase::IsOpen() { return g_open; }
static int g_resets, g_triggers;
void CTCTrapBase::Reset() { ++g_resets; }
void CTCTrapBase::Trigger() { ++g_triggers; }
static void Set(int i, float x, float y, float z) { g_positions[i].X = x; g_positions[i].Y = y; g_positions[i].Z = z; }

int main()
{
    static CTCTrapBase trap;
    static CTrapDef def;
    static CBaseIntelligentPointer triggers[2];
    def.TriggerRadius = 2.0f;
    trap.PTrapDef_Object = &def;
    trap.ParentThing = &g_things[0];
    triggers[0].PData = &g_things[1]; triggers[1].PData = &g_things[2];
    trap.TriggerThings._M_start = triggers; trap.TriggerThings._M_finish = triggers + 2; trap.TriggerThings._M_end_of_storage = triggers + 2;
    Set(0, 10, 10, 10); Set(1, 20, 10, 10); Set(2, 11, 11, 10.5f);   // thing 2 is within 2.0
    // untriggered, newly in range: Trigger()
    trap.Triggered = false; trap.LastTriggerOnProximity = false;
    trap.UpdateTriggerOnProximity();
    if (g_triggers != 1 || g_resets || !trap.LastTriggerOnProximity) { std::printf("007887be_TEST FAIL (trigger)\n"); return 1; }
    // still in range: no second trigger
    trap.UpdateTriggerOnProximity();
    if (g_triggers != 1) { std::printf("007887be_TEST FAIL (repeat)\n"); return 1; }
    // triggered opening trap, nobody near: reset
    Set(2, 13, 10, 10);                                                // exactly 3 away
    trap.Triggered = true; trap.TrapType = 4; g_open = true;
    trap.UpdateTriggerOnProximity();
    if (g_resets != 1 || trap.Triggered || trap.LastTriggerOnProximity) { std::printf("007887be_TEST FAIL (reset)\n"); return 1; }
    // triggered but of type 3: no reset
    trap.Triggered = true; trap.TrapType = 3;
    trap.UpdateTriggerOnProximity();
    if (g_resets != 1 || !trap.Triggered) { std::printf("007887be_TEST FAIL (type)\n"); return 1; }
    // triggered, type 2, but the opener is not open: no reset
    trap.TrapType = 2; g_open = false;
    trap.UpdateTriggerOnProximity();
    if (g_resets != 1) { std::printf("007887be_TEST FAIL (closed)\n"); return 1; }
    std::printf("007887be_TEST PASS\n");
    return 0;
}
