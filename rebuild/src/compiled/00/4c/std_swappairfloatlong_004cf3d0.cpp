// std::swap<pair<float,long> >(pair<float,long>&, pair<float,long>&) -- exchange two (distance, key) pairs
// Leaf: swap
// Retail: Fable.exe @ 0x004CF3D0, 37 bytes, __fastcall (a in ecx, b in edx, plain `ret`), unreferenced. Catalog label
// NHitLocations::CPhysicalPrimitiveMesh::GetClosestPointToPos2D is wrong: the body is STLport swap over an 8-byte value
// (`tmp = a; a = b; b = tmp` with tmp in an 8-byte stack slot). 0x004CF3D0 and 0x004CFBC0 are byte-identical copies
// (two instantiations that were not folded), so the second member type is not recoverable from the bytes.
// Layout: the shape pins the element as {float, 4-byte integer} with a user copy constructor (STLport pair's
// `pair(const pair&)`): two floats, two longs, {long, float} or an implicit copy constructor all give other code.
// Shape: /O2; the float first member keeps tmp.first in its stack slot while tmp.second is forwarded through eax.
namespace std {
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
    pair() {}
    pair(const pair& p) : first(p.first), second(p.second) {}
};
template <class T> void __fastcall swap(T& a, T& b)
{
    T tmp = a;
    a = b;
    b = tmp;
}
template void __fastcall swap<pair<float, long> >(pair<float, long>&, pair<float, long>&);
}