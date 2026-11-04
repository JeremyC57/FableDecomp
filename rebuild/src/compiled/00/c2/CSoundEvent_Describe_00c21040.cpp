// CSoundEvent::Describe(CSoundEventManager* manager) -- the trace description of one sound event: "EventId(<id>), Priority=<p>, State='<state>', offset(..), loops(..), " + behaviour emitter + 2D/3D position + base / user gain, returned by value
// Leaf: ?Describe@CSoundEvent@@QAE?AVCString@@PAUCSoundEventManager@@@Z
// Vendor naming wave V1 (one address, one name): 0x00C34600 ?GetValue@CQuadHolder@@QBE?AUSQuad@@XZ (was ?GetRecord@CSoundParams@@QBE?AUSEventRecord@@XZ)
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C21040, 684 bytes (body to the `ret 8` at 0x00C212D3; the 5-entry state jump table at
//   0x00C212D8 follows it), EH-framed, hidden CString result.
// Identity: the libpk sound event (vtable 0x012B3674; no RTTI / PDB, descriptive family names). Bound spelling
//   ?Describe@CSoundEvent@@QAE?AVCString@@PAUCSoundEventManager@@@Z (callers CSoundEventManager::TraceEvents 0x00C12720 /
//   TracePlaybacks 0x00C127C0, landed). Strings: "EventId(", "), Priority=", ", State='", the five state names (jump table
//   0x00C212D8: 2 "Waiting for data", 3 "Waiting for physical player", 4 "Waiting for memory", 5 "Playing using virtual player",
//   6 "Playing using physical player", checked by hand against the table), "', offset(", "), loops(", "), ", " offset=1.0, ",
//   "Emitter ID = " / "Not in behaviour, ", ", 3D(", "HEADRELATIVE:", ",", "2D(", ")", ", base gain(", "), user gain(".
// Header: engine/CString.h (FABLE_RETAIL_CSTRING_BUILDERS adds the ctor/dtor/builder declarations, chunk 750 overlay).
// Callees: CheckState 0x00C20720 (-1), GetLevel 0x00C20BE0 (manager, &level), GetMode 0x00C20EE0, CQuadHolder::GetRecord 0x00C34600
//   (+0x38, hidden result), the libpk MFC-style CString: ctor(const char*) 0x00C0BFE0, copy ctor 0x00C0DAB0, dtor 0x00C0BA40 and the
//   `__fastcall` builders operator<<(const char*) 0x00C0DEC0, (unsigned long) 0x00C0DF10, (long) 0x00C0DF00 (the bound spellings)
//   and (double) 0x00C0DED0 (`fld qword [esp+4]`, `ret 8`; no spelling bound yet).
// Layout: +0x0C event id, +0x14 priority, +0x24 behaviour emitter id, +0x38 the parameter block (its 2D pan +0x28 = event +0x60,
//   base gain +0x2C, user gain +0x30: the landed setters 0x00C34580 / 0x00C34500 write those words), +0xAC state, +0xB0 in-behaviour
//   flag. The level record (gain float, loop count long) and the 16-byte position record (x, y, z, head-relative flag) are the
//   landed SEventLevel / SQuad.
// Shape: each output line is one chained `<<` expression (VC evaluates the double arguments of the whole chain first, which
//   is why retail stores two or three doubles before the first call); the first chain builds a CString temporary that copy-
//   constructs the local; `return s;` copies into the hidden result (no NRVO in VC7.1).
#define FABLE_RETAIL_CSTRING_BUILDERS
#include "engine/CString.h"                     // the libpk MFC-style CString (m_pchData +0) and its stream builders

struct CSoundEventManager;

struct SEventLevel {                         // the 8-byte level record (GetLevel's output)
    float m_gain;                            // +0
    long m_loops;                            // +4
};

struct SQuad {                        // 16 bytes: the 3D position and the head-relative flag
    float m_x;
    float m_y;
    float m_z;
    bool m_headRelative;
};

struct CQuadHolder {                        // +0x38 (0x40 bytes)
    unsigned char _pad00[0x28];
    float m_pan;                             // +0x28 (event +0x60)
    float m_baseGain;                        // +0x2C (event +0x64)
    float m_userGain;                        // +0x30 (event +0x68)
    unsigned char _pad34[0x40 - 0x34];
    SQuad GetValue() const;          // 0x00C34600 (hidden result)
};

struct CSoundEvent {                         // 0xB4 bytes, vtable 0x012B3674 (no RTTI)
    void* __vftable;                         // +0x00
    unsigned char _pad04[0x0C - 0x04];
    unsigned long m_index;                   // +0x0C
    unsigned char _pad10[0x14 - 0x10];
    unsigned long m_priority;                // +0x14
    unsigned char _pad18[0x24 - 0x18];
    unsigned long m_idEmitter;               // +0x24
    unsigned char _pad28[0x38 - 0x28];
    CQuadHolder m_params;                   // +0x38
    unsigned char _pad78[0xAC - 0x78];
    unsigned short m_state;                  // +0xAC
    unsigned char _padae[2];
    bool m_listening;                        // +0xB0 (in behaviour)
    unsigned char _padb1[3];
    void CheckState(long expected);          // 0x00C20720
    void GetLevel(CSoundEventManager* manager, SEventLevel* level);   // 0x00C20BE0
    long GetMode();                          // 0x00C20EE0
    CString Describe(CSoundEventManager* manager);
};

// Same-TU body of CheckState 0x00C20720 (the landed WaitForData stand-in); only Describe is graded from this file.
#pragma auto_inline(off)
__declspec(noinline) void CSoundEvent::CheckState(long expected)
{
    switch (m_state) { case 0: case 1: case 2: case 3: case 4: case 5: case 6: break; }
}
#pragma auto_inline(on)

CString CSoundEvent::Describe(CSoundEventManager* manager)
{
    CheckState(-1);
    CString s = CString("EventId(") << m_index << "), Priority=" << m_priority << ", State='";
    switch (m_state) {
    case 2: s << "Waiting for data"; break;
    case 3: s << "Waiting for physical player"; break;
    case 4: s << "Waiting for memory"; break;
    case 5: s << "Playing using virtual player"; break;
    case 6: s << "Playing using physical player"; break;
    }
    SEventLevel level;
    GetLevel(manager, &level);
    s << "', offset(" << level.m_gain * 100.0f << "), loops(" << level.m_loops << "), ";
    if (level.m_gain == 1.0f)
        s << " offset=1.0, ";
    if (m_listening)
        s << "Emitter ID = " << m_idEmitter << ", ";
    else
        s << "Not in behaviour, ";
    if (GetMode()) {
        SQuad record = m_params.GetValue();
        s << ", 3D(";
        if (record.m_headRelative)
            s << "HEADRELATIVE:";
        s << record.m_x << "," << record.m_y << "," << record.m_z << ")";
    } else {
        s << "2D(" << m_params.m_pan << ")";
    }
    s << ", base gain(" << m_params.m_baseGain << "), user gain(" << m_params.m_userGain << ")";
    return s;
}