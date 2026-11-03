import sys,re,os
P,X=sys.argv[1:]; os.makedirs(X,exist_ok=True); t=open(P).read()
m=re.search(r"( *)for \(std::vector<CCharString>::iterator it = names\.begin\(\); it != names\.end\(\); \+\+it\) \{\n *(unsigned long handle = [^\n]*)\n *if \(handle != g_InvalidSoundHandle\) \{\n *return handle;\n *\}\n *\}\n",t)
ind,call=m.group(1),m.group(2)
IT='std::vector<CCharString>::iterator'
V={
'a_while':f"{IT} it = names.begin();\nwhile (it != names.end()) {{\n    {call}\n    if (handle != g_InvalidSoundHandle) {{\n        return handle;\n    }}\n    ++it;\n}}\n",
'b_guard':f"if (!names.empty()) {{\n    for ({IT} it = names.begin(); it != names.end(); ++it) {{\n        {call}\n        if (handle != g_InvalidSoundHandle) {{\n            return handle;\n        }}\n    }}\n}}\n",
'c_continue':f"for ({IT} it = names.begin(); it != names.end(); ++it) {{\n    {call}\n    if (handle == g_InvalidSoundHandle)\n        continue;\n    return handle;\n}}\n",
'd_guard_while':f"if (!names.empty()) {{\n    {IT} it = names.begin();\n    while (it != names.end()) {{\n        {call}\n        if (handle != g_InvalidSoundHandle) {{\n            return handle;\n        }}\n        ++it;\n    }}\n}}\n",
'e_dowhile':f"for ({IT} it = names.begin(); it != names.end(); ++it) {{\n    {call}\n    do {{ if (handle != g_InvalidSoundHandle) return handle; }} while (0);\n}}\n",
'f_size_guard':f"if (names.size() > 0) {{\n    for ({IT} it = names.begin(); it != names.end(); ++it) {{\n        {call}\n        if (handle != g_InvalidSoundHandle) {{\n            return handle;\n        }}\n    }}\n}}\n",
'g_forever':f"for ({IT} it = names.begin(); ; ++it) {{\n    if (it == names.end())\n        break;\n    {call}\n    if (handle != g_InvalidSoundHandle) {{\n        return handle;\n    }}\n}}\n",
'h_dowhile_loop':f"{IT} it = names.begin();\nif (it != names.end()) {{\n    do {{\n        {call}\n        if (handle != g_InvalidSoundHandle) {{\n            return handle;\n        }}\n        ++it;\n    }} while (it != names.end());\n}}\n",
'i_ret_continue_guard':f"if (!names.empty()) {{\n    for ({IT} it = names.begin(); it != names.end(); ++it) {{\n        {call}\n        if (handle == g_InvalidSoundHandle)\n            continue;\n        return handle;\n    }}\n}}\n",
'j_loopvar_outside':f"{IT} it;\nfor (it = names.begin(); it != names.end(); ++it) {{\n    {call}\n    if (handle != g_InvalidSoundHandle) {{\n        return handle;\n    }}\n}}\n",
'k_found_break':f"unsigned long handle = g_InvalidSoundHandle;\nfor ({IT} it = names.begin(); it != names.end(); ++it) {{\n    handle = {call.split('= ',1)[1]}\n    if (handle != g_InvalidSoundHandle)\n        return handle;\n}}\n",
}
for k,v in V.items():
    body=''.join(ind+l+'\n' if l else '\n' for l in v.rstrip('\n').split('\n'))
    open(f"{X}/{k}.cpp",'w').write(t[:m.start()]+body+t[m.end():])
print(len(V))
