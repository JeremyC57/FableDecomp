#include <cstdio>
#include <new>

struct CEngineLocalDetailGeneratorDef { char pad[0x3c]; };
struct CMember { char c; };

static int g_member = 0;
static int g_dtor = 0;

void __fastcall CMember_dtor(CMember* def) { (void)def; g_member++; }
void __fastcall CEngineLocalDetailGeneratorDef_dtor(CEngineLocalDetailGeneratorDef* def) { (void)def; g_dtor++; }

void* __fastcall CEngineLocalDetailGeneratorDef_scalar_deleting_destructor(CEngineLocalDetailGeneratorDef* def, int, unsigned int flags)
{
  CMember_dtor((CMember*)((char*)def + 0x3c));
  CEngineLocalDetailGeneratorDef_dtor(def);
  if (flags & 1) {
    ::operator delete(def);
  }
  return def;
}

int main() {
  CEngineLocalDetailGeneratorDef* obj = (CEngineLocalDetailGeneratorDef*)::operator new(sizeof(CEngineLocalDetailGeneratorDef));
  void* r = CEngineLocalDetailGeneratorDef_scalar_deleting_destructor(obj, 0, 0);
  if (r != obj || g_member != 1 || g_dtor != 1) { std::printf("FAIL nodelete m=%d d=%d\n", g_member, g_dtor); return 1; }
  r = CEngineLocalDetailGeneratorDef_scalar_deleting_destructor(obj, 0, 1);
  if (r != obj || g_member != 2 || g_dtor != 2) { std::printf("FAIL delete m=%d d=%d\n", g_member, g_dtor); return 1; }
  std::printf("CEngineSubPrimitiveText_00455cab_TEST PASS\n");
  return 0;
}