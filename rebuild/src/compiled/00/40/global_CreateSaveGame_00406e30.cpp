// 00406e30 Row00406e30::Create
// Flags: /O2 /Oy /W3 
// Leaf: Create
#include <wchar.h>
extern "C" int __cdecl _wchdir(const wchar_t*);
namespace Row00406e30 {
struct Wide{void*p;Wide(const wchar_t*);Wide(const Wide&);~Wide();const wchar_t*Get()const;};
// ABI: 0x00406350 forwards EDX to Wide copy ctor 0x0099b720; callers pass profile+0x8 wrapper.
Wide __fastcall Valid(const Wide&);Wide __fastcall SaveDir();Wide __fastcall Join(const Wide&,const Wide&);Wide __fastcall Append(const Wide&,const wchar_t*);bool __fastcall Exists(const Wide&);
struct Drive{int Create(const Wide&);};extern Drive*Manager;
int __fastcall Create(void*,const Wide&profile,int mode,int unused,wchar_t*out,int capacity){
 Wide valid=Valid(profile);Wide path=Append(Join(SaveDir(),valid),L"\\");
 wcsncpy(out,path.Get(),capacity-1);out[capacity-1]=0;
 if(mode==3){bool missing;{Wide local(out);missing=!Exists(local);}if(missing)return 3;}
 else{bool failed;{Wide local(out);failed=Manager->Create(local)!=1;}if(failed)return 15;_wchdir(L"..");}
 return 0;
}
}