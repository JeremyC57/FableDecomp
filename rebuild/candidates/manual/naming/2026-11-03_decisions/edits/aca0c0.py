P = "rebuild/src/compiled/00/ac/QuadricOptimiserInternalsCOptimisedMesh_RemoveBones_00aca0c0.cpp"
EDITS = [(P, "const_iterator find(const K&key)const{return const_iterator(_M_find(key));", "template<class _KT> const_iterator find(const _KT&key)const{return const_iterator(_M_find(key));"),
         (P, "private:_Link_type _M_find(const K&)const", "private:template<class _KT> _Link_type _M_find(const _KT&)const")]
