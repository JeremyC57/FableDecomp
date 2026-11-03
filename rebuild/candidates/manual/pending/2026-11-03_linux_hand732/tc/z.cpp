struct T { int a,b,c; unsigned long F;
 bool Z1() const { return (F & 4) ? true : false; }
 bool Z2() const { if (F & 4) return true; return false; }
 bool Z3() const { return !!(F & 4); }
 bool Z4() const { return (bool)(F & 4); }
 bool Z5() const { return (F & 4) != 0 ? true : false; }
 bool Z6() const { return (long)F & 4 ? true : false; }
};
int f1(T* t, int k){ if (k && t->Z1()) return 1; return 2; }
int f2(T* t, int k){ if (k && t->Z2()) return 1; return 2; }
int f3(T* t, int k){ if (k && t->Z3()) return 1; return 2; }
int f4(T* t, int k){ if (k && t->Z4()) return 1; return 2; }
int f5(T* t, int k){ if (k && t->Z5()) return 1; return 2; }
int f6(T* t, int k){ if (k && t->Z6()) return 1; return 2; }
