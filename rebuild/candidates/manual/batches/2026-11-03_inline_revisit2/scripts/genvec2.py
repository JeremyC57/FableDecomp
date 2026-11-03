import sys,re,os,subprocess
P,X=sys.argv[1:]; t0=open(P).read()
chain=('template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }\n'
 'struct __false_type { };\n'
 'template <class _Tp> inline void __destroy_aux(_Tp* __first, _Tp* __last, const __false_type&) { for (; __first != __last; ++__first) _Destroy(__first); }\n'
 'template <class _Tp> inline void __destroy(_Tp* __first, _Tp* __last, _Tp*) { __destroy_aux(__first, __last, __false_type()); }\n'
 'template <class _Tp> inline void _Destroy(_Tp* __first, _Tp* __last) { __destroy(__first, __last, (_Tp*)0); }\n')
old=('template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }\n'
 'template <class _Tp> inline void _Destroy(_Tp* __first, _Tp* __last) { for (; __first != __last; ++__first) _Destroy(__first); }\n')
assert old in t0
emp='    iterator end() { return this->_M_finish; }\n'
mods={'m1':t0.replace(old,chain),
      'm2':t0.replace(old,chain).replace('void deallocate(_Tp* __p, unsigned int) { free(__p); }','void deallocate(_Tp* __p, unsigned int) { if (__p) free(__p); }'),
      'm6':t0.replace(emp,emp+'    bool empty() const { return this->_M_start == this->_M_finish; }\n'),
      'm7':t0.replace(old,chain).replace(emp,emp+'    bool empty() const { return this->_M_start == this->_M_finish; }\n')}
keep={'m1':['a_while','h_dowhile_loop','c_continue'],'m2':['a_while'],'m6':['b_guard','d_guard_while','i_ret_continue_guard'],'m7':['b_guard','d_guard_while']}
tmp=X+'/_tmp'; os.makedirs(tmp,exist_ok=True)
for k,t in mods.items():
    open(f"{tmp}/{k}.cpp",'w').write(t)
    open(f"{X}/{k}_for.cpp",'w').write(t)
    subprocess.run(['python3',os.path.dirname(os.path.abspath(__file__))+'/genvec.py',f"{tmp}/{k}.cpp",f"{tmp}/{k}"])
    for v in keep[k]: os.rename(f"{tmp}/{k}/{v}.cpp",f"{X}/{k}_{v}.cpp")
subprocess.run(['rm','-rf',tmp])
