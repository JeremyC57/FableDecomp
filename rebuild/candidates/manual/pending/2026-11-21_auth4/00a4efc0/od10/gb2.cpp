#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_STLP_VECTOR_C
#define FABLE_RETAIL_STLP_COPY
#define FABLE_RETAIL_CSIMPLEMEMORYPOOL
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_CARRAY_INDEX_ASSERT
#define FABLE_RETAIL_ASSERT_DEAD_ARM
#define FABLE_RETAIL_CSIMPLEMEMORYPOOL_ALLOCATETYPE_IFBODY
#include "fable_assert.h"
#define FABLE_RETAIL_CARRAY_GETBYTESIZE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_ISNONZERO_ZYX

#define FABLE_RETAIL_CMATRIX3X4
#define FABLE_RETAIL_CCHUNKEDFILECHUNK
#define FABLE_RETAIL_CCHUNKEDFILESUPERCHUNK
#define FABLE_RETAIL_C3DANIMFILESEQUENCECHUNK_ADDMATRIX
#define FABLE_RETAIL_C3DANIMFILESEQUENCECHUNK_GETTERS
#define FABLE_RETAIL_C3DANIMATIONSEQUENCEDATA_FROMFILEDATA
#define FABLE_RETAIL_C3DANIMATIONSEQUENCEDATA_VECTORWORD_FROMVECTOR
#define FABLE_RETAIL_C3DKEYFRAME_FROMMATRIX
#define FABLE_RETAIL_C3DKEYFRAME_GETROTATION
#define FABLE_RETAIL_CQUATERNION_EQUALS
#include <string.h>
#include <math.h>
#include "fable_compiler.h"
#include "fable_string.h"
#include "stlport/fable_containers.h"
struct TB { int pad[13]; CCharString BoneName;
 const CCharString g0() const; const CCharString g1() const; const CCharString g2() const; CCharString g3() const; };
struct TC { int pad[13]; const char* BoneName; const CCharString g4() const; };
struct TD { int pad[13]; char BoneName[32]; const CCharString g5() const; };
const CCharString TB::g0() const
{
 return BoneName;
}
const CCharString TB::g1() const
{
 return CCharString(BoneName.GetAsCharArray());
}
CCharString TB::g3() const
{
 return BoneName;
}
const CCharString TC::g4() const
{
 return BoneName;
}
const CCharString TD::g5() const
{
 return BoneName;
}
