// _global::_Push_heap(CThing**, long, long, CThing*, bool (__fastcall*)(CThing*, CThing*)) -- heap sift-up of thing pointers under a function-pointer predicate
// Leaf: _Push_heap
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00932370, 92 bytes, `ret 0xc`.
// Label correction: the catalog "_Push_heap<float*,int,float,std::less<float>>" is propagated.  The
// sort is CAIStateGroup_MinionReactToCriminal::Update (0x00931390): `sort(first, last, 0x00930D90)`
// where 0x00930D90 is a __fastcall `bool (CThing* a, CThing* b)` comparing the two things'
// GetPosition() (0x004C73D0) heights (`fld [a+8] ; fcomp [b+8] ; test ah,5 ; jp`).
// Link-time __fastcall (/Gr algorithm TU): first in ecx, hole in edx, then top, the pointer value
// and the function pointer (4 + 4 + 4 = 0xc).  The predicate takes both pointers BY VALUE in
// registers (`mov ecx,[parent] ; mov edx,value ; call [esp+1Ch]`).
// Shape: SGI/STLport `__push_heap`; the drop is tail-duplicated into both exits (as 0x005140A0).
class CThing;

typedef bool (__fastcall* TThingCompare)(CThing* a, CThing* b);

void __fastcall _Push_heap(CThing** first, long hole, long top, CThing* value, TThingCompare comp)
{
    long parent = (hole - 1) / 2;
    while (hole > top && comp(first[parent], value)) {
        first[hole] = first[parent];
        hole = parent;
        parent = (hole - 1) / 2;
    }
    first[hole] = value;
}