#include <cstdio>

class CDefClassBase {
public:
    CDefClassBase& operator=(const CDefClassBase& other);
};

struct CVillageMemberBase {
  void Duplicate(const CDefClassBase* src);
};

class CVillageMemberDef { public:
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

void __fastcall CVillageMemberDef_Copy(CVillageMemberDef* self, void* edx, const CDefClassBase* src);

int main() {
  CVillageMemberDef dst; dst.f25 = 0;
  CVillageMemberDef src; src.f25 = 0xAB;
  CVillageMemberDef_Copy(&dst, 0, reinterpret_cast<const CDefClassBase*>(&src));
  if (g_base_called == 1 && dst.f25 == 0xAB) {
    printf("004dffa5_TEST PASS\n");
  } else {
    printf("FAIL base=%d f25=%02x\n", g_base_called, dst.f25);
  }
  return 0;
}