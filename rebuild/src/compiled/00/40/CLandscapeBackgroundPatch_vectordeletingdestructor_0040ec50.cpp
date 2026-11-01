// __fastcall this=ecx, flags=stack. base /O2 /Oy (no pragma). family len30 x22 (template 00412b00)
// Leaf: `vector_deleting_destructor'
class CWideString { public:
    ~CWideString();   // 0x0099B510 (Ego_r.pdb ??1CWideString@@QAE@XZ)
};
struct T {  void* VecDel(unsigned flags); };
void __cdecl operator delete(void* p);
void* T::VecDel(unsigned flags) {
    ((CWideString*)this)->~CWideString();
    if (flags & 1) ::operator delete(this);
    return this;
}