#pragma optimize("t", on)
extern void __fastcall DestructFirstMember_004ad4a0(void* member);
extern void __fastcall DestructSecondMember_006fcf10(void* member);
extern void __fastcall CSymbolMap_Destruct(void* map);
void* __fastcall CSymbolMap_CompositeScalarDeletingDestructor(void* map, int, unsigned int flags)
{
    DestructFirstMember_004ad4a0(static_cast<unsigned char*>(map) + 20);
    DestructSecondMember_006fcf10(static_cast<unsigned char*>(map) + 4);
    CSymbolMap_Destruct(map);
    if (flags & 1)
        operator delete(map);
    return map;
}