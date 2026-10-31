// NGlobalScriptDefs::DestroyPGlobalScriptDef() -- the dynamic atexit destructor for `pGlobalScriptDef`: release the global script definition (drop its reference, destroy it on the last one) and clear the pointer
// Leaf: _$E2
// Grade: RELOCATION_MATCH -- ~CDefPointer copies the object pointer first (`T* object = Object`, kept in ecx for the
//   Destroy call), takes a `const long&` view of the count, decrements through Object and tests the view: VC7.1 then
//   forms the count address (`add eax,4`) and re-reads it after the RMW (`dec [eax] ; cmp [eax],0`), retail's "vo-latile"
//   signature without vo-latile. Review at landing: the shared engine/CDefPointer.h destructor should take this shape
//   (its compile exceeded the lane's 200 s budget here, so the header model is local)
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
//   count after the RMW: the vo-latile signature (the same release appears inline all through the quest-script module,
//   e.g. 0x00CBEEB0); vo-latile cannot land, so the row is parked.
#pragma optimize("s", on)
class CDefPointeeBase { public:                               // counted def object: vptr, reference count at +4
    virtual void v00();
    virtual void Destroy();                            // slot 1 (+4)
    long m_refCount;                                   // +0x04
};

class CScriptDef : public CDefPointeeBase { public: };               // the global script definition

template <class T> class CDefPointer { public:
    T* Object;
    __forceinline ~CDefPointer()
    {
        if (Object) {
            T* object = Object;
            const long& count = Object->m_refCount;
            --Object->m_refCount;
            if (count == 0)
                object->Destroy();
            Object = 0;
        }
    }
};

namespace NGlobalScriptDefs {
CDefPointer<CScriptDef> pGlobalScriptDef;              // 0x0143E90C
}