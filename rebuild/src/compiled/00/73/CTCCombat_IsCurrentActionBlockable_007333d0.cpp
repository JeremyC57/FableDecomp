// Leaf: ?IsCurrentActionBlockable@CTCCombat@@ABE_NXZ
struct CInner { char pad[0x100]; void* p100; };
struct CVtbl { char pad48[0x48]; unsigned char (__fastcall *m)(void*); };
class CTCCombat { public: int f0,f4,f8,fC; char pad[0x24-0x10]; CInner* f24;
    private: bool IsCurrentActionBlockable() const;
};

static bool checkframe(int v)
{
  return v == 0 || v == (int)0xfffffcf7;
}

bool CTCCombat::IsCurrentActionBlockable() const
{
    CTCCombat* combat = const_cast<CTCCombat*>(this);
  void* obj = combat->f24->p100;
  if (obj != 0)
  {
    CVtbl** vt = (CVtbl**)obj;
    if (!(**vt).m(obj))
      return checkframe(combat->fC);
  }
  return false;
}