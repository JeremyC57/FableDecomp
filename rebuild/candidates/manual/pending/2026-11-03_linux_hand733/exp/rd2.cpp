#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDATAINPUTSTREAM
#include "fable_string.h"
#include "engine/CDataInputStream.h"
unsigned long g(CDataInputStream& s)
{
    return s.ReadULONG();
}
unsigned long h(CDataInputStream& s)
{
    return s.ReadType<unsigned long>();
}
