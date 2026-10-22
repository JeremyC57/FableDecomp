#pragma optimize("t", on)
extern void __fastcall DestructFirstMember(void* member);
extern void __fastcall DestructSecondMember(void* member);
extern void __fastcall CTCCreatureFlyOnSpline_Destruct(void* spline);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CTCCreatureFlyOnSpline_CompositeScalarDeletingDestructor(void* spline, int, unsigned int flags)
{
    DestructFirstMember(static_cast<unsigned char*>(spline) + 20);
    DestructSecondMember(static_cast<unsigned char*>(spline) + 12);
    CTCCreatureFlyOnSpline_Destruct(spline);
    if (flags & 1)
        OperatorDelete(spline);
    return spline;
}