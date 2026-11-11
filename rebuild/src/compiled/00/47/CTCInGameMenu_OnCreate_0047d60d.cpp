// CTCInGameMenu::OnCreate @ 0x0047d60d  (__fastcall, void, 59 bytes)
//
// Genuine VC7.1 (cl /c /O2 /Oy /W3) reconstruction -- byte-exact vs retail
// (the two call rel32 operands are relocation-masked). Frame pointers are
// forced on (retail keeps push ebp/leave) via #pragma optimize("y",off);
// size optimization ("s",on) reproduces the `and [ebp-4],0` zero-init idiom.
//
// The body is the classic inlined intrusive-counted-pointer pattern:
//   * a zero-initialised counted-pointer local `h` (holds the object ptr),
//   * a getter that fills it: this->field4->provider70->GetHeld(&h)
//     (ecx = this->field4->[+0x70]; &h pushed),
//   * a member call consuming the held object's +0xa8 field:
//     this->ApplyGuiField(o->fieldA8)  (ecx = this),
//   * the inlined counted-pointer release: --o->refs; if it hit zero, call the
//     object's virtual slot 1 (vtbl[+4]) -> `dec [esi+4]; jne; mov eax,[esi];
//     mov ecx,esi; call [eax+4]`.
// Caching h.obj into the local `o` and using it for BOTH the apply argument and
// the release is what makes cl keep the object live in esi (this stays in edi)
// across the apply call, matching the retail register allocation exactly.

#pragma optimize("y", off)
#pragma optimize("s", on)

// agentNF15: the held object is the owner's CCreatureDef sub-def: GetHeld = CDefClassBase::GetSubDef<CCreatureDef> 0x00485815
//   (Ego_r.pdb ??$GetSubDef@VCCreatureDef@@@CDefClassBase@@QBE_NPAV?$CDefPointer@$$CBVCCreatureDef@@@@@Z) on the thing's def +0x70.
class CCreatureDef { public:
    virtual void v0();          // vtbl+0x0
    virtual void v1();          // vtbl+0x4  -- destroy-on-zero-refcount
    int          refs;          // +0x4
    char         pad[0xa8 - 8]; // +0x8 .. +0xa7
    int          fieldA8;       // +0xa8
};

template <class T> class CDefPointer { public: CCreatureDef* obj; CDefPointer() : obj(0) {} };   // the zeroed out slot

class CDefClassBase { public:
    template <class T> bool GetSubDef(CDefPointer<const T>* out) const;   // 0x00485815 GetSubDef<CCreatureDef>
};

struct Sub4 {
    char         pad[0x70];
    CDefClassBase* provider70;      // +0x70 the thing's def
};

class CTCInGameMenu { public:
    void* v0field;                  // +0x0
    Sub4* ParentThing;                   // +0x4
    void ApplyGuiField(int v);      // __fastcall member (masked callee)
    void OnCreate();
};

void CTCInGameMenu::OnCreate()
{
    CDefPointer<const CCreatureDef> h;
    ParentThing->provider70->GetSubDef(&h);
    CCreatureDef* o = h.obj;
    ApplyGuiField(o->fieldA8);
    if (--o->refs == 0)
        o->v1();
}