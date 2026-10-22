#pragma optimize("s",on)
// scalar deleting destructor for CEngineSubPrimitiveText @ 00456d43
// idiom: push esi; mov esi,ecx; lea ecx,[esi+0x3c]; call member-dtor;
//        mov ecx,esi; call base/self dtor; test [esp+8],1; je; delete this; ret 4

extern void __fastcall MemberAt3C_dtor(void* p);   // dtor of member subobject at +0x3c
extern void __fastcall Self_dtor(void* p);         // the real (non-deleting) destructor
extern void __cdecl operator_delete(void* p);      // operator delete

void* __fastcall COpinionDeedMaskDef_scalar_deleting_destructor(char* def, int /*edx*/, unsigned int flags)
{
    MemberAt3C_dtor(def + 0x3c);
    Self_dtor(def);
    if (flags & 1)
        operator_delete(def);
    return def;
}