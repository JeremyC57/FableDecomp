# NF230 lessons (one line each)
- Ego_d.pdb's class field lists (LF_ONEMETHOD) carry inline accessors the Ego_r / FableWin symbol lists lack (GetPSoundBank, GetDef, Get3DEngine): read the type record before writing "no accessor".
- An explicit instantiation of std::_Destroy<pair<...> > makes VC7.1 emit the pair's ??_G COMDAT; that is enough to grade a retail scalar deleting destructor as a row.
- A derived class's override can be public over a protected base virtual (SetQuit): check the retail RTTI vtable slot before calling a slot-4 call "the base's".
- A never-constructed test subclass can hand out pointers-to-member for protected members (`&Derived::Member` has type `T Base::*`): CManager members test on raw storage without stubbing its ~60 virtuals.
