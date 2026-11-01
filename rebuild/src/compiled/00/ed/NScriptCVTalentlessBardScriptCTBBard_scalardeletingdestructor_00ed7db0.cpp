// NScript::CV_TalentlessBardScript::CTB_Bard::`scalar deleting destructor'(unsigned int flags) -- ~CTB_Bard, then free the entity when bit 0 is set
// Leaf: ??_GCTB_Bard@CV_TalentlessBardScript@NScript@@UAEPAXI@Z
// Naming wave 28 (family bard, c696 queue): slot 0 of vtable 0x012F3494 (CTB_Bard; stored by the landed ctor 0x00ED7D50); the destructor
//   0x00ED7DD0 is ??1CTB_Bard@CV_TalentlessBardScript@NScript@@UAE@XZ (chunk 696); Ego_r.pdb ??_GCTB_Bard@CV_TalentlessBardScript@NScript@@UAEPAXI@Z.
//   Was a free __fastcall CLandscapeBackgroundPatch_vector_deleting_destructor (catalog CVtable_012f3494) calling a
//   ?CLandscapeBackgroundPatch_dtor_00ed7dd0@@ stand-in.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00ED7DB0, 30 bytes: push esi; mov esi,ecx; call 0x00ED7DD0; test byte [esp+8],1; je;
//   push esi; call operator delete 0x00BFE9BC; add esp,4; mov eax,esi; pop esi; ret 4.
// Shape: the deleting destructor is emitted with the vtable, i.e. in a TU that constructs the class: MakeOne is that construction.
class CEntityScriptBase {                               // vtable 0x012C3224
public:
    CEntityScriptBase();
    virtual ~CEntityScriptBase();                       // 0x00F35B40
};
namespace NScript {
class CV_TalentlessBardScript {
public:
    class CTB_Bard : public CEntityScriptBase {         // vtable 0x012F3494
    public:
        virtual ~CTB_Bard();                            // 0x00ED7DD0
    };
};
}

void* MakeOne()
{
    return new NScript::CV_TalentlessBardScript::CTB_Bard;
}
