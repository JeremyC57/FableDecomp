#ifndef FABLE_ENGINE_CWIDESTRINGDATA_H
#define FABLE_ENGINE_CWIDESTRINGDATA_H
#include "stlport/fable_string.h"
// Ego_r TPI d45a/d454: protected Data, public NoRefs and const equality.
// Native47aa32..44 loads both start/finish pairs; native refcount is +12.
class CWideStringData {
protected:
    std::basic_string<unsigned short> Data;
public:
    long NoRefs;
    bool operator==(const CWideStringData& other)const { const std::basic_string<unsigned short>& other_data=other.Data;return Data==other_data; }
};
FABLE_STATIC_ASSERT(sizeof(CWideStringData)==16);
#endif
