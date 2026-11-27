// 00b22670 NParticleEngine::CPSCSplineBase::DatabaseSerialise
// Leaf: ?DatabaseSerialise@CPSCSplineBase@NParticleEngine@@UAEXAAVCPersistContext@@ABVCEmitterDatabaseSerialiseInfo@2@@Z
// Flags: none
#define FABLE_RETAIL_STLP_FILL_NOINLINE
#define FABLE_RETAIL_STLP_UNINITIALIZED_FILL_N_DECL_ONLY
#define FABLE_RETAIL_STLP_RANDOM_ACCESS_COPY_BACKWARD
#define FABLE_RETAIL_STLP_COPY_BACKWARD_NOINLINE
#define FABLE_RETAIL_SPLINE_TRAITS_SIZE_PASS29
#define FABLE_RETAIL_CPERSISTTRAITS_LONG_BINARY_BODY
#define FABLE_RETAIL_STLP_UNINIT_COPY_FALSE_FASTCALL_PASS26
// Inline-budget: vector<C3DVector> growth expands the fill loop at 0066e370 and calls the suffix copy.
#define FABLE_RETAIL_STLP_UNINIT_FILL_N_FORCEINLINE
#define FABLE_RETAIL_STLP_VECTOR_ALLOCATE_COPY_DECL
#define FABLE_RETAIL_CPSCSPLINEBASE_SERIAL_PASS29
#define FABLE_RETAIL_CPSCSPLINEBASE_PASS23
#define FABLE_RETAIL_CPSCSPLINEBASE_MEMBERS
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_CONCAT_STRING
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CPERSISTCONTEXT
#define FABLE_RETAIL_CPERSISTCONTEXT_NAMED_DEFAULTS
#define FABLE_RETAIL_CPERSISTCONTEXT_GETDEFAULTVAL_BODY
#define FABLE_RETAIL_CPERSISTTRAITS
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
#define FABLE_RETAIL_STLP_VECTOR_RESIZE_OUTOFLINE
#define FABLE_RETAIL_STLP_VECTOR_OVERFLOW_OUTOFLINE
#define FABLE_RETAIL_STLP_VECTOR_C
#define FABLE_RETAIL_STLP_VECTOR_COPY_OUTOFLINE
#define FABLE_RETAIL_CARRAY_ORIGINAL_USER_SPECIAL_MEMBERS_PASS22
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_PASS22_CPSCLIGHT_DATABASE_SERIALISE
#include "rebuild_abi.h"
#include "stlport/fable_containers.h"
#include "fable_string.h"
#undef FABLE_RETAIL_PASS22_CPSCLIGHT_DATABASE_SERIALISE
#include "engine/fable_script_text.h"
#define FABLE_RETAIL_CMEMORYDATAINPUTSTREAM
#define FABLE_RETAIL_CDATAINPUTSTREAM
#define FABLE_RETAIL_CDATAOUTPUTSTREAM
#include "engine/CMemoryDataInputStream.h"
#include "engine/CDataOutputStream.h"
#define FABLE_RETAIL_PASS22_CPSCLIGHT_DATABASE_SERIALISE
#include "engine/CPersistTraits.h"
#undef FABLE_RETAIL_PASS22_CPSCLIGHT_DATABASE_SERIALISE
#define FABLE_RETAIL_CPERSISTCONTEXT_TRANSFER_BODY
#include "engine/CPersistContext.h"
#include "engine/CPSCSplineBase.h"

template<> void std::vector<unsigned long>::resize(size_type,const unsigned long&);
#ifdef _MSC_VER
#pragma optimize("y",off)
#pragma optimize("s",on)
#endif
template<> inline void CPersistContext::Transfer<C3DVector>(const char* name,C3DVector& value,const C3DVector& defaultValue)
{
 CheckCRC(name);
 switch(Mode) {
 case MODE_LOAD_TEXT: {
  if(UseDefaults)value=defaultValue;
  CPersistContext_TransferableComponent<C3DVector> item(value);
  TransferObjectLoadText(name,item);break;
 }
 case MODE_SAVE_TEXT: {
  CPersistContext_TransferableComponent<C3DVector> item(value);
  TransferObjectSaveText(name,item);break;
 }
 case MODE_LOAD_BINARY:{ CPersistTraits<C3DVector> traits;traits.TransferBinaryIn(*PLoadStream,value);break; }
 case MODE_SAVE_BINARY:PSaveStream->Write3DVector(value);break;
 default:value=defaultValue;break;
 }
}
template<> inline void CPersistContext::Transfer<C3DVector>(const char* name,C3DVector& value) {
 Transfer(name,value,GetDefaultVal<C3DVector>());
}
#ifdef _MSC_VER
#pragma optimize("",on)
#endif
namespace NParticleEngine {
void CPSCSplineBase::DatabaseSerialise(CPersistContext& context, const CEmitterDatabaseSerialiseInfo& info)
{
    float t = Spline.GetTension();
    context.Transfer("SplineTension", t);
    Spline.SetTension(t);
    bool end_points_connected = Spline.GetEndPointsConnected();
    context.Transfer("SplineConnectedEndPoints", end_points_connected);
    Spline.SetEndPointsConnected(end_points_connected);
    if (context.IsSaving()) {
        long size = Spline.GetControlPointCount();
        context.Transfer("SplineUserPointsArraySize_BugFixed", size);
        CArray<C3DVector> control_points = Spline.GetControlPoints();
        for (long ci = 0; ci < size; ++ci)
            context.Transfer((CCharString("SplineUserPointsArray_") + GFIntToCharString(ci)).GetAsCharArray(), control_points[ci]);
    } else {
        long size;
        CArray<C3DVector> control_points;
        context.Transfer("SplineUserPointsArraySize", size, 0L);
        if (size > 0) {
            if (size > 20) size = 20;
            control_points.resize(size);
            for (long ci = 0; ci < size; ++ci)
                context.Transfer("SplineUserPointsArray" + ci, control_points[ci]);
        } else {
            context.Transfer("SplineUserPointsArraySize_BugFixed", size);
            control_points.resize(size);
            for (long ci = 0; ci < size; ++ci)
                context.Transfer((CCharString("SplineUserPointsArray_") + GFIntToCharString(ci)).GetAsCharArray(), control_points[ci]);
        }
        C3DVector new_pt(0, 0, 0);
        while (control_points.size() < 2)
            control_points.push_back(new_pt);
        Spline.SetControlPoints(control_points);
    }
    bool offset_pos = !SplineRandomOffsetData.IsNull();
    context.Transfer("OffsetPos", offset_pos, false);
    if (offset_pos) {
        if (!context.IsSaving()) SplineRandomOffsetData.Reset(new CSplineRandomOffsetData(Spline));
        SplineRandomOffsetData->DatabaseSerialise(context, info);
    }
    if (!context.IsSaving()) SplinePosParamData.Reset(new CSplinePosParamData(Spline));
    SplinePosParamData->DatabaseSerialise(context, info);
    if (!SplinePosParamData->IsParamListValid()) SplinePosParamData.Reset(0);
}
}