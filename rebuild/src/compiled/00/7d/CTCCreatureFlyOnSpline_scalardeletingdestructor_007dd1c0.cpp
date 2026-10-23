#pragma optimize("t", on)
extern void __fastcall DestructFirstMember_00440c40(void* member);
extern void __fastcall DestructSecondMember_0075f2b0(void* member);
extern void __fastcall CTCCreatureFlyOnSpline_Destruct(void* spline);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CTCCreatureFlyOnSpline_CompositeScalarDeletingDestructor(void* spline, int, unsigned int flags)
{
    DestructFirstMember_00440c40(static_cast<unsigned char*>(spline) + 20);
    DestructSecondMember_0075f2b0(static_cast<unsigned char*>(spline) + 12);
    CTCCreatureFlyOnSpline_Destruct(spline);
    if (flags & 1)
        OperatorDelete(spline);
    return spline;
}