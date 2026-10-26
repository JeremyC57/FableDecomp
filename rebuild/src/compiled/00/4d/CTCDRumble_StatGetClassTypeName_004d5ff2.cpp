class CCharString { public:
  char* p;
  CCharString* ctorB(const char* s, int n);
  CCharString* StatGetClassTypeName_8();
};
CCharString* CCharString::StatGetClassTypeName_8() {
  ctorB((const char*)0x123dd2c, -1);
  return this;
}