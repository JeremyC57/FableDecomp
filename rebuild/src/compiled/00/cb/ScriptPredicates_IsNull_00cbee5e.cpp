// ScriptPredicates::IsNull(const CCharString& text) -- Compare the token without regard to case.
// Leaf: ?IsNull@@YI_NABVCCharString@@@Z
// Flags: /O1 /Oy-
// Identity: original global fastcall IsNull in both PDB name sets; own literal null and actual MSVCR71 _stricmp thunk00bfeba8, not strcmp. ScriptPredicates is a descriptive catalog grouping only, function has global scope.
// Layout: original CCharString pointer to character-data pointer; no active engine header. Exact original constructor(const char*,long)99ebf0 and destructor99eae0.
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __forceinline inline
#endif
extern "C" int __cdecl _stricmp(const char*,const char*);
class CCharStringData{public:char* PData;};
class CCharString{public:CCharStringData* PStringData;CCharString():PStringData(0){}CCharString(const char*,long=-1);~CCharString();const char* GetAsCharArray()const{return PStringData==0?"":PStringData->PData;}__forceinline bool operator==(const CCharString& other)const{return _stricmp(GetAsCharArray(),other.GetAsCharArray())==0;}};
bool __fastcall IsNull(const CCharString& text){CCharString expected("null");return text==expected;}