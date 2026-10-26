#pragma optimize("t", on)
class CTCBase {
public:
    virtual ~CTCBase();   // 0x00686830
};
extern void __fastcall DestructFirstMember_00440c40(void* member);
extern void __fastcall DestructSecondMember_0075f2b0(void* member);
void* __fastcall CTCCreatureFlyOnSpline_CompositeScalarDeletingDestructor(void* spline, int, unsigned int flags)
{
    DestructFirstMember_00440c40(static_cast<unsigned char*>(spline) + 20);
    DestructSecondMember_0075f2b0(static_cast<unsigned char*>(spline) + 12);
    ((CTCBase*)(spline))->CTCBase::~CTCBase();
    if (flags & 1)
        operator delete(spline);
    return spline;
}