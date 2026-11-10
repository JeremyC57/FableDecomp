#ifndef FABLE_ENGINE_CTHINGFILTER_HASSCRIPTNAME_H
#define FABLE_ENGINE_CTHINGFILTER_HASSCRIPTNAME_H
// New header (chunk 1010, row 0x008930F0 CGameScriptInterface::GetThingWithScriptName). Ego_r.pdb CThingFilter_HasScriptName,
// 4 bytes: CCharString Name +0x0 (struct_layouts_egor.tsv); ??RCThingFilter_HasScriptName@@QBE_NABVCThing@@@Z out of line
// (retail 0x006A7E30, landed binding). The constructor copies the name (CCharString copy ctor 0x0099EC30, inline); the implicit
// destructor runs ~CCharString 0x0099EAE0. The includer supplies the complete CCharString (fable_string.h) first.
#include "stlport/fable_containers.h"
class CThing;
class CThingFilter_HasScriptName : public std::unary_function<CThing,bool> {
private:
    CCharString Name;
public:
    CThingFilter_HasScriptName(const CCharString& script_name):Name(script_name){}
    bool operator()(const CThing& thing) const;                          // 0x006A7E30
};
#endif
