// CPlayerGui::DestroyPGuiDef() -- the dynamic atexit destructor for the static `CPlayerGui::PGuiDef`: release the cached "PLAYER_GUI_PC" definition (drop the def's reference, destroy it on the last one) and clear the pointer
// Leaf: _$E2
// Grade: RELOCATION_MATCH -- the same ~CDefPointer shape as 0x0122AE7E (object pointer copied first for Destroy, a
//   `const long&` view of the count tested after the decrement through Object); under /O2 VC7.1 gives retail's register
//   decrement through the formed pointer (`mov edx,[eax+4] ; mov ecx,eax ; add eax,4 ; dec edx ; test edx,edx ; mov
//   [eax],edx`). Two TUs agree on this destructor body: Review at landing, the shared engine/CDefPointer.h should take
//   it
// Retail: Fable.exe (TLC, VC7.1) function @ 0x01228B70, 40 bytes, in `.text$yd`; its dynamic initializer @ 0x0121A360 only
//   registers it (`push 0x01228B70 ; call atexit`): the pointer is zero in .bss. VC7.1 names the pair `$E1` / `$E2`.
// Identity: the static at 0x013B878C is the GUI def pointer every CPlayerGui method loads on first use (landed PeekGuiDef
//   0x00438570, SetAbilityButton 0x00438A20: `GetDef("PLAYER_GUI_PC", &PGuiDef)`). FableWin.pdb and Ego_r.pdb both list
//   "`dynamic atexit destructor for 'CPlayerGui::PGuiDef''" right after the CPlayerGui singleton's PClass thunk, as
//   retail does (0x01228B30). The catalog label "`anonymous namespace'::`dynamic atexit destructor for
//   'DEFAULT_CAMERA_MODE_DEF''" is propagated from a byte-identical body.
// Callee: the def's virtual slot 1 (+4, its self-destroy) when the reference count at +4 reaches zero.
// Shape: CDefPointer's inline destructor `if (Object) { if (--Object->m_refCount == 0) Object->Destroy(); Object = 0; }`.
//   Tried: the member form (this file: `dec [eax+4]`, the static re-loaded after the store), a named `object` local
//   (31 bytes: `dec [ecx+4] ; jne`), the count in a non-polymorphic base at +4 with an inline RemoveReference(), and a
//   `vo-latile long` count probe (load/dec/store, but the static still re-loaded). Retail's unfolded register decrement
//   with an explicit `test` is the vo-latile signature; vo-latile cannot land, so the row is parked.
class CDefPointeeBase { public:                               // counted def object: vptr, reference count at +4
    virtual void v00();
    virtual void Destroy();                            // slot 1 (+4)
    long m_refCount;                                   // +0x04
};

class CPlayerGuiDef : public CDefPointeeBase { public: };            // the "PLAYER_GUI_PC" definition

template <class T> class CDefPointer { public:
    T* Object;
    ~CDefPointer()
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

class CPlayerGui {
public:
    static CDefPointer<CPlayerGuiDef> PGuiDef;         // 0x013B878C
};

CDefPointer<CPlayerGuiDef> CPlayerGui::PGuiDef;