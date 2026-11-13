#ifndef FABLE_ENGINE_CPERSISTCONTEXTWRITESECTION_H
#define FABLE_ENGINE_CPERSISTCONTEXTWRITESECTION_H
// NEW (chunk 2052, rows 0x0049F4C0 / 0x004A21F0): Ego_r.pdb CPersistContextWriteSection (struct_layouts_egor.tsv: Name CCharString +0,
// StartPosition long +4, sizeof 8) with its inline constructor / destructor (??0CPersistContextWriteSection@@QAE@XZ /
// ??1CPersistContextWriteSection@@QAE@XZ in Ego_r.pdb; expanded in retail: CCharString() 0x0099E4B0 on +0, `mov [section+4],0`,
// ~CCharString 0x0099EAE0 at scope end, 0x0049F532 / 0x0049FF5C).
#include "fable_string.h"
class CPersistContextWriteSection {
public:
    CPersistContextWriteSection() : StartPosition(0) {}
    ~CPersistContextWriteSection() {}
    CCharString Name;                                        // +0x00
    long        StartPosition;                               // +0x04
};
FABLE_STATIC_ASSERT(sizeof(CPersistContextWriteSection) == 8);
#endif
