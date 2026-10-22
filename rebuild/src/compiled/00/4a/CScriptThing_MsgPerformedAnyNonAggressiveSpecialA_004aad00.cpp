struct CInner;
struct CInnerVtbl { char pad[0xa0]; bool (__fastcall *slot40)(CInner*); };
struct CInner { CInnerVtbl* vtbl; };
struct CScriptThing { void* pad0; CInner* inner; };

bool __fastcall CScriptThing_MsgPerformedAnyNonAggressiveSpecialAbility(CScriptThing* scriptThing)
{
    CInner* inner = scriptThing->inner;
    if (!inner)
        return false;
    return inner->vtbl->slot40(inner);
}