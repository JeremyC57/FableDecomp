#include <stdio.h>
// Behaviour test for 0x009F69B0 CInputEventPackage copy constructor: links against the source object; the vector copy
// constructor is defined here and records its source.
namespace std {
template <class _Tp> class allocator { };
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
    vector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    vector(const vector& __x);
};
}
class CInputEvent { public: unsigned char _body[0x34]; };
static const void* g_copiedFrom = 0;
template <> std::vector<CInputEvent, std::allocator<CInputEvent> >::vector(const vector& __x)
    : _M_start(__x._M_start), _M_finish(__x._M_finish), _M_end_of_storage(__x._M_end_of_storage) { g_copiedFrom = &__x; }
class CChecksum { public: unsigned long Word0, Word1, Word2, Word3; };
class C2DVector {
public:
    float X;
    float Y;
    C2DVector() { }
    C2DVector(const C2DVector& v) : X(v.X), Y(v.Y) { }
};
class CInputEventPackage {
public:
    std::vector<CInputEvent> Events;
    long                     TimeStamp;
    CChecksum                Checksum;
    C2DVector                MousePos;
    C2DVector                JoystickPos;
    CInputEventPackage() { }
    CInputEventPackage(const CInputEventPackage& other);
};
int main()
{
    if (sizeof(CInputEventPackage) != 0x30) { printf("009f69b0_TEST FAIL size\n"); return 1; }
    static CInputEvent events[2];
    CInputEventPackage a;
    a.Events._M_start = events; a.Events._M_finish = events + 2; a.Events._M_end_of_storage = events + 2;
    a.TimeStamp = 1234;
    a.Checksum.Word0 = 1; a.Checksum.Word1 = 2; a.Checksum.Word2 = 3; a.Checksum.Word3 = 4;
    a.MousePos.X = 0.25f; a.MousePos.Y = -0.5f; a.JoystickPos.X = 1.0f; a.JoystickPos.Y = 2.0f;
    CInputEventPackage b(a);
    if (g_copiedFrom != &a.Events || b.Events._M_finish != events + 2) { printf("009f69b0_TEST FAIL events\n"); return 1; }
    if (b.TimeStamp != 1234 || b.Checksum.Word0 != 1 || b.Checksum.Word3 != 4) { printf("009f69b0_TEST FAIL scalars\n"); return 1; }
    if (b.MousePos.X != 0.25f || b.MousePos.Y != -0.5f || b.JoystickPos.X != 1.0f || b.JoystickPos.Y != 2.0f) { printf("009f69b0_TEST FAIL vectors\n"); return 1; }
    printf("009f69b0_TEST PASS\n");
    return 0;
}
