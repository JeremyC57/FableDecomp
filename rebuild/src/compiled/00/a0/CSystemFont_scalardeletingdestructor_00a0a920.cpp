class CInitBaseClass {
public:
    virtual ~CInitBaseClass();   // 0x0099A410
};
struct CSystemFont {
    void* vftable;
};

void operator delete(void*);

void* __fastcall CSystemFont_scalar_deleting_destructor(CSystemFont* font, int edx_unused, unsigned int flags)
{
    ((CInitBaseClass*)(font))->CInitBaseClass::~CInitBaseClass();
    if (flags & 1)
        operator delete(font);
    return font;
}