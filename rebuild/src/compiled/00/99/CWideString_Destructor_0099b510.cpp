#define FABLE_RETAIL_CWIDESTRING_NOSTRINGS
#include "fable_string.h"
// Leaf: ??1CWideString@@QAE@XZ

#include <stdlib.h>

CWideString::~CWideString()
{
    CWideStringData* storage = storage_;

    if (storage != 0)
    {
        --storage->owners;
        if (storage->owners <= 0)
        {
            storage = storage_;
            if (storage != 0)
            {
                if (storage->text != 0)
                    free(storage->text);
                delete storage;
            }
        }
        storage_ = 0;
    }

    --NoStrings;                                   // 0x013BCA20 ?NoStrings@CWideString@@1JA
}