// Leaf: ?CLandscapeBackgroundPatch_vector_deleting_destructor@CLandscapeBackgroundPatch@@QAEPAXI@Z

class CLandscapeBackgroundPatch { public:
    void* vftable;
    void base_dtor();
    void* CLandscapeBackgroundPatch_vector_deleting_destructor(unsigned int flags);
};

void* CLandscapeBackgroundPatch::CLandscapeBackgroundPatch_vector_deleting_destructor(unsigned int flags)
{
    this->base_dtor();
    if (flags & 1)
        operator delete(this);
    return this;
}