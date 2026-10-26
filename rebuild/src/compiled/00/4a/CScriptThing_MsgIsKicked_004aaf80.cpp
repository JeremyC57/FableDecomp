struct CInner;
struct CInnerVtbl { char pad[0xf0]; bool (__fastcall *IsKicked)(CInner*); };
struct CInner { CInnerVtbl* vtbl; };
class CScriptThing { public: void* pad0; CInner* inner; };

bool __fastcall CScriptThing_MsgIsKicked(CScriptThing* scriptThing)
{
    CInner* p = scriptThing->inner;
    if (!p)
        return false;
    return p->vtbl->IsKicked(p);
}