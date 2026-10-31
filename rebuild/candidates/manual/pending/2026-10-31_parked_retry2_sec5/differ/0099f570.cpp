// global::operator+(const CCharString& left,const CCharString& right) -- Copy the left string, append the right value and return the result.
// Leaf: ??H@YI?AVCCharString@@ABV0@0@Z
// Grade: DIFFER(130v130) -- length now matches: `CCharString result; result = left; result += right;` (default ctor +
//   inline operator= with the load / +1 / store refcount, as the sibling overloads 0x0099F600 / 0x0099F7A0); residue is
//   register choice only: other.PStringData lands in ecx, retail reuses edx (the &left register)
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
class CCharStringData{public:CBasicString<char> Data;long NoRefs;};
#pragma pack(pop)
extern long g_CCharStringInstanceCount;
class CCharString{public:CCharStringData* PStringData;
CCharString():PStringData(0){++g_CCharStringInstanceCount;}
CCharString(const CCharString& other):PStringData(0){++g_CCharStringInstanceCount;if(&other!=this){if(PStringData)UnassignString();CCharStringData* data=other.PStringData;if(data){PStringData=data;++data->NoRefs;}}}
__forceinline CCharStringData* GetStringData() const{return PStringData;}
CCharString& operator=(const CCharString& other){if(&other!=this){if(PStringData)UnassignString();if(other.PStringData){long refs=other.PStringData->NoRefs+1;PStringData=other.PStringData;PStringData->NoRefs=refs;}}return *this;}
~CCharString(){UnassignString();--g_CCharStringInstanceCount;}
protected:void UnassignString();
public:const CCharString& operator+=(const CCharString&);
};
CCharString __fastcall operator+(const CCharString& left,const CCharString& right){CCharString result;result=left;result+=right;return result;}