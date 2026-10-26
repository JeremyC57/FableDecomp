class CCharString { public:
  void* p;
  void Ctor(const char* s, int n);
};
CCharString* __fastcall StatGetClassTypeName(CCharString* ret) {
  ret->Ctor("DoCreatureAction", -1);
  return ret;
}