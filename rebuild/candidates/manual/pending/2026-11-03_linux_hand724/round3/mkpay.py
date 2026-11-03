import json,os,sys
rows=[l.rstrip('\n').split('\t') for l in open('rows.tsv') if l.strip()]
out=[];ora=['address\tname\tlength\tbytes']
orac={l.split('\t')[0]:l.rstrip('\n') for l in open(sys.argv[1])}
for addr,module,name,leaf,*fl in rows:
    e={'address':addr,'module':module,'name':name,'source_cpp':open('src/%s.cpp'%addr).read(),
       'test_cpp':open('test/%s.test.cpp'%addr).read(),'pass_pattern':'%s_TEST PASS'%addr,'leaf':leaf}
    if fl and fl[0]: e['flags']=fl[0].split()
    out.append(e); ora.append(orac[addr])
json.dump({'result':{'authored':out}},open('payload.json','w'),indent=1)
open('oracle.tsv','w').write('\n'.join(ora)+'\n')
print(len(out),'rows')
