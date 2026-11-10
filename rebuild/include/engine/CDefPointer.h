#ifndef FABLE_ENGINE_CDEFPOINTER_H
#define FABLE_ENGINE_CDEFPOINTER_H
// Retail model (hand-written 2026-10-30, class-model migration wave 2): the counted def handle CDefPointer<T>.
// Layout: Ego_r.pdb CDefPointer<T> is 4 bytes, `CDefPointeeBase* Object` at +0 (ghidra_out/struct_layouts_egor.tsv; every
// instantiation). Ego_r inlines every member; the member set is FableWin.pdb's (debug, nothing inlined):
//   ??0..QAE@W4ENullDef@@@Z (332 instantiations), copy ??0..QAE@ABV0@@Z (52), the template converting ctor ??$?0 (e.g.
//   CDefPointer<const CDefClassBase>::CDefPointer<CDefClassBase>(const CDefPointer<CDefClassBase>&)), ??1 (249), ??4 (31),
//   the template ??$?4 operator=<U>, ?Release@..QAEXXZ (249), ?InternalGetObject@..QBEPAVCDefPointeeBase@@XZ (245),
//   ??$SetDynamicCast@$$CBVCDefClassBase@@@..QAEXABV?$CDefPointer@$$CBVCDefClassBase@@@@@Z (246), ??C operator-> (229), ??D
//   operator* (42), ?Get@..QBEPBV..XZ (73), ?IsNull@..QBE_NXZ (42), ?Reset@..QAEXPBV..@Z (18), ?GetRefCount@..QBEKXZ (1).
// Inline bodies (retail bytes): retail expands all of them. One template gives both release shapes the landed GetSubDef<T>
// rows show (0x008C79B0 and family): the manager's CDefPointer<CDefClassBase> temporary releases as `mov edi,[eax+4]; ...
// dec edi; ... mov [eax],edi; jne`, the CDefPointer<const T> assignment and destructor as `dec dword ptr [ecx+4]; jne`;
// Release() must hold the pointee in a local (the temporary's Delete call reuses ecx; reloading Object differs).
// Release() decrements the pointee's count in place (Codex pass 11 consolidation 2): 0x00667450
// CThingCreatureBase::ReactToGenericStrike releases its CDefPointer<const CSpecialEffectsDef> locals as `dec dword ptr
// [ecx+4]; jne; mov eax,[ecx]; call [eax+4]`; `object->Release() == 0` (CIVCountedPointeeBase::Release returns the new
// count) emits `mov edx,[ecx+4]; dec edx; mov eax,edx; test eax,eax; mov [ecx+4],edx` there (0x007355F0 matches either way);
// the GetSubDef<T> load/dec/store shape above still comes out of this form (every landed DEF_CLASS_BASE row rechecked).
// The assignment shape (`cmp; test; dec [ecx+4]; jne; call [eax+4]; and [esi],0; inc [eax+4]`) is the out-of-line
// operator= rows (e.g. 0x00592844). SetDynamicCast<U>: FableWin.pdb pairs it with GFDebugPDynamicCast<CDefPointeeBase*,
// const T*> (a debug-only type check); release code is the plain assignment.
// Forced inline: retail never calls a CDefPointer member out of line (no relocation binding names one; the four
// out-of-line operator= bodies, e.g. 0x00592844, are unreferenced COMDATs), size-optimised TUs included (landed GetSubDef<T>
// rows 0x007BE88C / 0x007BE92E: plain `inline` is called out of line there, 112 vs 162 bytes); __forceinline reproduces that.
// ENullDef constructor (added 2026-10-31, owner request): FableWin.pdb's ??0?$CDefPointer@..@@QAE@W4ENullDef@@@Z (332
// instantiations) is the null constructor; no ??0..@@QAE@XZ default constructor exists in either PDB although classes
// default-construct CDefPointer members, so the enum parameter carries a default argument (inferred from that absence).
// The enumerator's name is in neither PDB name list: NULL_DEF is a placeholder spelling (naming queue), the enum's type
// name ENullDef is the PDB's. Retail bytes: `and/mov dword ptr [..], 0` — the body is `Object = 0`. Unproven: Reset's body.
#ifndef FABLE_RETAIL_DEF_CLASS_BASE
#error engine/CDefPointer.h is the retail def model: define FABLE_RETAIL_DEF_CLASS_BASE before including it
#else
#include "rebuild_abi.h"
#include "engine/CDefPointeeBase.h"
#if defined(_MSC_VER)
#define FABLE_DEFPTR_INLINE __forceinline
#else
#define FABLE_DEFPTR_INLINE inline
#endif

enum ENullDef { NULL_DEF };                                          // W4ENullDef@@ (global); enumerator name unproven

