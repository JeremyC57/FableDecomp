class C3DVector { public: float x, y, z; };
// Leaf: IsNewDestinationGoingToResetNavigation
class CTCCreatureNavigation;
struct IHelper {
  virtual void v0();
  virtual void v1();
  virtual void v2();
  virtual void v3();
  virtual void v4();
  virtual void v5();
  virtual void v6();
  virtual bool v7(CTCCreatureNavigation* nav, C3DVector* dest);
};
class CTCCreatureNavigation { public:
  int f0;
  IHelper* ParentThing;
};
bool __fastcall IsNewDestinationGoingToResetNavigation(CTCCreatureNavigation* self, int /*edx*/, C3DVector* dest){
  return self->ParentThing->v7(self, dest);
}