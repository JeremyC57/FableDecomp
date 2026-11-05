// Leaf: ?CScriptGameResourceObjectScriptedThingBase_ClearAllActionsIncludingLoopingAnimations@@YIXPAVCScriptGameResourceObjectScriptedThingBase@@@Z
struct Inner; typedef void (__fastcall* VFn)(Inner*);
struct Inner { void** vtbl; };
class CScriptGameResourceObjectScriptedThingBase { public: char pad[8]; Inner* inner; };
void __fastcall CScriptGameResourceObjectScriptedThingBase_ClearAllActionsIncludingLoopingAnimations(CScriptGameResourceObjectScriptedThingBase* thing)
{
    Inner* p = thing->inner;
    if (p)
    {
        VFn fn = *reinterpret_cast<VFn*>(*reinterpret_cast<char**>(p) + 0x58);
        fn(p);
    }
}