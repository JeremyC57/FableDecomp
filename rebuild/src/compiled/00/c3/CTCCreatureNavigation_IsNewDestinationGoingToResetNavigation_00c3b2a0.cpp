#include "engine/C3DVector.h"
// Leaf: IsNewDestinationGoingToResetNavigation
// Plan phase 4 (2026-11-05), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
class CTCCreatureNavigation;
struct Helper {
  virtual void slot0();
  virtual void slot1();
  virtual void slot2();
  virtual void slot3();
  virtual void slot4();
  virtual void slot5();
  virtual void slot6();
  virtual bool slot7(CTCCreatureNavigation* nav, C3DVector* dest);
};
class CTCCreatureNavigation { public:
  void* f0;
  Helper* ParentThing;
};
bool __fastcall IsNewDestinationGoingToResetNavigation(CTCCreatureNavigation* self, int /*edx*/, C3DVector* dest) {
  return self->ParentThing->slot7(self, dest);
}