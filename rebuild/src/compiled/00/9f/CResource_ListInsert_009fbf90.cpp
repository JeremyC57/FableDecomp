// CResource::ListInsert(CResource*, CResource*)
// Leaf: ?ListInsert@CResource@@AAEXPAV1@0@Z
// Name (NF143, q4943): the private member ?ListInsert@CResource@@AAEXPAV1@0@Z (Ego_r.pdb 0x00B6BE40 + FableWin.pdb 0x03176F70; donor order
//   ListInsert, ListRemove = retail 0x009FBF90, 0x009FBFB0), declared by engine/CResource.h (FABLE_RETAIL_CRESOURCE_TOUCHRESOURCE, parameter
//   names prev / next); was the stand-in CResource_Methods::ListInsert.
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#define FABLE_RETAIL_CRESOURCE_TOUCHRESOURCE
#include "engine/CResource.h"  // retyped onto the PDB layout; byte parity re-verified

void CResource::ListInsert(CResource* prev, CResource* next)
{
    this->PrevResource = prev;
    this->NextResource = next;
    prev->NextResource = this;
    this->NextResource->PrevResource = this;
}
