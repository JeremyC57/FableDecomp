class C3DVector { public: float x,y,z; };
// Leaf: IsNewDestinationGoingToResetNavigation
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