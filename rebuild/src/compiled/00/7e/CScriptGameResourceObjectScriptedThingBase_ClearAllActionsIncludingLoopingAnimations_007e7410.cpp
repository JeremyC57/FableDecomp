struct Inner; typedef void (__fastcall* VFn)(Inner*);
struct Inner { void** vtbl; };
struct CScriptGameResourceObjectScriptedThingBase { char pad[8]; Inner* inner; };
void __fastcall ClearAllActionsIncludingLoopingAnimations(CScriptGameResourceObjectScriptedThingBase* thing)
{
    Inner* p = thing->inner;
    if (p)
    {
        VFn fn = *reinterpret_cast<VFn*>(*reinterpret_cast<char**>(p) + 0x58);
        fn(p);
    }
}