import sys,re,os
P,X=sys.argv[1:]; t=open(P).read()
emp='    iterator end() { return this->_M_finish; }\n'
m=re.search(r"( *)for \(std::vector<CCharString>::iterator it = names\.begin\(\); it != names\.end\(\); \+\+it\) \{\n *(unsigned long handle = [^\n]*)\n *if \(handle != g_InvalidSoundHandle\) \{\n *return handle;\n *\}\n *\}\n",t)
ind,call=m.group(1),m.group(2); IT='std::vector<CCharString>::iterator'
loop=lambda init,a='    ':f"{a}for ({init}; it != names.end(); ++it) {{\n{a}    {call}\n{a}    if (handle != g_InvalidSoundHandle) {{\n{a}        return handle;\n{a}    }}\n{a}}}\n"
V={'g1_beginend':(None,f"if (names.begin() != names.end()) {{\n"+loop(IT+' it = names.begin()')+"}\n"),
   'g2_itfirst':(None,f"{IT} it = names.begin();\nif (it != names.end()) {{\n"+loop('')+"}\n"),
   'g3_empty_be':('    bool empty() const { return begin() == end(); }\n',f"if (!names.empty()) {{\n"+loop(IT+' it = names.begin()')+"}\n"),
   'g4_size':('    unsigned int size() const { return unsigned int(this->_M_finish - this->_M_start); }\n',f"if (names.size() != 0) {{\n"+loop(IT+' it = names.begin()')+"}\n"),
   'g5_itdecl_empty':('    bool empty() const { return this->_M_start == this->_M_finish; }\n',f"{IT} it;\nif (!names.empty()) {{\n"+loop('it = names.begin()')+"}\n"),
   'g6_size_gt0':('    unsigned int size() const { return unsigned int(this->_M_finish - this->_M_start); }\n',f"if (names.size() > 0) {{\n"+loop(IT+' it = names.begin()')+"}\n"),
   'g7_itfirst_empty':('    bool empty() const { return this->_M_start == this->_M_finish; }\n',f"{IT} it = names.begin();\nif (!names.empty()) {{\n"+loop('')+"}\n"),
}
for k,(mem,v) in V.items():
    tt=t.replace(emp,emp+mem) if mem else t
    if mem and 'begin() == end()' in mem: tt=tt.replace(emp+mem, emp+mem.replace('begin() == end()','const_cast<vector*>(this)->begin() == const_cast<vector*>(this)->end()'))
    mm=re.search(re.escape(m.group(0)),tt)
    body=''.join(ind+l+'\n' if l else '\n' for l in v.rstrip('\n').split('\n'))
    open(f"{X}/{k}.cpp",'w').write(tt[:mm.start()]+body+tt[mm.end():])
