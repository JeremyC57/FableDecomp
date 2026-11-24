#include <cstdio>

class CDefClassBase {
public:
    CDefClassBase& operator=(const CDefClassBase& other);
};

struct CVillageMemberBase {
  void Duplicate(const CDefClassBase* src);
};

class CVillageMemberDefLayout { public:
  char pad[0x25];
  unsigned char f25;
};

static int g_base_called = 0;
CDefClassBase& CDefClassBase::operator=(const CDefClassBase& other_)
{
    const CDefClassBase* src = (const CDefClassBase*)&other_; (void)src;
  g_base_called = 1;
    return *this;
}

// NF335 (NF324 Q2): the row now defines the PDB member CVillageMemberDef::Copy
// (?Copy@CVillageMemberDef@@UAEXPBVCDefClassBase@@@Z).
// The test calls it qualified through a pointer to this declaration-only model, cast from the test's object:
// no body and never constructed, so the test emits no vftable and no copy of the row's symbol.
class CVillageMemberDef { public:
    virtual void Copy(const CDefClassBase*);
};

int main() {
  CVillageMemberDefLayout dst; dst.f25 = 0;
  CVillageMemberDefLayout src; src.f25 = 0xAB;
  ((CVillageMemberDef*)(&dst))->CVillageMemberDef::Copy(reinterpret_cast<const CDefClassBase*>(&src));
  if (g_base_called == 1 && dst.f25 == 0xAB) {
    printf("004dffa5_TEST PASS\n");
  } else {
    printf("FAIL base=%d f25=%02x\n", g_base_called, dst.f25);
  }
  return 0;
}