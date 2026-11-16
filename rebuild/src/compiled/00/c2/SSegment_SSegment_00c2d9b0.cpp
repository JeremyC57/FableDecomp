// SSegment::~SSegment() -- implicit destructor of a segment-table entry: releases its +8 name string
// Leaf: ??1SSegment@@QAE@XZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C2D9B0, 8 bytes: `add ecx,8 ; jmp 0x00C0BA40` (thiscall tail call to CString::~CString).
// Identity: DESCRIPTIVE name (libpk has no PDB or RTTI; recorded in NOTES), the entry struct the landed CSegmentFile::OpenSegment
//   0x00C2D9C0 models as SSegment {m_first, m_count} (segment start and length, the table's Find result). Its +8 string is the
//   segment's name: CSegmentFile::~CSegmentFile 0x00C2DBE0 removes every entry its +0x80 segment table still yields and deletes it
//   with this destructor inlined (`lea ecx,[edi+8] ; call 0x00C0BA40 ; push edi ; call operator delete`, 0x00C2DC2C -- the only
//   `lea ecx,[reg+8]` call of 0x00C0BA40 in the image). Its other caller is the unreferenced scalar deleting destructor 0x00C2DCA0.
// Callee: CString::~CString 0x00C0BA40, the catalog and engine/CString.h spelling ??1CString@@QAE@XZ.
// Shape: a compiler-generated destructor; EmitSegmentDestructor below only makes the translation unit emit the implicit ??1, it is
//   not a retail function.
// Header: engine/CString.h (FABLE_RETAIL_CSTRING_BUILDERS).
#define FABLE_RETAIL_CSTRING_BUILDERS
#include "engine/CString.h"

struct SSegment {                               // a segment table entry
    unsigned long m_first;                      // +0x00 start offset
    unsigned long m_count;                      // +0x04 length
    CString       m_name;                       // +0x08 segment name
};

void EmitSegmentDestructor(SSegment* segment)   // emits the implicit ??1; not a retail function
{
    delete segment;
}