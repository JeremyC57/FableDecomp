// SExpSpendUndo::SExpSpendUndo() -- the experience-spend undo record's constructor: its four empty list<long> histories (Generals, Strengths, Skills, Wills), Levels = 0, then the before / after stat words cleared
// Leaf: ??0SExpSpendUndo@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005797A4, length 86 bytes.
// Identity (2026-11-10, with the Codex _List_base name fix): Ego_r.pdb ??0SExpSpendUndo@@QAE@XZ; struct_layouts_egor SExpSpendUndo =
//   four list<long> (Generals / Strengths / Skills / Wills) + nine longs (Levels, General .. Will, aGeneral .. aWill), 0x34 bytes
//   with STLport's one-pointer list -- the four list constructions and the nine zeroed words here; the landed destructor
//   ~SExpSpendUndo 0x005797FA starts right after this body. The earlier "CHeroExperienceDef" label was a propagated twin
//   (CHeroExperienceDef has vectors and maps, no lists at +0).
// Callees: _List_base<long, allocator<long> >(const allocator<long>&) 0x0042AB58 (out of line, four times).
// Shape: the EBP frame with the 1-byte allocator temporary at [ebp-1] shared by the four lists; the chained `= 0` stores go
//   right to left (Will, Skill, Strength, General; then the after set). The 0x0057xxxx module is size-optimised with the frame
//   pointer kept.
#pragma optimize("s", on)
#pragma optimize("y", off)
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_LIST_BASE_OUTOFLINE
#include "stlport/fable_containers.h"

struct SExpSpendUndo {
    std::list<long> Generals;                   // +0x00
    std::list<long> Strengths;                  // +0x04
    std::list<long> Skills;                     // +0x08
    std::list<long> Wills;                      // +0x0c
    long Levels;                                // +0x10
    long General;                               // +0x14
    long Strength;                              // +0x18
    long Skill;                                 // +0x1c
    long Will;                                  // +0x20
    long aGeneral;                              // +0x24
    long aStrength;                             // +0x28
    long aSkill;                                // +0x2c
    long aWill;                                 // +0x30
    SExpSpendUndo();
};

SExpSpendUndo::SExpSpendUndo()
    : Levels(0)
{
    General = Strength = Skill = Will = 0;
    aGeneral = aStrength = aSkill = aWill = 0;
}
