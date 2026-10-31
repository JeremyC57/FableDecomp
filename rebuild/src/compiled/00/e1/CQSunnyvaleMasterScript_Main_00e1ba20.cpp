// CQ_SunnyvaleMasterScript::Main @ 00e1ba20
// Leaf: Main
struct IStep {
    struct VT { void (__fastcall *slot1c)(IStep*); };
    VT* vtbl;
};
struct CQ_SunnyvaleMasterScript {
    void* field0;
    IStep* field4;
};

// helper 0xf35b30 : __fastcall bool(CQ_SunnyvaleMasterScript*)
class CEntityScriptBase { public: bool IsActiveThreadTerminating() const; };   // 0x00F35B30 (Ego_r.pdb CEntityScriptBase, const)

void __fastcall Main(CQ_SunnyvaleMasterScript* self)
{
    do {
        IStep* s = self->field4;
        // vtbl slot at +0x1c => index 7
        (*(void (__fastcall **)(IStep*))((char*)s->vtbl + 0x1c))(s);
    } while (!((const CEntityScriptBase*)self)->IsActiveThreadTerminating());
}