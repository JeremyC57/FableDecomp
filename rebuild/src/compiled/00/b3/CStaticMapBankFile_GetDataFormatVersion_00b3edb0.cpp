// Leaf: ?GetDataFormatVersion@CStaticMapBankFile@@UAEKXZ
#define FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#define FABLE_RETAIL_CSTATICMAPBANKFILE_CLASS
#include "engine/CBankFileAsync.h"
#include "engine/CStaticMapBankFile.h"
unsigned long CStaticMapBankFile::GetDataFormatVersion()
{
    return 174;
}
// Flatten route (flatten_pilot v4, NF324): CStaticMapBankFile_GetDataFormatVersion -> CStaticMapBankFile::GetDataFormatVersion, the RTTI-placed PDB member; class model: engine/CStaticMapBankFile.h under FABLE_RETAIL_CSTATICMAPBANKFILE_CLASS (preamble of CEngineWorldMap_CreateNewBankMap_00b3ff70.cpp).
