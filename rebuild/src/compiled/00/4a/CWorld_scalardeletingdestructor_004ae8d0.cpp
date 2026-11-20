// CWorld::`scalar deleting destructor' -- __thiscall this=ecx, flags=stack. base /O2 /Oy (no pragma). family len30 x22 (template 00412b00)
// Leaf: ?ScalarDeletingDestructor@CWorld@@QAEPAXI@Z
// agentNF187 (q5081, in-place respelling of this row's existing local model, the NF182 q4021 recipe): the destructor it calls is
//   ??1CWorld@@UAE@XZ 0x004A6AB0 (Ego_r.pdb / FableWin.pdb ~CWorld; slot 0 of the primary CWorld vtable is this ??_G; the landed
//   0x004A6AB0 row defines that leaf), called by qualified name so the call is direct. It was the stand-in ?Dtor@CWorld@@QAEXXZ.
//   Header debt: the self leaf stays the stand-in (the real ??_G is emitted only by a TU that compiles a CWorld constructor or
//   destructor). The Leaf line is new (the leaf is unchanged).
class CWorld { public: virtual ~CWorld(); void* ScalarDeletingDestructor(unsigned flags); };
void __cdecl operator delete(void* p);
void* CWorld::ScalarDeletingDestructor(unsigned flags) {
    this->CWorld::~CWorld();
    if (flags & 1) ::operator delete(this);
    return this;
}
