// NUISystem::CUIMiscThingsDef::CUIMiscThingsDef() -- default constructor: CResource and the inline def-base defaults, the vtable, every UI string, the two maps and the music string
// Leaf: ??0CUIMiscThingsDef@NUISystem@@QAE@XZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x004304E0, 1039 bytes, thiscall, `ret`. Name: Ego_r.pdb and FableWin.pdb both list
//   ??0CUIMiscThingsDef@NUISystem@@QAE@XZ; its only retail caller is the factory NUISystem::CUIMiscThingsDef::Alloc 0x00432C40
//   (new 0x260 bytes, then this ctor), the same TU as the landed implicit destructor 0x004317D0 (this source is that row's
//   source with the constructor opt-ins below).
// Shape: no EH frame (the TU is built without /EHsc, as the landed destructor). CResource::CResource 0x009FBEC0 is called,
//   then the inline CDefClassBase ctor (FableWin ??0CDefClassBase@@AAE@XZ: PDefManager 0, flag bits cleared), the inline
//   CParentDefClassBase ctor (its out-of-line COMDAT is 0x00430370: InstantiationName -1, the sub-def CVectorMap zeroed and
//   its Dirty byte cleared by the FableWin CVectorMap default ctor), only the derived vtable store survives, then the members
//   in declaration order: 66 CWideString / 14 CCharString default ctors (0x0099AED0 / 0x0099E4B0) and the two STLport map
//   headers (malloc 0x18 node, red colour, null parent, left/right to itself, count 0).
// Headers: FABLE_RETAIL_DEF_CLASS_BASE_CTORS (CDefClassBase.h / CParentDefClassBase.h) and FABLE_RETAIL_CVECTORMAP_CTOR
//   (CVectorMap.h) are this chunk's opt-ins; the default text is unchanged.
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CPARENTDEFCLASSBASE
#define FABLE_RETAIL_DEF_CLASS_BASE_CTORS
#define FABLE_RETAIL_CVECTORMAP_CTOR
#include "engine/NUISystem_CUIMiscThingsDef.h"
// The original factory (PDB ?Alloc@CUIMiscThingsDef@NUISystem@@SIPAVCDefClassBase@@XZ, retail 0x00432C40) instantiates the
// implicit constructor and destructor in this TU.
CDefClassBase* __fastcall NUISystem::CUIMiscThingsDef::Alloc()
{
    return new CUIMiscThingsDef;
}