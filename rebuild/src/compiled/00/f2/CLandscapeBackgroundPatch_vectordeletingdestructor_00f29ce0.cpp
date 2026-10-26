class CParentDefClassBase {
public:
    virtual ~CParentDefClassBase();   // 0x00430300
};
class CLandscapeBackgroundPatch { public:
// Leaf: `vector_deleting_destructor'
    void* vftable;
};

void operator delete(void*);

void* __fastcall CLandscapeBackgroundPatch_vector_deleting_destructor(CLandscapeBackgroundPatch* self, int edx_unused, unsigned int flags)
{
    ((CParentDefClassBase*)(self))->CParentDefClassBase::~CParentDefClassBase();
    if (flags & 1)
        operator delete(self);
    return self;
}