// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12). Member accesses x/y/z respelled to the PDB X/Y/Z.
#include "engine/C3DVector.h"
class CTCCreatureNavigation;
struct IDelegate {
  virtual bool m0(CTCCreatureNavigation*, C3DVector*);
  virtual bool m1(CTCCreatureNavigation*, C3DVector*);
  virtual bool m2(CTCCreatureNavigation*, C3DVector*);
  virtual bool m3(CTCCreatureNavigation*, C3DVector*);
  virtual bool m4(CTCCreatureNavigation*, C3DVector*);
  virtual bool m5(CTCCreatureNavigation*, C3DVector*);
  virtual bool m6(CTCCreatureNavigation*, C3DVector*);
};
class CTCCreatureNavigation { public:
  void* f0;
  IDelegate* ParentThing;
  bool IsNewDestinationGoingToResetNavigation(C3DVector* dest);
};
bool CTCCreatureNavigation::IsNewDestinationGoingToResetNavigation(C3DVector* dest) {
  return this->ParentThing->m6(this, dest);
}