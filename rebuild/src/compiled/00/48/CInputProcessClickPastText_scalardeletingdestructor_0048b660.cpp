// CInputProcessClickPastText::`vector_deleting_destructor'  0x0048b660
// Standard MSVC vector deleting destructor for a class with a scalar
// (non-virtual) destructor at 0xa0d3d0 and operator delete at 0xbfe9bc.

class CInputProcessClickPastText
{ public:
    // scalar destructor - emitted as a direct call in the vector-deleting stub
    void Destroy();  // maps to 0xa0d3d0
};

// operator delete used by the class (0xbfe9bc)
void ClassOperatorDelete(void* p);

void* __fastcall CInputProcessClickPastText_scalar_deleting_destructor(
    CInputProcessClickPastText* process, int /*edx*/, unsigned int flags)
{
    process->Destroy();
    if (flags & 1)
        ClassOperatorDelete(process);
    return process;
}