// AreAllThingsInVectorAlive(const vector<CScriptThing>& things) -- true when every script thing in the vector is alive
#pragma optimize("y", off)
#pragma optimize("s", on)
// Leaf: ?AreAllThingsInVectorAlive@@YI_NABV?$vector@VCScriptThing@@V?$allocator@VCScriptThing@@@std@@@std@@@Z
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CBED48, 58 bytes, `__fastcall` global (vector in ecx), plain `ret`
//   (PDB ?AreAllThingsInVectorAlive@@YI_NABV?$vector@VCScriptThing@@...@Z). No frame: everything lives in registers.
// Callees: CScriptThing (vtable 0x01238C8C) slot 75 IsAlive (+0x12c).
// Shape: the twin of AreAllThingsInVectorDead 0x00CBED00: count the living things (begin read once, end re-read every
//   pass), compare the count with size() (`push 0Ch ; pop ecx ; cdq ; idiv`), return through `sete`.
#include "engine/CCountedPointer.h"
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
namespace std {
template <class T> class allocator { };
template <class T, class A = allocator<T> > class vector {
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    const T* begin() const { return _M_start; }
    const T* end() const { return _M_finish; }
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
};
}

bool __fastcall AreAllThingsInVectorAlive(const std::vector<CScriptThing>& things)
{
    unsigned int alive = 0;
    for (const CScriptThing* it = things.begin(); it != things.end(); ++it)
        if (it->IsAlive())
            ++alive;
    return alive == things.size();
}