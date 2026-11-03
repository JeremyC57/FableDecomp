#ifndef FABLE_ENGINE_CTHINGFILTER_HASDEFGLOBALINDEX_H
#define FABLE_ENGINE_CTHINGFILTER_HASDEFGLOBALINDEX_H
#include "rebuild_abi.h"
class CThing;
// Ego_r.pdb CThingFilter_HasDefGlobalIndex (struct_layouts_egor.tsv: sizeof 4, unsigned long DefGlobalIndex +0), class key V
// (??0CThingFilter_HasDefGlobalIndex@@QAE@K@Z, ??RCThingFilter_HasDefGlobalIndex@@QBE_NABVCThing@@@Z). The constructor is inline in
// retail (0x0074B7D7 stores the index straight into the filter's stack slot); operator() is out of line in Ego_r.
class CThingFilter_HasDefGlobalIndex {
private:
    unsigned long DefGlobalIndex;
public:
    CThingFilter_HasDefGlobalIndex(unsigned long def_global_index) : DefGlobalIndex(def_global_index) { }
    bool operator()(const CThing& thing) const;
};
FABLE_STATIC_ASSERT(sizeof(CThingFilter_HasDefGlobalIndex) == 4);
#endif
