class CBankFileAsync {
public:
    virtual ~CBankFileAsync();   // 0x009D6490
};
class CWADBank { public:
    void* vftable;
};

void operator delete(void*);

void* __fastcall CWADBank_scalar_deleting_destructor(CWADBank* bank, int edx_unused, unsigned int flags)
{
    ((CBankFileAsync*)(bank))->CBankFileAsync::~CBankFileAsync();
    if (flags & 1)
        operator delete(bank);
    return bank;
}