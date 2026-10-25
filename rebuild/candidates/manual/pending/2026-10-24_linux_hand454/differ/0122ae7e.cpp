// NGlobalScriptDefs::DestroyPGlobalScriptDef() -- the dynamic atexit destructor for `pGlobalScriptDef`: release the global script definition (drop its reference, destroy it on the last one) and clear the pointer
// Grade: DIFFER(37v34) -- retail `mov ecx,eax ; add eax,4 ; dec dword ptr [eax] ; cmp dword ptr [eax],0` (RMW then re-read through a formed pointer, object kept in ecx); ours `dec dword ptr [eax+4]` then re-loads the static for `cmp [ecx+4],0` (volatile candidate)
// Leaf: _$E2
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0122AE7E, 34 bytes, in `.text$yd` (size-optimised quest-script module:
//   no 16-byte alignment, `and dword ptr [g],0`); its dynamic initializer @ 0x01224C99 only registers it with atexit (the
//   pointer is zero in .bss). VC7.1 names the pair `$E1` / `$E2`.
// Identity: the static at 0x0143E90C is the quest scripts' global script def pointer (landed quest scripts call it
//   g_pScriptDef: CQ_HangingTreeEvilScript::DoMission 0x00D6BFA0 reads +0xF58 through it). FableWin.pdb lists
//   "`dynamic initializer for 'pGlobalScriptDef''" and "`dynamic atexit destructor for 'pGlobalScriptDef''" (next to
//   CScriptThing::NullString's, as retail's thunk sits by 0x0122AE74). The catalog label "`anonymous namespace'::
//   `dynamic atexit destructor for 'DEFAULT_CAMERA_MODE_DEF''" is propagated. The namespace here is descriptive (the
//   PDB gives the bare global name).
// Callee: the def's virtual slot 1 (+4, its self-destroy) when the reference count at +4 reaches zero.
// Shape: CDefPointer's inline destructor under `#pragma optimize("s", on)` (which gives retail's `and [g],0`); the
//   destructor must be `__forceinline` (/Os leaves it out of line). Tried: the member form (this file) and a named
//   `object` local, and (grade 4) a named `object` plus a `long& count` reference with `--count; if (count == 0)` as two
//   statements (28 bytes both: VC7.1 folds them into `dec [ecx+4] ; jne`). Retail's `dec dword ptr [eax] ; cmp dword ptr [eax],0` re-reads the
//   count after the RMW: the volatile signature (the same release appears inline all through the quest-script module,
//   e.g. 0x00CBEEB0); volatile cannot land, so the row is parked.
#pragma optimize("s", on)
struct CDefPointeeBase {                               // counted def object: vptr, reference count at +4
    virtual void v00();
    virtual void Destroy();                            // slot 1 (+4)
    long m_refCount;                                   // +0x04
};

struct CScriptDef : CDefPointeeBase { };               // the global script definition

template <class T> struct CDefPointer {
    T* Object;
    __forceinline ~CDefPointer()
    {
        if (Object) {
            if (--Object->m_refCount == 0)
                Object->Destroy();
            Object = 0;
        }
    }
};

namespace NGlobalScriptDefs {
CDefPointer<CScriptDef> pGlobalScriptDef;              // 0x0143E90C
}
