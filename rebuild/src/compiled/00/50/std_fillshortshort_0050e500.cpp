// std::fill<short*,short>(short* first, short* last, const short& value) -- STLport fill: store value into every slot of [first, last)
// Leaf: ??$fill@PAFF@std@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x0050E500, 33 bytes, `ret 4` (__fastcall: first in ecx, last in edx, the value
//   reference on the stack -- the free algorithm templates of this TU are /Gr).
// Callers: 0x00514A7F, 0x00514AD3 (a vector<short> fill path in the CWorldMap module).
// Shape: STLport 4.x `fill(first, last, const T& val) { for (; first != last; ++first) *first = val; }`. The value is
//   re-read through its reference every pass (`mov si,[eax]`): a short store through `first` may alias `val`. The
//   16-byte aligned loop head (`lea esp,[esp]`) marks a speed (/O2) TU.
namespace std {

template <class ForwardIter, class T>
void __fastcall fill(ForwardIter first, ForwardIter last, const T& val)
{
    for (; first != last; ++first)
        *first = val;
}

template void __fastcall fill<short*, short>(short* first, short* last, const short& val);

}