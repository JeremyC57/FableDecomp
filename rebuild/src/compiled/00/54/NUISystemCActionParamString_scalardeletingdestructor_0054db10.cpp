// NUISystem::CActionParamString::`scalar deleting destructor'(unsigned int flags) -- destroy the member, restore the base vtable, free when bit 0 is set
// Leaf: ??_GCActionParamString@NUISystem@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0054DB10, 45 bytes, thiscall, `ret 4`.
// Identity (proven): it stores vtable 0x01249520 (RTTI .?AVCActionParamString@NUISystem@@; this function is its slot 0) and then the base NUISystem::CActionParam's vtable 0x0122F83C (RTTI) after destroying the CWideString at +8 (Ego_r.pdb CActionParamString: CActionParam base, Value +8).
// Callees: the member destructor 0x0099B510, operator delete 0x00BFE9BC.
// Shape (family M128, representative 0x0054DB10): the compiler-generated virtual `??_G` with the class's empty inline
//   destructor (own vptr, member destructor) and the base's empty inline destructor (its vptr store, scheduled after the
//   flag test's load); MakeOne's `new` emits the vtable and with it this function (not part of the retail body).
void* __cdecl operator new(unsigned int size);
void __cdecl operator delete(void* p);                      // 0x00BFE9BC
class CWideString {
public:
    CWideString(const CWideString& other);
    ~CWideString();                                         // 0x0099B510
    void* PStringData;
};
namespace NUISystem {
enum EActionParamType { ACTION_PARAM_TYPE_NONE = 0 };
class CActionParam {                                        // vtable 0x0122F83C; Ego_r.pdb: vptr, ActionParamType
public:
    CActionParam() { }
    virtual ~CActionParam() { }
    EActionParamType ActionParamType;                       // +0x4
};
class CActionParamString : public CActionParam {            // vtable 0x01249520
public:
    CActionParamString(CWideString& value) : Value(value) { }
    virtual ~CActionParamString() { }
    CWideString Value;                                      // +0x8
};
}
NUISystem::CActionParamString* MakeOne(CWideString& value)   // emits the vtable and the scalar deleting destructor; not part of the retail body
{
    return new NUISystem::CActionParamString(value);
}