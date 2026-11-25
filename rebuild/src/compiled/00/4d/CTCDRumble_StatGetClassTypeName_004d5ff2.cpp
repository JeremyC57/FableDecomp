class CCharString { public:
  char* p;
  CCharString* ctorB(const char* s, int n);
  CCharString* StatGetClassTypeName_8();
};
// Literal (NF413, 2026-11-25): HEAD cast the retail address (const char*)0x0123DD2C; it is now the literal spelled from retail's bytes
//   at 0x0123DD2C (narrow "CTCDRumble", NUL-terminated).
CCharString* CCharString::StatGetClassTypeName_8() {
  ctorB("CTCDRumble", -1);
  return this;
}