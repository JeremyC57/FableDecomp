// CNamedRecord_00ab3520::CNamedRecord_00ab3520() -- default constructor: a CCharString at +0, two words at +4/+8 left uninitialised, five zero words at +0xc..+0x1c and two false bytes at +0x20/+0x21
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AB3520, length 35 bytes (the oracle row carries 13 bytes of int3 padding).
// Callees: CCharString::CCharString() @ 0x0099E4B0. Shape: the string constructor runs first (first member), then one zero register feeds
// the word stores and the two byte stores in declaration order; the words at +4 and +8 are not touched (members the loader fills).
// The manifest label "CEntityScriptInfo::CEntityScriptInfo" is propagated (CEntityScriptInfo is 0x18 bytes with a different body @ 0x0052AB00);
// the class is named by address.
#include "fable_string.h"

struct CNamedRecord_00ab3520 {
    CCharString   m_name;                          // +0x00
    long          m_4;                             // +0x04 (not initialised here)
    long          m_8;                             // +0x08 (not initialised here)
    long          m_c;                             // +0x0c
    long          m_10;                            // +0x10
    long          m_14;                            // +0x14
    long          m_18;                            // +0x18
    long          m_1c;                            // +0x1c
    bool          m_20;                            // +0x20
    bool          m_21;                            // +0x21
    CNamedRecord_00ab3520();
};

CNamedRecord_00ab3520::CNamedRecord_00ab3520()
    : m_name()
    , m_c(0), m_10(0), m_14(0), m_18(0), m_1c(0)
    , m_20(false), m_21(false)
{
}