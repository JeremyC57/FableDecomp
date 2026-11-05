// CGameScriptInterface::GetTargetName(CScriptThing thing) -- the thing's CTCAIScratchpad::TargetName, or an empty string when the script thing has no thing
// Leaf: ?GetTargetName@CGameScriptInterface@@UAE?AVCCharString@@VCScriptThing@@@Z
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Retail: Fable.exe (TLC, VC7.1) @ 0x008a4840, 201 bytes, thiscall, `ret 10h` (the hidden result pointer and the 12-byte CScriptThing).
// Identity (proven): vtable 0x01260F0C (RTTI .?AVCGameScriptInterface@@) slot 803; ghidra_out/cgsi_slots_aligned.tsv (Ego_r.pdb vtable order) names it ?GetTargetName@CGameScriptInterface@@UAE?AVCCharString@@VCScriptThing@@@Z; it copies Ego_r.pdb CTCAIScratchpad::TargetName (+0x50) through interface 0xAA (CTCAIScratchpad::GetInterfaceType() is 170, landed).
// Callees: CCharString::CCharString(const char*, long) 0x0099EBF0 (the empty literal 0x0122D70E), CVectorMap<ETCInterfaceType,
//   CTCBase*, ...>::LowerBound 0x0040F020 (inlined GetInterface), CCharString::CCharString(const CCharString&) 0x0099EC30,
//   operator delete 0x00BFE9BC and CBaseClass::~CBaseClass 0x0099A2E0 (the parameter's inline ~CScriptThing: vtable
//   0x01238C8C, the CCountedPointer<CScriptThing> release, the base destructor).
// Body: CScriptThing::GetThing() is an inline virtual (`PImp.Data == 0 ? 0 : PImp.Data->GetPThing()`) called on the by-value object,
//   so it devirtualises and inlines twice; GetInterface's result is ignored (a miss reads the uninitialised scratchpad local).
// Header debt: engine/CScriptThing.h, CThing and engine/CTCAIScratchpad.h are flat layouts; modelled locally.
enum ETCInterfaceType { ETCInterfaceType_FIRST };
class CTCBase;
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing {
public:
    virtual ~CThing();
    unsigned char _pad04[0x1C];
    unsigned long PresentInterfaces[9];                    // +0x20 (the bitset of present interface types)
    CInterfaceMap Interfaces;                              // +0x44
    template <class T> __forceinline bool GetInterface(T** out, ETCInterfaceType type)
    {
        if (PresentInterfaces[type >> 5] & (1UL << (type & 31))) {
            ETCInterfaceType key = type;
            CInterfaceMap* map = &Interfaces;
            CInterfaceMap::value_type* it = map->LowerBound(key);
            if (it == map->end() || type < it->first)
                it = map->end();
            *out = (T*)it->second;
            return true;
        }
        return false;
    }
};
class CCharStringData;
class CCharString {
public:
    CCharStringData* PStringData;                          // +0x00
    CCharString(const char* text, long length);            // 0x0099EBF0
    CCharString(const CCharString& other);                 // 0x0099EC30
    ~CCharString();                                        // 0x0099EAE0
};
#include "engine/CCountedPointer.h"
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// Retail expands these at this row's sites (the local model had them in class): defined inline after the header.
inline CScriptThing::~CScriptThing() { }
inline CThing* CScriptThing::GetPThing() const { return PImp.Data == 0 ? 0 : PImp.Data->GetPThing(); }
class CTCAIScratchpad {
public:
    unsigned char _pad00[0x50];
    CCharString   TargetName;                                   // +0x50
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
CCharString CGameScriptInterface::GetTargetName(CScriptThing thing)
{
    if (thing.GetPThing() == 0)
        return CCharString("", -1);
    CTCAIScratchpad* scratchpad;
    thing.GetPThing()->GetInterface(&scratchpad, (ETCInterfaceType)0xAA);
    return scratchpad->TargetName;
}