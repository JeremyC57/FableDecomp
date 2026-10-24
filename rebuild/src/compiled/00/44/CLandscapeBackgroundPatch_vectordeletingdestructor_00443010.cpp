extern void __cdecl eng_operator_delete(void* p);
// Leaf: `vector_deleting_destructor'

struct CLandscapeBackgroundPatch {
    void* vftable;
    void inner_dtor();
    void* vector_deleting_destructor(unsigned int flags);
};

void* CLandscapeBackgroundPatch::vector_deleting_destructor(unsigned int flags)
{
    this->inner_dtor();
    if (flags & 1)
        eng_operator_delete(this);
    return this;
}