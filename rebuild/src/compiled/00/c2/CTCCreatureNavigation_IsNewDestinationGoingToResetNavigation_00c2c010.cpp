class C3DVector { public: float x, y, z; };
struct Sub {
  virtual void v0();
  virtual void v1();
  virtual void v2();
  virtual void v3();
  virtual void v4();
  virtual void v5();
  virtual bool slot6(void* a, C3DVector* b);
};
class CTCCreatureNavigation { public:
  void* f0;
  Sub* ParentThing;
  bool IsNewDestinationGoingToResetNavigation(C3DVector* dest);
};
bool CTCCreatureNavigation::IsNewDestinationGoingToResetNavigation(C3DVector* dest) {
  return this->ParentThing->slot6(this, dest);
}