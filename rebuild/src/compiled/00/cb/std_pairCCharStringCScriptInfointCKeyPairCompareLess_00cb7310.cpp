// std::__introsort_loop<std::pair<CCharString,CScriptInfo> *,std::pair<CCharString,CScriptInfo>,int,CKeyPairCompareLess<CCharString,CScriptInfo> >(std::pair<CCharString,CScriptInfo> *,std::pair<CCharString,CScriptInfo> *,std::pair<CCharString,CScriptInfo> *,int,CKeyPairCompareLess<CCharString,CScriptInfo>) -- SGI depth-limited partition sort.
// Leaf: ??$__introsort_loop@PAU?$pair@VCCharString@@VCScriptInfo@@@std@@U12@HV?$CKeyPairCompareLess@VCCharString@@VCScriptInfo@@@@@std@@YIXPAU?$pair@VCCharString@@VCScriptInfo@@@0@00HV?$CKeyPairCompareLess@VCCharString@@VCScriptInfo@@@@@Z
// Flags: /Gr
/*
 *
 * Copyright (c) 1994
 * Hewlett-Packard Company
 *
 * Copyright (c) 1996,1997
 * Silicon Graphics Computer Systems, Inc.
 *
 * Copyright (c) 1997
 * Moscow Center for SPARC Technology
 *
 * Copyright (c) 1999 
 * Boris Fomitchev
 *
 * This material is provided "as is", with absolutely no warranty expressed
 * or implied. Any use is at your own risk.
 *
 * Permission to use or copy this software for any purpose is hereby granted 
 * without fee, provided the above notices are retained on all copies.
 * Permission to modify the code and to distribute modified code is granted,
 * provided the above notices are retained, and a notice that the code was
 * modified is included with the above copyright notice.
 *
 */
// Modified excerpt: only the algorithms required by this instantiation are retained.
#include "engine/CScriptInfo.h"
#include "engine/CVectorMap.h"
#pragma inline_recursion(on)
namespace std {
template<class _Tp,class _Compare>
const _Tp& __median(const _Tp&,const _Tp&,const _Tp&,_Compare);
template<class _RandomAccessIter,class _Tp,class _Compare>
_RandomAccessIter __unguarded_partition(_RandomAccessIter,_RandomAccessIter,_Tp,_Compare);
template<class _RandomAccessIter,class _Tp,class _Compare>
void __partial_sort(_RandomAccessIter,_RandomAccessIter,_RandomAccessIter,_Tp*,_Compare);
template<class _RandomAccessIter,class _Compare>
void partial_sort(_RandomAccessIter __first,_RandomAccessIter __middle,_RandomAccessIter __last,_Compare __comp) {
    __partial_sort(__first,__middle,__last,(_RandomAccessIter)0,__comp);
}
template<class _RandomAccessIter,class _Tp,class _Size,class _Compare>
void __introsort_loop(_RandomAccessIter __first,_RandomAccessIter __last,_Tp*,_Size __depth_limit,_Compare __comp) {
    while (__last-__first > 16) {
        if (__depth_limit == 0) {
            partial_sort(__first,__last,__last,__comp);
            return;
        }
        --__depth_limit;
        _RandomAccessIter __cut = __unguarded_partition(__first,__last,
            _Tp(__median(*__first,*(__first+(__last-__first)/2),*(__last-1),__comp)),__comp);
        __introsort_loop(__cut,__last,(_Tp*)0,__depth_limit,__comp);
        __last=__cut;
    }
}
}
template void std::__introsort_loop<std::pair<CCharString,CScriptInfo>*,std::pair<CCharString,CScriptInfo>,int,CKeyPairCompareLess<CCharString,CScriptInfo> >(std::pair<CCharString,CScriptInfo>*,std::pair<CCharString,CScriptInfo>*,std::pair<CCharString,CScriptInfo>*,int,CKeyPairCompareLess<CCharString,CScriptInfo>);