// Leaf: `vector_deleting_destructor'

class CLandscapeBackgroundPatch { public:
    void* vftable;
    void inner_dtor();
    void* vector_deleting_destructor(unsigned int flags);
};

void* CLandscapeBackgroundPatch::vector_deleting_destructor(unsigned int flags)
{
    this->inner_dtor();
    if (flags & 1)
        operator delete(this);
    return this;
}