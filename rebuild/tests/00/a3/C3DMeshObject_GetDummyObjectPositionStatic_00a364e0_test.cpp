#if !defined(_MSC_VER)
#define FABLE_PORTABLE
#define FABLE_RETAIL_PASS23_HOST_TEST_ABI
#define FABLE_RETAIL_STLP_HOST_TAGS_PASS23
#define __int64 long long
#define __fastcall
#define __cdecl
#define __stdcall
#define __declspec(x)
#include <strings.h>
#define _stricmp strcasecmp
#endif
// 00a364e0 C3DMeshObject::GetDummyObjectPositionStatic
// Leaf: ?GetDummyObjectPositionStatic@C3DMeshObject@@QBEXABVCCharString@@JAAVC3DVector@@11@Z
// Flags: none
#define FABLE_RETAIL_C3DMESHOBJECT_STATIC_DUMMY_PASS26
#define FABLE_RETAIL_CMATRIX3X4_BILLBOARD
#define FABLE_RETAIL_CMATRIX3X4_SETMULTIPLY
#define FABLE_RETAIL_CMATRIX3X4_SETMULTIPLY_PREMULTIPLY
#define FABLE_RETAIL_CMATRIX3X4_SETMULTIPLY_POSTMULTIPLY
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_GETCRC_INLINE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CMATRIX3X4
#define FABLE_RETAIL_CMATRIX3X4_TRANSFORMVECTOR_SOURCE_DEST_INLINE
#define FABLE_RETAIL_C3DMESHSTATS_DUMMYOBJECTS
#define FABLE_RETAIL_CMOVABLERESOURCE
#define FABLE_RETAIL_CMATRIX3X4_TRANSFORMPOINT_ZYX_SUMS
#define FABLE_RETAIL_C3DMESHSTATS_ISANIMATED
#define FABLE_RETAIL_ASSERT_DEAD_ARM
#define FABLE_RETAIL_C3DVECTOR_NORMALISE_SQUARE_HELPER
#define FABLE_RETAIL_C3DVECTOR_SQUARE_ORIGINAL_FORMAL
#define FABLE_RETAIL_GFFASTONEOVERSQRT_ORIGINAL_UNION
#define FABLE_RETAIL_GFFASTONEOVERSQRT_HOST_BITS_PASS23
#define FABLE_RETAIL_C3DVECTOR_NORMALISE_GROUPED_SUM
#define FABLE_RETAIL_MESH_DUMMY_INVERSE_STEP_PASS26
#include "fable_assert.h"
#include "engine/CCRC.h"
#include "engine/C3DVector.h"
#include "engine/C3DDummyObject.h"
#include "engine/C3DMeshStats.h"
#include "engine/C3DMeshObject.h"

void C3DMeshObject::GetDummyObjectPositionStatic(const CCharString& name, long n, C3DVector& dummy_pos,
                                                C3DVector& dummy_direction, C3DVector& dummy_up) const
{
    FABLE_RETAIL_ASSERT(!Mesh->IsAnimated());
    const C3DDummyObject* dummy = Mesh->GetDummyObject(name.GetCRC(), n);
    FABLE_RETAIL_ASSERT(dummy);
    dummy_pos = dummy->GetPosition();
    PositionMatrix.TransformPoint(dummy_pos);
    CMatrix3x4 mat = dummy->PeekTransform();
    mat.PostMultiply(PositionMatrix);
    dummy_direction.Initialise(mat.E21, mat.E22, mat.E23);
    dummy_up.Initialise(mat.E31, mat.E32, mat.E33);
    dummy_direction.Normalise();
    dummy_up.Normalise();
}

#include <math.h>
#include <stdio.h>
#include <string.h>

CBaseClass::CBaseClass() {}
CBaseClass::~CBaseClass() {}
bool CMovableResource::CanRelocateData(long) const { return true; }
unsigned long CMovableResource::Verify() const { return 123; }
CCharString::CCharString() : PStringData(0) {}
const char CCharString::NullChar = 0;

class TestText : public CBasicString<char> {
public:
    TestText(const char* text, size_t size) {
        PData = const_cast<char*>(text);
        StringLength = static_cast<unsigned long>(size);
        DataLength = StringLength;
        UseFastExtend = false;
    }
};
CCharString::CCharString(const char* text, long length) : PStringData(new CCharStringData) {
    PStringData->Data = TestText(text, length < 0 ? strlen(text) : (size_t)length);
}
CCharString::~CCharString() { delete PStringData; }

