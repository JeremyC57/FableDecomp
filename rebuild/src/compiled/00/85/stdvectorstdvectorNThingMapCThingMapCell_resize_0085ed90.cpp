// std::vector<std::vector<NThingMap::CThingMapCell>>::resize(unsigned int) -- value-initialise an empty inner vector and forward it
// Leaf: ?resize@?$vector@V?$vector@VCThingMapCell@NThingMap@@V?$allocator@VCThingMapCell@NThingMap@@@std@@@std@@V?$allocator@V?$vector@VCThingMapCell@NThingMap@@V?$allocator@VCThingMapCell@NThingMap@@@std@@@std@@@2@@std@@QAEXI@Z
// Identity (proven): Ego_r.pdb egor_pdb_names.tsv 0x0055E410 and FableWin.pdb fablewin_pdb_names.tsv 0x029DA890, both that QAEXI spelling.
// Retail 0x0085ED90 is 36 bytes: load n, xor-zero a 12-byte vector, call resize(n, value) at 0x0085ED20.
// Was: vector<EWeaponClass>::_Umove<EWeaponClass*> (length estimate 96).
namespace NThingMap { class CThingMapCell { public: int x; }; }
namespace std {
template<class T> class allocator {};
template<class T, class Al=allocator<T> > class vector {
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    vector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
    void resize(unsigned int, const vector&);
    void resize(unsigned int n) {
        unsigned long z = 0;
        unsigned int count = n;
        vector value;
        value._M_start = 0;
        value._M_finish = 0;
        value._M_end_of_storage = 0;
        resize(count, value);
    }
};
}
template void std::vector<std::vector<NThingMap::CThingMapCell> >::resize(unsigned int);