#pragma once
#ifdef FABLE_RETAIL_CPERSISTCONTEXT
// Retail opt-in (Codex pass 11 consolidation): the complete 44-byte Ego_r CPersistContext (engine/CPersistContext.h).
#include "engine/CPersistContext.h"
#else

#include "rebuild_abi.h"

class CPersistContext
{
public:
    template <class T>
    void Transfer(const char* name, T& value, const T& defaultValue);

    void TransferSignedChar(
        const char* name,
        signed char& value,
        const signed char& defaultValue);
};
#endif