static const char* expectedText;
static unsigned long expectedIndex;
static unsigned long lookupCalls, crcCalls, failures;
#ifdef _MSC_VER
bool FABLE_FASTCALL GFAssertFailed(const char*,const char*,const char*,unsigned long) { ++failures; return false; }
#endif
static CMatrix3x4 dummyMatrix, worldMatrix;
static C3DDummyObject* selectedDummy;
unsigned long FABLE_FASTCALL CCRC::Calc(unsigned long seed, const void* data, unsigned long size) {
    ++crcCalls;
    if (seed || !data || size != strlen(expectedText) || memcmp(data, expectedText, size)) ++failures;
    return 0x79f043a1UL;
}
const C3DDummyObject* C3DMeshStats::GetDummyObject(unsigned long crc, unsigned long index) const {
    ++lookupCalls;
    if (crc != 0x79f043a1UL || index != expectedIndex) ++failures;
    return selectedDummy;
}
C3DDummyObject::C3DDummyObject() : Transform(dummyMatrix), BoneIndex(0) {}
class TestMesh : public C3DMeshStats {
public:
    void RelocateData(long,void*,void*,long) { ++failures; }
    C3DVector GetHelperPointPositionAnimated(const C3DHelperPoint&,const C3DAnimationBlendState&,C3DAnimationManager&)const { ++failures; return C3DVector(0,0,0); }
    C3DVector GetHelperPointPositionAnimated(const C3DHelperPoint&,const C3DAnimationBlendState&,const C3DAnimationBlendState&,float,C3DAnimationManager&)const { ++failures; return C3DVector(0,0,0); }
    void GetDummyObjectPositionAnimated(const C3DDummyObject&,C3DDummyObject&,const C3DAnimationBlendState&,C3DAnimationManager&,bool)const { ++failures; }
    void GetDummyObjectPositionAnimated(const C3DDummyObject&,C3DDummyObject&,const C3DAnimationBlendState&,const C3DAnimationBlendState&,float,C3DAnimationManager&,bool)const { ++failures; }
    void GetDummyObjectTransformAnimated(const C3DDummyObject&,CMatrix3x4&,const C3DAnimationBlendState&,C3DAnimationManager&,bool)const { ++failures; }
    void GetDummyObjectTransformAnimated(const C3DDummyObject&,CMatrix3x4&,const C3DAnimationBlendState&,const C3DAnimationBlendState&,float,C3DAnimationManager&,bool)const { ++failures; }
};
C3DMeshObject::C3DMeshObject() : Mesh(new TestMesh), PositionMatrix(worldMatrix) {}
C3DMeshObject::~C3DMeshObject() {}

unsigned long GFastOneOverSqrtTable[128];
static void prepareTestTable() {
    for (unsigned int i = 0; i < 128; ++i) {
        float value = (i < 64 ? 2.0f : 1.0f) * (1.0f + ((i & 63) + 0.5f) / 64.0f);
        float inverse = 1.0f / (float)sqrt(value);
        unsigned int bits;
        memcpy(&bits, &inverse, sizeof(float));
        GFastOneOverSqrtTable[i] = (bits & 0x7fffffu) >> 15;
    }
}
static void setMatrix(CMatrix3x4& m, const float data[12]) {
    m.E11=data[0]; m.E12=data[1]; m.E13=data[2];
    m.E21=data[3]; m.E22=data[4]; m.E23=data[5];
    m.E31=data[6]; m.E32=data[7]; m.E33=data[8];
    m.E41=data[9]; m.E42=data[10]; m.E43=data[11];
}
static void normalizeOracle(float v[3]) {
    double length = sqrt((double)v[0]*v[0] + (double)v[1]*v[1] + (double)v[2]*v[2]);
    if (length)
        for (int k=0; k<3; ++k) v[k] = (float)(v[k]/length);
}
int main() {
    prepareTestTable();
    const float matrices[][12] = {
        {1,0,0,0,1,0,0,0,1,0,0,0}, {0,2,0,-4,0,0,0,0,0.5f,8,-4,2},
        {1,-2,4,0.5f,2,1,-4,2,-0.5f,-8,4,2}, {0,0,0,0,0,0,0,0,0,2,8,4},
        {-0.125f,0,0,0,0.25f,0,0,0,-0.5f,16,0,4}
    };
    const char* names[] = {"", "", "head", "left_hand"};
    const int destinations[][3] = {{0,1,2},{0,0,2},{0,1,0},{0,1,1},{0,0,0}};
    for (int d=0; d<5; ++d) for (int w=0; w<5; ++w)
    for (int title=0; title<4; ++title) for (int n=0; n<3; ++n)
    for (int alias=0; alias<5; ++alias) {
        setMatrix(dummyMatrix,matrices[d]); setMatrix(worldMatrix,matrices[w]);
        C3DDummyObject dummy; selectedDummy=&dummy;
        C3DMeshObject mesh;
        expectedText=names[title]; expectedIndex=n;
        CCharString nullData;
        CCharString allocatedData(expectedText);
        const CCharString& key = title ? allocatedData : nullData;
        const int* dst = destinations[alias];
        C3DVector got[3]; float expected[3][3];
        for (int i=0; i<3; ++i) {
            got[i].Initialise(16,32,64); expected[i][0]=16; expected[i][1]=32; expected[i][2]=64;
        }
        const int offsets[3]={9,3,6};
        for (int out=0; out<3; ++out) for (int axis=0; axis<3; ++axis) {
            float sum = out == 0 ? matrices[w][9+axis] : 0.0f;
            for (int j=0;j<3;++j) sum += matrices[d][offsets[out]+j]*matrices[w][3*j+axis];
            expected[dst[out]][axis]=sum;
        }
        normalizeOracle(expected[dst[1]]); normalizeOracle(expected[dst[2]]);
        mesh.GetDummyObjectPositionStatic(key,n,got[dst[0]],got[dst[1]],got[dst[2]]);
        for (int v=0;v<3;++v) {
            const float actual[]={got[v].X,got[v].Y,got[v].Z};
            for (int k=0;k<3;++k)
                if (!(fabs(actual[k]-expected[v][k])<=0.0003)) return 1;
        }
    }
    if (lookupCalls!=1500 || crcCalls!=1500 || failures) return 2;
    puts("00a364e0_TEST PASS");
    return 0;
}