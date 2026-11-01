#pragma optimize("s",on)
// Leaf: `vector_deleting_destructor'
// vector deleting destructor whose deallocator is the engine free helper (0x00bfe9bc),
// cleaned with `pop ecx` (size peephole). __fastcall this=ecx, flags=stack (ret 4).
class CWideString { public:
    ~CWideString();   // 0x0099B510 (Ego_r.pdb ??1CWideString@@QAE@XZ)
};
struct T {  void* VecDel(unsigned flags); };
void operator delete(void*);
void* T::VecDel(unsigned flags) {
    ((CWideString*)this)->~CWideString();
    if (flags & 1) operator delete(this);
    return this;
}