# mkpayload.py <addr:module:name:leaf>... -> payload.json from src/<addr>.cpp + test/<addr>.cpp
import json,sys,os
D=os.path.dirname(os.path.abspath(__file__))
rows=[]
for a in sys.argv[1:]:
    addr,mod,name,leaf=a.split(':',3)
    rows.append({"address":"0x"+addr.upper(),"module":mod,"name":name,"source_cpp":open(f"{D}/src/{addr}.cpp").read(),
                 "test_cpp":open(f"{D}/test/{addr}.cpp").read(),"pass_pattern":f"{addr}_TEST PASS","leaf":leaf})
json.dump({"result":{"authored":rows}},open(f"{D}/payload.json","w"),indent=1)
print(len(rows),"rows")
