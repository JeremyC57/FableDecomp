// global::operator+(const CCharString& left,const char* right) -- Copy the left string, append the right value and return the result.
// Leaf: ??H@YI?AVCCharString@@ABV0@PBD@Z
// Flags: /Gr
// Identity: original PDB free fastcall overload; own append0099f100 and UnassignString0099e9b0. Counter binding 013bd800 is descriptive, not a recovered original variable name.
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
class CCharStringData{public:CBasicString<char> Data;long NoRefs;};
#pragma pack(pop)
extern long g_CCharStringInstanceCount;
class CCharString{public:CCharStringData* PStringData;
CCharString():PStringData(0){++g_CCharStringInstanceCount;}
CCharString(const CCharString& other):PStringData(0){++g_CCharStringInstanceCount;if(&other!=this){if(PStringData)UnassignString();CCharStringData* data=other.PStringData;if(data){PStringData=data;++data->NoRefs;}}}
CCharString& operator=(const CCharString& other){if(&other!=this){if(PStringData)UnassignString();CCharStringData* data=other.PStringData;if(data){long refs=data->NoRefs+1;PStringData=data;data->NoRefs=refs;}}return *this;}
~CCharString(){UnassignString();--g_CCharStringInstanceCount;}
protected:void UnassignString();
public:const CCharString& operator+=(const char*);
};
CCharString __fastcall operator+(const CCharString& left,const char* right){CCharString result;result=left;result+=right;return result;}