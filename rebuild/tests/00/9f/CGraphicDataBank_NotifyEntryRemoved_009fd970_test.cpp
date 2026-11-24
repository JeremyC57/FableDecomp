#include <stdio.h>
#include <string.h>

class CGraphicDataBankLayout { public:
    char pad[0x1e0];
    char* f1e0;
};

// NF335 (NF324 Q2): the row now defines the PDB member CGraphicDataBank::NotifyEntryRemoved
// (?NotifyEntryRemoved@CGraphicDataBank@@MAEXK@Z).
// The test calls it qualified through a pointer to this declaration-only model, cast from the test's object:
// no body and never constructed, so the test emits no vftable and no copy of the row's symbol.
class CGraphicDataBank { protected:
    virtual void NotifyEntryRemoved(unsigned long);
    friend class CGraphicDataBank_TestCaller;
};
class CGraphicDataBank_TestCaller { public:   // test-local: reaches the protected member for the test
    static void Call(CGraphicDataBank* self, unsigned long p0) { return self->CGraphicDataBank::NotifyEntryRemoved(p0); }
};

int main()
{
    CGraphicDataBankLayout obj;
    memset(&obj, 0, sizeof(obj));
    char arr[0x2c * 8];
    memset(arr, 0, sizeof(arr));
    obj.f1e0 = arr;

    unsigned long idx = 3;
    CGraphicDataBank_TestCaller::Call((CGraphicDataBank*)(&obj), idx);

    int v = *(int*)(arr + idx * 0x2c + 0x24);
    int other = *(int*)(arr + 2 * 0x2c + 0x24);
    if (v == -1 && other == 0)
        printf("009fd970_TEST PASS\n");
    else
        printf("FAIL v=%d other=%d\n", v, other);
    return 0;
}