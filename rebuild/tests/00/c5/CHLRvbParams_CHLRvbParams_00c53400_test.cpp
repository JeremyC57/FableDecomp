// 00c53400 CHLRvbParams::CHLRvbParams
// Self-contained: exact production source and complete shared original class.
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <new>
// 00c53400 CHLRvbParams::CHLRvbParams
// Leaf: ??0CHLRvbParams@@QAE@XZ
// Flags: none
// Retail empty constructor; SDK 2.10 initializes defaults instead. See NAMING_EVIDENCE.md.
#define FABLE_RETAIL_PASS22_FLOATREVERB_CREATE
#include "engine/CHLRvbParams.h"

CHLRvbParams::CHLRvbParams()
{
}

int main()
{
    assert(sizeof(CHLRvbParams) == 104);
    assert(sizeof(_RVB_VECTOR) == 12);
    unsigned int cases = 0;
    for (unsigned int pattern = 0; pattern != 256; ++pattern) {
        // Global operator new supplies real object alignment; the 16-byte prefix preserves it.
        unsigned char* storage = static_cast<unsigned char*>(::operator new(sizeof(CHLRvbParams) + 32));
        memset(storage, pattern, sizeof(CHLRvbParams) + 32);
        unsigned char before[sizeof(CHLRvbParams) + 32];
        memcpy(before, storage, sizeof(before));
        CHLRvbParams* object = new (storage + 16) CHLRvbParams;
        assert(static_cast<void*>(object) == static_cast<void*>(storage + 16));
        // Inspect object representation through unsigned char, never an indeterminate scalar member.
        assert(memcmp(storage, before, sizeof(before)) == 0);
        if (pattern == 0)
            assert(object->GetDecayTime() == 0.0f);
        object->~CHLRvbParams();
        assert(memcmp(storage, before, sizeof(before)) == 0);
        ::operator delete(storage);
        ++cases;
    }
    { CHLRvbParams normal; assert(sizeof(normal) == 104); }
    for (unsigned int count = 0; count != 65; ++count) {
        CHLRvbParams* objects = new CHLRvbParams[count];
        if (count > 1)
            assert(reinterpret_cast<unsigned char*>(objects + 1) - reinterpret_cast<unsigned char*>(objects) == 104);
        delete[] objects;
        ++cases;
    }
    assert(cases == 321);
    puts("00c53400_TEST PASS");
    return 0;
}