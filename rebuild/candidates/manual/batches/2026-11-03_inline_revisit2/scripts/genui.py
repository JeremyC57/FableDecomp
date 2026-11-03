import sys,re
P,X=sys.argv[1:]; import os; os.makedirs(X,exist_ok=True); t=open(P).read()
L=" for(unsigned long i=0;i<count;++i){long key=(long)i-offset;if(element.Object&&Find(Entries->First,Entries->Last,key,Tag())!=Entries->Last){\n"
assert L in t
m=re.search(re.escape(L)+r"(.*?\n)(child->[^\n]*\}\})\n",t,re.S); body=m.group(1); tail=m.group(2)  # tail ends with '}}'
inner=body+tail[:-2]   # statements of the if body, ending with ';'
F="Find(Entries->First,Entries->Last,key,Tag())!=Entries->Last"
def put(new): return t[:m.start()]+new+'\n'+t[m.end():]
V={
'a_guard':" if(count){for(unsigned long i=0;i<count;++i){long key=(long)i-offset;if(element.Object&&"+F+"){\n"+inner+"}}}",
'b_while':" unsigned long i=0;while(i<count){long key=(long)i-offset;if(element.Object&&"+F+"){\n"+inner+"}++i;}",
'c_nested':" for(unsigned long i=0;i<count;++i){long key=(long)i-offset;if(element.Object)if("+F+"){\n"+inner+"}}",
'd_guard_nested':" if(count){for(unsigned long i=0;i<count;++i){long key=(long)i-offset;if(element.Object)if("+F+"){\n"+inner+"}}}",
'f_continue':" for(unsigned long i=0;i<count;++i){long key=(long)i-offset;if(!element.Object||!("+F+"))continue;\n"+inner+"}",
'g_ne':" for(unsigned long i=0;i!=count;++i){long key=(long)i-offset;if(element.Object&&"+F+"){\n"+inner+"}}",
'h_hoist':" if(element.Object)for(unsigned long i=0;i<count;++i){long key=(long)i-offset;if("+F+"){\n"+inner+"}}",
'i_direct':" for(unsigned long i=0;i<count;++i){long key=(long)i-offset;if(element.Object&&"+F+"){\n"+inner.replace("Component* created=Manager::GetInstance()->CreateComponent(element->GetName());\nPointer child(created);","Pointer child(Manager::GetInstance()->CreateComponent(element->GetName()));")+"}}",
'j_guard_continue':" if(count){for(unsigned long i=0;i<count;++i){long key=(long)i-offset;if(!element.Object||!("+F+"))continue;\n"+inner+"}}",
'k_copyinit':" for(unsigned long i=0;i<count;++i){long key=(long)i-offset;if(element.Object&&"+F+"){\n"+inner.replace("Pointer child(created);","Pointer child=Pointer(created);")+"}}",
}
for k,v in V.items(): open(f"{X}/{k}.cpp",'w').write(put(v))
print(len(V))
