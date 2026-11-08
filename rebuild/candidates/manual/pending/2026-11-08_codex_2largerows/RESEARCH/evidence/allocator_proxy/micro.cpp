extern int call(int); int probe(int a,int b){ int c=call(a); return call(b)+c+a; }
