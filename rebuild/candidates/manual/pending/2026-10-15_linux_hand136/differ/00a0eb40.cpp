// Grade: DIFFER(ours v retail) -- 118v116 (same shape as 00a0e680): retail homes the empty __false_type tag temporary in the dead `result` argument slot (`lea edx,[esp+58h]`, no frame), the lane gives it its own `push ecx` frame word (`lea edx,[esp+37h]`, + `pop ecx`); tried a user-constructed tag (best), a plain empty tag (adds a zero store) and optimize("s") (frame pointer)
// UninitCopyDeque_00a0eb40(SDequeIterator first, SDequeIterator last, SDequeIterator result) -- SGI uninitialized_copy over deque iterators: forwards to the __false_type copy loop and returns the advanced destination iterator
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A0EB40, length 116 bytes (+ int3 padding).
// Callees: 0x00A0E790 the SGI `__uninitialized_copy_aux(first, last, result, __false_type)` deque-segment copy
// loop (the element type of this instantiation is not modelled; `ret 30h`).
// Label correction: the catalog calls this CEnginePrimitiveRenderer::AddStaticMesh (propagated across three
// byte-identical instantiations 0x00A0E680 / 0x00A0EB40 / 0x00A0EF90, each forwarding to its own copy loop);
// the body is a free template function of the SGI algorithm TU.
// Shape: the algorithm TU is compiled with /Gr, so the function is `__fastcall`: the 16-byte iterator result
// comes back through the hidden pointer in ecx (kept in esi and returned in eax), the three iterators arrive by
// value on the stack (`ret 30h`). Each iterator has a user copy constructor, so every by-value argument of the
// forwarded call is built in its reserved slot (`sub esp,10h ; mov eax,esp ; mov [eax],..`); the empty
// `__false_type` tag is passed by const reference -- the register argument in edx -- and VC homes the temporary
// in the dead `result` argument slot (`lea edx,[esp+58h]`) once the copies are made.

struct SElement8 { unsigned long m_0, m_4; };            // 8-byte deque element
struct SDequeIterator {                                  // SGI _Deque_iterator: cur, first, last, node
    SElement8*  m_cur;
    SElement8*  m_first;
    SElement8*  m_last;
    SElement8** m_node;
    SDequeIterator(const SDequeIterator& other)
        : m_cur(other.m_cur), m_first(other.m_first), m_last(other.m_last), m_node(other.m_node) { }
};
struct SFalseType { SFalseType() { } };

SDequeIterator __fastcall UninitCopyAux_00a0e790(SDequeIterator first, SDequeIterator last, SDequeIterator result,
                                                const SFalseType& tag);   // 0x00A0E790

SDequeIterator __fastcall UninitCopyDeque_00a0eb40(SDequeIterator first, SDequeIterator last, SDequeIterator result)
{
    return UninitCopyAux_00a0e790(first, last, result, SFalseType());
}
