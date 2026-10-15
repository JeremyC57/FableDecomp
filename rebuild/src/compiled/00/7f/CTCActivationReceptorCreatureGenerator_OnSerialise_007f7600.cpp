// CTCActivationReceptorCreatureGenerator::OnSerialise(CPersistContext& context, bool loading) -- the base receptor's OnSerialise, then the two bools ActivateOnActivate and TriggerOnActivate, each with a false default
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007F7600, length 76 bytes.
// Leaf: ?OnSerialise@CTCActivationReceptorCreatureGenerator
// Callees: CTCActivationReceptorBase::OnSerialise @ 0x007F77C0 (the manifest's "CAT_BanditAlly::OnPersist" is propagated),
// CPersistContext::Transfer<bool>(const char*, bool&, const bool&) @ 0x004045C0 (ICF-shared with the signed char
// instantiation the manifest names). Literals 0x01271F60 = "ActivateOnActivate", 0x01269284 = "TriggerOnActivate".
// Ego_r.pdb `?OnSerialise@CTCActivationReceptorCreatureGenerator@@MAEXAAVCPersistContext@@_N@Z` (the chunk's "vfunc_1").
// Shape: the `false` default is a literal bound to `const bool&`: VC7.1 materialises it right before each call in the
// dead `loading` argument slot (`lea ecx,[esp+10h] ... mov byte ptr [esp+1Ch],0`).
struct CPersistContext {
    template<class T> void Transfer(const char* name, T& value, const T& defaultValue);   // 0x004045C0
};

struct CTCActivationReceptorBase {             // retail 0x2c bytes (see 0x007F7710)
    virtual void v00();
    unsigned char _body[0x28];
    void OnSerialise(CPersistContext& context, bool loading);   // 0x007F77C0
};

struct CTCActivationReceptorCreatureGenerator : CTCActivationReceptorBase {
    bool TriggerOnActivate;                    // +0x2c
    bool ActivateOnActivate;                   // +0x2d
    void OnSerialise(CPersistContext& context, bool loading);
};

void CTCActivationReceptorCreatureGenerator::OnSerialise(CPersistContext& context, bool loading)
{
    CTCActivationReceptorBase::OnSerialise(context, loading);
    context.Transfer("ActivateOnActivate", ActivateOnActivate, false);
    context.Transfer("TriggerOnActivate", TriggerOnActivate, false);
}