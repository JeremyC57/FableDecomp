// NativeMatrix3x3::TransformVectorPair -- rotate two adjacent vectors with no translation
// Retail00705040,270bytes. Owner identity unresolved: propagated cloth ConvertVectorToLocal label is unsupported;
// actual cloth conversion is00B80AD0. ECX is a bounded36-byte3x3 matrix, argument is24-byte vector pair; RET4 establishes thiscall ABI.
// No actual C3DVector/matrix definition/header found in rebuild/include; native vectors are three floats,12bytes.
// Both input vectors are copied before output stores; the two matrix calculations precede pair assignment.
// The pair parameter is projected as a pointer to two vector elements, permitting defined overlap with matrix columns;
// pointer/reference identity is not recoverable from this ABI. Both are one stack address.
// Leaf: TransformVectorPair
#include "rebuild_abi.h"
struct NativeVector3 {float X,Y,Z;NativeVector3(){}NativeVector3(const NativeVector3& v):X(v.X),Y(v.Y),Z(v.Z){}};
struct NativeVectorPair {NativeVector3 First,Second;};
struct NativeMatrix3x3 {NativeVector3 Columns[3];
 __forceinline void Transform(NativeVector3& v)const{float x=v.X,y=v.Y,z=v.Z;v.X=Columns[0].X*x+Columns[1].X*y+Columns[2].X*z;v.Y=Columns[0].Y*x+Columns[1].Y*y+Columns[2].Y*z;v.Z=Columns[0].Z*x+Columns[1].Z*y+Columns[2].Z*z;}
 void TransformVectorPair(NativeVector3*)const;
};
FABLE_STATIC_ASSERT(sizeof(NativeVector3)==12);
FABLE_STATIC_ASSERT(sizeof(NativeVectorPair)==24);
FABLE_STATIC_ASSERT(sizeof(NativeMatrix3x3)==36);
void NativeMatrix3x3::TransformVectorPair(NativeVector3* pair)const{NativeVector3 first=pair[0];NativeVector3 second=pair[1];Transform(first);Transform(second);pair[0]=first;pair[1]=second;}