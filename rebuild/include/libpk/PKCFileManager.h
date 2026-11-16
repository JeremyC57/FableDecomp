#ifndef FABLE_LIBPK_PKCFILEMANAGER_H
#define FABLE_LIBPK_PKCFILEMANAGER_H
#ifdef FABLE_RETAIL_BANK_LOAD_WRAPPER_PASS23
#include "libpk/CAudioFile.h"
// Native base012b2fb8 has exactly Open,pointer-deleting-destructor. Default
// implementation012b30e4 slots C1ABB0/C1B6B0; factory member at owner+5c.
class PKCFileManager {public:
 virtual CAudioFile* Open(const unsigned short*)=0;
 virtual ~PKCFileManager(); // Native plain baseC19F60, deletingC19F70.
};
#endif
#endif
