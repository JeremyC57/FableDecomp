# h22_c690.py: naming wave 22, family c690 -- chunk-690 queue rows 0x0076C000 / 0x0076C1A0: the two std::sort instantiations take the nested
# comparators CParticleUpdateParameters<K,K>::CParticleUpdateCCompareVector<K> / CParticleUpdateCCompareParamDesc<K> with class key V, as the PDB
# declares them (FableWin.pdb; the chunk-690 own rows bind ...V?$CParticleUpdateCCompareVector...); the landed caller
# CParticleUpdateParameters<unsigned long, unsigned long>::Update 0x0076C750 declared them as structs (U). Name-only (empty classes).
import os, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import he
a = '0076c750'
he.edit(a, 'src', '    template <class K> struct CParticleUpdateCCompareVector { };\n    template <class K> struct CParticleUpdateCCompareParamDesc { };',
        '    template <class K> class CParticleUpdateCCompareVector { };      // class key V (FableWin.pdb)\n    template <class K> class CParticleUpdateCCompareParamDesc { };   // class key V (FableWin.pdb)')
