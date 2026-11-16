// CPersistContext::TransferVector<C2DVector>(std::vector<C2DVector>& value) -- unnamed binary vector transfer: CheckCRC(""), then the global vector reader (Mode 2, load stream +0x24) or writer (any other mode, save stream +0x28)
// Leaf: ??$TransferVector@VC2DVector@@@CPersistContext@@QAEXAAV?$vector@VC2DVector@@V?$allocator@VC2DVector@@@std@@@std@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00475D99, oracle length 46 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; push offset "" ; mov esi,ecx ; call CPersistContext::CheckCRC ; mov eax,[esi+18h] ; mov edx,[value] ; dec eax ; dec eax ; je L ; mov ecx,[esi+28h] ; call GFSerialiseVectorBinaryOut_C2DVector ; jmp R ; L: mov ecx,[esi+24h] ; call GFSerialiseVectorBinaryIn_C2DVector ; R: pop esi ; ret 4
// Persist transfer of one value: after the CRC check with the (empty) key the mode at +0x18
// selects the direction -- 2 (load) hands the load stream (+0x24) and the value to the
// `__fastcall` reader @ 0x00475DFA, anything else hands the save stream (+0x28) and the value to
// the `__fastcall` writer @ 0x00475DC7. The `dec/dec` sequence is the size-optimised lowering of
// the two-way switch on the mode (the lander's "s" pragma). Labels are propagated; the streams are modelled as opaque
// pointers. One of 24 bodies of this shape.
// Name (round 4, NF40): the member template instance ??$TransferVector@VC2DVector@@@CPersistContext@@QAEXAAV?$vector@VC2DVector@@V?$allocator@VC2DVector@@@std@@@std@@@Z, the unnamed overload engine/CPersistContext.h
//   declares (`template<class T> void TransferVector(std::vector<T>&)`; TLC's release persist macro drops the name argument, as
//   for TransferVectorOfSubComponents; the PDB name sets hold the named ??$TransferVector@VC2DVector@@@..QAEXPBDAAV?$vector@..@Z twin).
//   Same body and spelling as the landed 0x006724B0 TransferVector<ENavigatorType>. Was a local CPersistContext model with a
//   stand-in TransferBinary_00475d99 / TransferVectorBinary member; now on the shared header.
#pragma optimize("s", on)
#include "fable_compiler.h"
#define FABLE_RETAIL_CPERSISTCONTEXT
#include "engine/CPersistContext.h"
class C2DVector;
template <class T> void FABLE_FASTCALL GFSerialiseVectorBinaryIn(CMemoryDataInputStream& is, std::vector<T, std::allocator<T> >& vec);
template <class T> void FABLE_FASTCALL GFSerialiseVectorBinaryOut(CDataOutputStream& os, std::vector<T, std::allocator<T> >& vec);

template <class T> void CPersistContext::TransferVector(std::vector<T, std::allocator<T> >& vec)
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

template void CPersistContext::TransferVector<C2DVector>(std::vector<C2DVector, std::allocator<C2DVector> >& vec);
