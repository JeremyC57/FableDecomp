// std::vector<CCharString, std::allocator<CCharString> >::vector(const vector& x) -- STLport 4.6 vector copy constructor for CCharString elements: allocate exactly x.size() slots (none when empty), then copy-construct x's strings into them
// Leaf: ??0?$vector@VCCharString@@V?$allocator@VCCharString@@@std@@@std@@QAE@ABV01@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x007ACEA0, 106 bytes, thiscall, `ret 4`, returns this.
// Identity: STLport 4.6 vector has two template arguments (vector<CCharString, allocator<CCharString> >): the landed
//   _Rb_tree<CCharString, pair<const CCharString, vector<CCharString> > >::_M_insert 0x007AE1D0 and NF85's map::operator[] 0x007AEBE0
//   (probed) bind this spelling; FableWin / Ego_r spell the same (Dinkumware) copy ctor ??0?$vector@VCCharString@@V?$allocator@..@std@@@std@@.
//   The old row's one-argument vector<T> model was a stand-in.
// Callees: CRT malloc 0x00BFEA0E, CCharString::CCharString(const CCharString&) 0x0099EC30 (out of line).
// Shape: STLport 4.6 _vector.h `vector(const _Self& __x) : _Vector_base(__x.size(), __x.get_allocator()) { _M_finish =
//   __uninitialized_copy(__x._M_start, __x._M_finish, _M_start, __false_type()); }` with the placement-new null test per element.
// NF87: moved off the row-local CCharString / allocator / vector models onto stlport/fable_containers.h; explicit instantiation.
#define FABLE_RETAIL_CCHARSTRING
#include "stlport/fable_containers.h"
#include "fable_string.h"
template std::vector<CCharString, std::allocator<CCharString> >::vector(const std::vector<CCharString, std::allocator<CCharString> >&);
