// CPersistContext::TransferVector<ENavigatorType>(vector<ENavigatorType>& vec) -- unnamed binary-only vector transfer: CRC check, then the binary vector reader (Mode 2) or writer (any other mode)
// Leaf: ??$TransferVector@W4ENavigatorType@@@CPersistContext@@QAEXAAV?$vector@
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x006724B0, 37 bytes, `ret 4`; ecx = the context, one pushed vector address; no frame.
// Identity (T = ENavigatorType): sole caller CThingCreatureDef::Transfer (0x00671600) at 0x00671742 passes +0xC8 after the float
//   +0xBC (= Ego_r FOV +0xC0) and the ESex / ECreatureType transfers +0xC0 / +0xC4 = Ego_r NavigatorTypes +0xCC
//   (vector<ENavigatorType>); the caller then sorts it and removes duplicates (std::sort<ENavigatorType*> 0x006720B0,
//   std::adjacent_find 0x00671BB0, std::__unique_copy 0x00671BE0). FableWin.pdb names the named form
//   ??$TransferVector@W4ENavigatorType@@@CPersistContext@@QAEXPBDAAV?$vector@... and both helpers called here:
//   GFSerialiseVectorBinaryIn<ENavigatorType> 0x00672530 (Mode 2, the load stream) and GFSerialiseVectorBinaryOut<ENavigatorType>
//   0x006724F0 (the save stream; both landed under those names), __fastcall (stream ecx, vector edx). The one-argument parameter
//   list is the descriptive spelling of the unnamed retail-only transfer family (engine/CPersistContext.h `TransferVector(vector&)`,
//   the sibling of the landed `Transfer(T&)` rows 0x00431020 / 0x00431143): no PDB carries it (descriptive, NOTES).
// Shape: speed TU (no size pragma: `sub eax,2` where the size TUs emit `dec eax ; dec eax`); a one-case switch with a default.
// Body: CheckCRC("") (0x00404500, the pooled empty literal 0x0122D70E); Mode 2 reads, any other mode writes (`sub eax,2 ; je`).
// Replaces a pad-model `CEnginePrimitiveRenderer2D::AddPrimitive` stand-in (fake EngineAssertLog for CheckCRC, free helper names).
#include "fable_compiler.h"
#define FABLE_RETAIL_CPERSISTCONTEXT
#include "engine/CPersistContext.h"

enum ENavigatorType { };
template <class T> void FABLE_FASTCALL GFSerialiseVectorBinaryIn(CMemoryDataInputStream& is, std::vector<T, std::allocator<T> >& vec);
template <class T> void FABLE_FASTCALL GFSerialiseVectorBinaryOut(CDataOutputStream& os, std::vector<T, std::allocator<T> >& vec);

template <> void CPersistContext::TransferVector<ENavigatorType>(std::vector<ENavigatorType, std::allocator<ENavigatorType> >& vec)
{
    CheckCRC("");
    switch (Mode) {
    case MODE_LOAD_BINARY:
        GFSerialiseVectorBinaryIn(*PLoadStream, vec);
        break;
    default:
        GFSerialiseVectorBinaryOut(*PSaveStream, vec);
        break;
    }
}
