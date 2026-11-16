#ifndef FABLE_LIBPK_PKCAUTOFILE_H
#define FABLE_LIBPK_PKCAUTOFILE_H
#ifdef FABLE_RETAIL_BANK_LOAD_WRAPPER_PASS23
#include "rebuild_abi.h"
#include "libpk/PKCFileManager.h"
// Original PKCAutoFile.cpp stem proved by C31DB0 assertion. Genuine4B owner:
// ctor C31D80 stores pointer; dtor C31D90 closes nonnull via file slot3.
class PKCAutoFile {public:
 PKCAutoFile(CAudioFile*);~PKCAutoFile();CAudioFile* GetChecked();CAudioFile* m_pFile;
};
struct CBankData;
// Actual PKCAutoDelete.h ownerC17740 holds one pointer and conditionally deletes.
class CBankDataOwner {public:
 explicit CBankDataOwner(CBankData* p):Pointer(p){}
 ~CBankDataOwner();
 CBankData* GetChecked();
 CBankData* Pointer;
};
FABLE_STATIC_ASSERT(sizeof(PKCAutoFile)==4);
#endif
#endif
