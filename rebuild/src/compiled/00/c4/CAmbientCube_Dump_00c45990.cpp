// CAmbientCube::Dump() -- when any apex of the ambient-sound cube has a non-zero minimum listener distance, log "Ambient cube:-" and one "   Apex (x,y,z) = <distance>" line per corner through the libpk log sink
// Leaf: ?Dump@CAmbientCube@@QAEXXZ
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C45990, 1440 bytes (one function, `ret` at 0x00C45F2F), thiscall, EH-framed (nine
// CString states 0..8) over an 8-aligned frame (`push ebp ; mov ebp,esp ; and esp,-8`).
// Identity: libpk's CAmbientCube (descriptive, no PDB/RTTI; the landed ctor 0x00C45F30, UpdateDistances 0x00C45810,
// SetCentre 0x00C458D0, GetLevel 0x00C45940, UpdateLevel 0x00C45980): eight 0x1C-byte apexes whose +0x18 is the minimum
// distance (read here at +0x18 + i * 0x1C); the corners are printed in the ctor's bit order (X, Y, Z = bits 2, 1, 0).
// Strings "Ambient cube:-" 0x012B7D40, "   Apex (" 0x012B7D34, "," 0x0122E024, ") = " 0x012B7D2C, "\n" 0x0129B208.
// No caller or data reference in retail (a debug dump left unreferenced). The catalog's "Render_CheckAmbientCubeLighting" is
// a propagated guess.
// Callees: the libpk CString builders through engine/CString.h: operator<<(const char*) 0x00C0DEC0, (double) 0x00C0DED0,
// (long) 0x00C0DF00, ~CString 0x00C0BA40; _afxPchNil 0x0139C900; the trace hook g_pfnAssertHook 0x0143BB2C (the landed
// ReadBlock 0x00C4B890 spells it g_pfnLogSink_0143bb2c and the nil string g_pchNil_0139c900).
// Shape: the any-apex test is a loop that stops at the first hit (`i < 8 && !any`): retail keeps the up-counter next to the
// apex pointer; a loop without the early exit is fully unrolled (for/while) or turned into a down-counter (do-while).
#define FABLE_RETAIL_CSTRING_BUILDERS
#define FABLE_RETAIL_CSTRING_ALLOCCOPY
#define FABLE_RETAIL_CSTRING_DEFAULT_CTOR
#include "engine/CString.h"

struct SAmbientApex {                                   // 0x1C bytes
    float m_offset[3];                                  // +0x00 corner offset from the cube's centre
    float m_position[3];                                // +0x0c corner position
    float m_minDistance;                                // +0x18 closest listener distance seen
};

class CAmbientCube {
public:
    SAmbientApex m_apexes[8];                           // +0x00
    void Dump();
};

void CAmbientCube::Dump()
{
    int any = 0;
    for (int i = 0; i < 8 && !any; ++i)
        any = m_apexes[i].m_minDistance != 0.0f;
    if (!any)
        return;
    {
        CString line;
        line << "Ambient cube:-" << "\n";
        if (g_pfnAssertHook)
            g_pfnAssertHook(line.m_pchData);
    }
    {
        CString line;
        line << "   Apex (" << 0L << "," << 0L << "," << 0L << ") = " << (double)m_apexes[0].m_minDistance << "\n";
        if (g_pfnAssertHook)
            g_pfnAssertHook(line.m_pchData);
    }
    {
        CString line;
        line << "   Apex (" << 0L << "," << 0L << "," << 1L << ") = " << (double)m_apexes[1].m_minDistance << "\n";
        if (g_pfnAssertHook)
            g_pfnAssertHook(line.m_pchData);
    }
    {
        CString line;
        line << "   Apex (" << 0L << "," << 1L << "," << 0L << ") = " << (double)m_apexes[2].m_minDistance << "\n";
        if (g_pfnAssertHook)
            g_pfnAssertHook(line.m_pchData);
    }
    {
        CString line;
        line << "   Apex (" << 0L << "," << 1L << "," << 1L << ") = " << (double)m_apexes[3].m_minDistance << "\n";
        if (g_pfnAssertHook)
            g_pfnAssertHook(line.m_pchData);
    }
    {
        CString line;
        line << "   Apex (" << 1L << "," << 0L << "," << 0L << ") = " << (double)m_apexes[4].m_minDistance << "\n";
        if (g_pfnAssertHook)
            g_pfnAssertHook(line.m_pchData);
    }
    {
        CString line;
        line << "   Apex (" << 1L << "," << 0L << "," << 1L << ") = " << (double)m_apexes[5].m_minDistance << "\n";
        if (g_pfnAssertHook)
            g_pfnAssertHook(line.m_pchData);
    }
    {
        CString line;
        line << "   Apex (" << 1L << "," << 1L << "," << 0L << ") = " << (double)m_apexes[6].m_minDistance << "\n";
        if (g_pfnAssertHook)
            g_pfnAssertHook(line.m_pchData);
    }
    {
        CString line;
        line << "   Apex (" << 1L << "," << 1L << "," << 1L << ") = " << (double)m_apexes[7].m_minDistance << "\n";
        if (g_pfnAssertHook)
            g_pfnAssertHook(line.m_pchData);
    }
}