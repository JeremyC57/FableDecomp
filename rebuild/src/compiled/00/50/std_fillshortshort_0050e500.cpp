// std::fill<unsigned short*,unsigned short>(unsigned short* first, unsigned short* last, const unsigned short& value) -- STLport fill: store value into every slot of [first, last)
// Leaf: ??$fill@PAGG@std@@YIXPAG0ABG@Z
// Name fix (NF383, 2026-11-24): was fill<short*,short> (??$fill@PAFF@std@@YIXPAF0ABF@Z). Retail has no ICF, so the instance is the one its only caller
//   names: 0x005149F0 vector<unsigned short>::_M_fill_insert (landed self ?_M_fill_insert@?$vector@GV?$allocator@G@std@@@std@@QAEXPAGIABG@Z; its callers
//   are CWideString::Sprintf and NSpeechGainManager::Initialise, whose FableWin body calls vector<unsigned short>::resize), and STLport's
//   _M_fill_insert calls fill(pos, old_finish, x) with its own element type. Ego_r.pdb world_map.obj holds fill<unsigned short*,unsigned short> at
//   33 bytes (= retail; FableWin world_map.obj too), and the table already binds that spelling here (binder 005149f0).
// Retail: Fable.exe (TLC, VC7.1) @ 0x0050E500, 33 bytes, `ret 4` (__fastcall: first in ecx, last in edx, the value
//   reference on the stack -- the free algorithm templates of this TU are /Gr).
// Callers: 0x00514A7F, 0x00514AD3 (vector<unsigned short>::_M_fill_insert 0x005149F0).
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

template void __fastcall fill<unsigned short*, unsigned short>(unsigned short* first, unsigned short* last, const unsigned short& val);

}