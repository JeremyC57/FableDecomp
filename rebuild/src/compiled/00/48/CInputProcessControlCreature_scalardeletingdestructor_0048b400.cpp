// CInputProcessControlCreature::`vector deleting destructor' @ 0048b400
// Standard MSVC vector/scalar deleting destructor idiom.
// call 0xa0d3d0 = scalar destructor (~CInputProcessControlCreature)
// call 0xbfe9bc = operator delete


class CInputProcessControlCreature
{ public:
    void dtor();                              // maps to 0xa0d3d0 (external)
    void * scalar_deleting_destructor(unsigned int flags);
};

void * CInputProcessControlCreature::scalar_deleting_destructor(unsigned int flags)
{
    this->dtor();
    if (flags & 1)
        operator delete(this);
    return this;
}