template <class T> class CDefPointer {
public:
    FABLE_DEFPTR_INLINE CDefPointer(ENullDef = NULL_DEF) : Object(0) {}             // ??0..QAE@W4ENullDef@@@Z
    FABLE_DEFPTR_INLINE CDefPointer(const CDefPointer& other) : Object(other.Object) { if (Object) Object->AddRef(); }
    template <class U> FABLE_DEFPTR_INLINE CDefPointer(const CDefPointer<U>& other) : Object(other.InternalGetObject())
    {
        if (Object) Object->AddRef();
    }
    FABLE_DEFPTR_INLINE ~CDefPointer() { Release(); }
    // Copy assignment (Codex pass 11 consolidation 2): retail reads other.Object again after the Release call --
    // 0x007355F0 SetCombatType `PCombatTypeDef=pcombat_type` (a const reference that may alias) emits `cmp ecx,[ebp]`,
    // the release, `mov [esi+28h],0`, then `mov eax,[ebp]` / `mov [esi+28h],eax` / `inc [eax+4]`. Assign(other.Object)
    // caches the pointer in a register across the Release call instead. Where `other` cannot alias (a local) the two
    // compile identically (every landed DEF_CLASS_BASE row rechecked).
    FABLE_DEFPTR_INLINE CDefPointer& operator=(const CDefPointer& other)
    {
        if (Object != other.Object) {
            Release();
            Object = other.Object;
            if (Object) Object->AddRef();
        }
        return *this;
    }
    template <class U> FABLE_DEFPTR_INLINE CDefPointer& operator=(const CDefPointer<U>& other) { Assign(other.InternalGetObject()); return *this; }
    template <class U> FABLE_DEFPTR_INLINE void SetDynamicCast(const CDefPointer<U>& other) { Assign(other.InternalGetObject()); }
    CDefPointeeBase* InternalGetObject() const { return Object; }
    T* Get() const { return static_cast<T*>(Object); }
    T* operator->() const { return Get(); }
    T& operator*() const { return *Get(); }
    bool IsNull() const { return Object == 0; }
    unsigned long GetRefCount() const { return Object->GetRefCount(); }
#ifdef FABLE_RETAIL_CDEFPOINTER_RESET
    // Chunk 761 (row 0x00779CD0 ~CTCDExperienceOrb): FableWin.pdb ?Reset@?$CDefPointer@$$CBVCExperienceDef@@@@QAEXPBVCExperienceDef@@@Z
    // (one of the 18 Reset instantiations), inline in retail. The CIVCountedPointer::Reset body (chunk 737): Release() (its null test
    // holds the `mov [ptr],0`), then the new pointer stored unconditionally and AddRef'd when non-null; `Reset(0)` gives retail's
    // release block followed by a second `mov dword ptr [esi+0Ch],0` (0x00779CEA / 0x00779CF1). Needs T complete (a def class).
    FABLE_DEFPTR_INLINE void Reset(T* object)
    {
        Release();
        Object = const_cast<CDefPointeeBase*>(static_cast<const CDefPointeeBase*>(object));
        if (Object) Object->AddRef();
    }
#else
    void Reset(T*);                                                      // unproven body (FableWin ?Reset@..QAEXPBV..@Z)
#endif
#ifdef FABLE_RETAIL_CDEFPOINTER_RELEASE_COUNTREF
    // Nested opt-in (chunk 853, row 0x0043A570 ~CPlayerGui): the release of a CDefPointer held in a STATIC (CPlayerGui::PGuiDef
    // 0x013B878C) reads the count through a reference on Object (`mov ebx,[eax+4]; mov ecx,eax; add eax,4; dec ebx; ...; mov [eax],ebx`,
    // 0x0043A587; the landed atexit destructor of the same static 0x01228B70 has the identical shape). The in-place form above gives
    // `dec dword ptr [ecx+4]` there. Same semantics.
    FABLE_DEFPTR_INLINE void Release()
    {
        if (Object) {
            CDefPointeeBase* object = Object;
            const unsigned long& count = Object->IVRefCount;
            --Object->IVRefCount;
            if (count == 0)
                object->Delete();
            Object = 0;
        }
    }
#elif defined(FABLE_RETAIL_CDEFPOINTER_RELEASE_POINTEE)
    // Nested opt-in (agentSCR, rows 0x005C8459 / 0x005C9A95 CTCInventoryWeapons::ConstructScreen(PC); proven on 0x005C3180
    // GetActiveProjectileWeaponDamage too): the count is dropped through the pointee's own inline FableWin.pdb
    // ?Release@CIVCountedPointeeBase@@QBEJXZ on a fresh read of Object, the cached pointer only names the Delete target. Same bytes
    // as the in-place form for every release (`dec dword ptr [ecx+4] ; jne` / the temporary's `add eax,4 ; dec [eax] ; cmp [eax],0`),
    // but c2 then keeps a may-alias edge between a load through the def and the decrement: retail's
    // `mov eax,[esi+54h] ; dec dword ptr [esi+4] ; mov [ebp-8],eax` (0x005C3214, 0x005C8FAF: `damage = shot->HitThingDamage` before
    // the shot def's release); the in-place form schedules the decrement first. Same semantics.
    FABLE_DEFPTR_INLINE void Release()
    {
        if (Object) {
            CDefPointeeBase* object = Object;
            if (Object->Release() == 0)
                object->Delete();
            Object = 0;
        }
    }
#else
    FABLE_DEFPTR_INLINE void Release()
    {
        if (Object) {
            CDefPointeeBase* object = Object;
            if (--object->IVRefCount == 0)                              // Codex pass 11 consolidation 2: see Release note above
                object->Delete();
            Object = 0;
        }
    }
#endif
    CDefPointeeBase* Object;                                             // +0x00
private:
    FABLE_DEFPTR_INLINE void Assign(CDefPointeeBase* object)                                 // the inline body of operator= / SetDynamicCast
    {
        if (Object != object) {
            Release();
            Object = object;
            if (Object) Object->AddRef();
        }
    }
};
FABLE_STATIC_ASSERT(sizeof(CDefPointer<CDefPointeeBase>) == 0x4);

#endif // FABLE_RETAIL_DEF_CLASS_BASE
#endif // FABLE_ENGINE_CDEFPOINTER_H
