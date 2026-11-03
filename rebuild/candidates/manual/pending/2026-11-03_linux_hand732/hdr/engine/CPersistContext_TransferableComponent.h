#ifndef FABLE_ENGINE_CPERSISTCONTEXT_TRANSFERABLECOMPONENT_H
#define FABLE_ENGINE_CPERSISTCONTEXT_TRANSFERABLECOMPONENT_H
// CPersistContext_TransferableComponentGenericType / CPersistContext_TransferableComponent<T>: the {vtable, T* Object} wrapper the text
// arms of CPersistContext::Transfer<T>(const char*, T&, const T&) build and hand to the protected TransferObjectSaveText /
// TransferObjectLoadText (retail 0x009BAD60 / 0x009BBA40).
// Evidence: Ego_r.pdb layout CPersistContext_TransferableComponent<bool> sizeof 8 { vptr, bool* Object +4 }; FableWin.pdb
// ??0CPersistContext_TransferableComponentGenericType@@QAE@XZ (public ctor), ??0?$CPersistContext_TransferableComponent@_N@@QAE@AA_N@Z
// (public ctor taking T&), and the private virtuals ?TransferOut@?$CPersistContext_TransferableComponent@_N@@EAEXAAVCPersistContext@@
// AAVCCharString@@@Z (slot 0) / ?TransferIn@...@EAEXAAVCPersistContext@@AAVCStringParser@@@Z (slot 1) (both PDBs; retail vtable
// 0x0122D6C4 = .?AV?$CPersistContext_TransferableComponent@_N@@, slot 0 TransferOut 0x004039B0-twin, slot 1 TransferIn). Same model as
// the landed Transfer<bool> 0x004045C0 / Transfer<long> 0x00410BE0 / Transfer<float> 0x00410620 sources.
class CPersistContext;
class CCharString;
class CStringParser;
class CPersistContext_TransferableComponentGenericType {
public:
    CPersistContext_TransferableComponentGenericType() {}
    virtual void TransferOut(CPersistContext& context, CCharString& out) = 0;      // slot 0
    virtual void TransferIn(CPersistContext& context, CStringParser& parser) = 0;  // slot 1
};
template<class T> class CPersistContext_TransferableComponent : public CPersistContext_TransferableComponentGenericType {
public:
    T* Object;                                                                     // +0x04
    CPersistContext_TransferableComponent(T& object) : Object(&object) {}
private:
    virtual void TransferOut(CPersistContext& context, CCharString& out);
    virtual void TransferIn(CPersistContext& context, CStringParser& parser);
};
#endif
