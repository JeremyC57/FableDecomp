// global::operator+(const CCharString& left,const CCharString& right) -- Copy the left string, append the right value and return the result.
// Leaf: ??H@YI?AVCCharString@@ABV0@0@Z
// Flags: /Gr
// Identity: original PDB free fastcall overload; own append0099f0a0 and UnassignString0099e9b0. Counter binding 013bd800 is descriptive, not a recovered original variable name.
// Layout: Original packed CBasicString<char>13 plus NoRefs+13, PStringData sole pointer.
#include <stddef.h>
#include <stdlib.h>
#ifndef _MSC_VER
#define __fastcall
#define __forceinline inline
#define __declspec(x)
#endif
#pragma pack(push,1)
template<class T>class CBasicString{public:T* PData;unsigned long StringLength;unsigned long DataLength:31;bool UseFastExtend:1;};
class CCharStringData{public:CBasicString<char> Data;long NoRefs;void IncRef(){++NoRefs;}};
#pragma pack(pop)
extern long g_CCharStringInstanceCount;
class CCharString{public:CCharStringData* PStringData;
CCharString():PStringData(0){++g_CCharStringInstanceCount;}
CCharString(const CCharString& other):PStringData(0){++g_CCharStringInstanceCount;if(&other!=this){if(PStringData)UnassignString();if(other.PStringData){PStringData=other.PStringData;PStringData->IncRef();}}}
~CCharString(){UnassignString();--g_CCharStringInstanceCount;}
protected:void UnassignString();
public:const CCharString& operator+=(const CCharString&);
};
CCharString __fastcall operator+(const CCharString& left,const CCharString& right){CCharString result(left);result+=right;return result;}