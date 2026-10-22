struct CDefClassBase;
struct CTCNoiseDef;
extern void __fastcall CTCNoiseDef_Copy_impl(CTCNoiseDef* def, void* edx, const CDefClassBase* other);
void __fastcall CTCNoiseDef_Copy(CTCNoiseDef* def, void* edx, const CDefClassBase* other)
{
    CTCNoiseDef_Copy_impl(def, edx, other);
}