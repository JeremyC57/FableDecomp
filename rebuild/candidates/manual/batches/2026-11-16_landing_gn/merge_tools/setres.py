import json,os,sys
p=os.path.dirname(os.path.abspath(__file__))+'/res.json'
r=json.load(open(p)) if os.path.exists(p) else {}
v=sys.argv[3]
if v.startswith('py:') and not v.startswith('py:/'): v='py:'+os.path.dirname(p)+'/rs/'+v[3:]
r.setdefault(sys.argv[1],{})[sys.argv[2]]=v; json.dump(r,open(p,'w'),indent=1)
