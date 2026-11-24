// CParentDefClassBase::InternalLoadBinary(CDataInputStream& is) -- load counted sub-definition records and leave the map clean.
// Leaf: ?InternalLoadBinary@CParentDefClassBase@@UAEXAAVCDataInputStream@@@Z
// Flags: (none)
// Inline-budget: existing forced Insert/Read wrappers retain the native expanded operations.
// ABI: owned 009B2550 and 009B60D0 require the original fastcall STLport dispatch helpers.
// Evidence: Ego_d locals/scopes and source lines1582..1595; both release PDB member declarations.
#define FABLE_RETAIL_STLP_PASS22_FLARE_ABI
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_PDCB_TYPED_MEMBERS
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CDATAINPUTSTREAM
#define FABLE_RETAIL_CDATAINPUTSTREAM_ORIGINAL_READ_SCOPE
#define FABLE_RETAIL_CVECTORMAP_INSERT_KEYVALUE
#define FABLE_RETAIL_CVECTORMAP_SETDIRTY
#include "fable_string.h"
#include "engine/CDefString.h"
#include "engine/CDefClassBase.h"
#include "engine/CVectorMap.h"
#include "engine/CParentDefClassBase.h"
#include "engine/CDataInputStream.h"
void CParentDefClassBase::InternalLoadBinary(CDataInputStream& is)
{
    CDefClassBase::InternalLoadBinary(is);
    unsigned long no_subs=is.ReadUWORD();
    SubDefInfoMap.reserve(no_subs);
    for (unsigned long c_sub=0;c_sub<no_subs;++c_sub) {
        CSubDefInfo sub_def_info;
        unsigned long name_crc=is.ReadULONG();
        sub_def_info.DefIndex=is.ReadSLONG();
        sub_def_info.OriginalParentDefIndex=is.ReadSLONG();
        SubDefInfoMap.Insert(name_crc,sub_def_info);
    }
    SubDefInfoMap.SetDirty(false);
}