// p00: plain-inline releasing dtor at N exits of one function
struct Info { unsigned long RefCount; void (__fastcall* DeleteFunc)(void*); void* Data; };
void operator delete(void*);
struct Handle {
    void* Data; Info* I;
    Handle(const Handle& o) : Data(o.Data), I(o.I) { if (I) ++I->RefCount; }
    ~Handle() { if (I) { if (--I->RefCount == 0) { I->DeleteFunc(I->Data); operator delete(I); } } Data = 0; I = 0; }
};
bool Term(); void Yield(); bool Test(int);
Handle Get();
void f2() { Handle a = Get(); if (Test(1)) return; Yield(); if (Test(2)) return; Yield(); }
void f3() { Handle a = Get(); if (Test(1)) return; Yield(); if (Test(2)) return; Yield(); if (Test(3)) return; Yield(); }
void f4() { Handle a = Get(); if (Test(1)) return; Yield(); if (Test(2)) return; Yield(); if (Test(3)) return; Yield(); if (Test(4)) return; Yield(); }
