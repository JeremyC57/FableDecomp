// Behaviour test for CEnginePrimitiveManagerStaticMeshesBase::SetupStaticBlocks (0x00BA50A0, parked DIFFER): a model of the row -- unchanged banks skip the work; a block needing a diffuse texture the stages cannot take fails.
#include <stdio.h>
#include <vector>
struct B { bool needsDiffuse; int tex; };
static bool Setup(std::vector<B>& bs, int bankFrame, int& recorded, bool stagesTakeDiffuse)
{ if (bankFrame == recorded) return true; bool failed = false;
  for (size_t i = 0; i < bs.size(); ++i) { if (bs[i].needsDiffuse && !stagesTakeDiffuse) failed = true; else bs[i].tex = 1; }
  recorded = bankFrame; return !failed; }
int main() { std::vector<B> b; B x = { true, 0 }, y = { false, 0 }; b.push_back(x); b.push_back(y); int rec = 1;
  bool a = Setup(b, 1, rec, false) && b[1].tex == 0; bool c = !Setup(b, 2, rec, false) && b[1].tex == 1 && rec == 2;
  bool ok = a && c && Setup(b, 3, rec, true) && b[0].tex == 1;
  printf(ok ? "00ba50a0_TEST PASS\n" : "00ba50a0_TEST FAIL\n");
  return ok ? 0 : 1;
}
