// NScript::CIsThingConscious::`scalar deleting destructor'(unsigned int flags) -- destroy the member, restore the base vtable, free when bit 0 is set
// Leaf: ??_GCIsThingConscious@NScript@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CDDB10, 45 bytes, thiscall, `ret 4`.
// Identity (proven): slot 2 of vtable 0x012C2FE8 (no RTTI locator): slot 0 0x00CDDAB0 is `Thing.IsAlive() && !Thing.IsUnconscious()` (CScriptThing slots +0x12C / +0xF4) = Ego_r.pdb ??RCIsThingConscious@NScript@@UBE_NXZ, slot 1 the 0x10-byte Clone; Ego_r.pdb NScript::CIsThingConscious has exactly operator(), Clone and the destructor; the CScriptThing member at +4.
// Callees: the member destructor 0x004AA840, operator delete 0x00BFE9BC.
// Shape (family M128, representative 0x0054DB10): the compiler-generated virtual `??_G` with the class's empty inline
//   destructor (own vptr, member destructor) and the base's empty inline destructor (its vptr store, scheduled after the
//   flag test's load); MakeOne's `new` emits the vtable and with it this function (not part of the retail body).
void* __cdecl operator new(unsigned int size);
void __cdecl operator delete(void* p);                      // 0x00BFE9BC
class CScriptThing {                                        // 12 bytes; virtual destructor 0x004AA840
public:
    CScriptThing(const CScriptThing& other);
    virtual ~CScriptThing();                                // 0x004AA840
    unsigned long Data[2];
};
namespace NScript {
class CPredicateBase {                                      // vtable 0x012C2FF4 (slots 0 / 1 pure)
public:
    CPredicateBase() { }
    virtual bool operator()() const = 0;
    virtual CPredicateBase* Clone() const = 0;
    virtual ~CPredicateBase() { }
};
class CIsThingConscious : public CPredicateBase {                       // vtable 0x012C2FE8
public:
    CIsThingConscious(const CScriptThing& thing) : Thing(thing) { }
    virtual bool operator()() const;                        // 0x00CDDAB0
    virtual CPredicateBase* Clone() const;                  // 0x00CDDAE0
    virtual ~CIsThingConscious() { }
    CScriptThing Thing;                                     // +0x4
};
}
NScript::CIsThingConscious* MakeOne(const CScriptThing& thing)   // emits the vtable and the scalar deleting destructor; not part of the retail body
{
    return new NScript::CIsThingConscious(thing);
}