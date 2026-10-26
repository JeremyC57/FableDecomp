class CInitBaseClass {
public:
    virtual ~CInitBaseClass();   // 0x0099A410
};
struct CInkeyBuffer {
    void* vftable;
};

void operator delete(void*);

void* __fastcall CInkeyBuffer_scalar_deleting_destructor(CInkeyBuffer* buffer, int edx_unused, unsigned int flags)
{
    ((CInitBaseClass*)(buffer))->CInitBaseClass::~CInitBaseClass();
    if (flags & 1)
        operator delete(buffer);
    return buffer;
}