# count prologue (retail 0x0060AFA1: `xor ebx,ebx` BEFORE `call 0x006166E0` CategoriesList clear, the seven zero stores via ebx after)
CL = '    CategoriesList.clear();\n'
DECL = '''    unsigned long basics = 0;
    unsigned long objects = 0;
    unsigned long towns = 0;
    unsigned long hero = 0;
    unsigned long combat = 0;
    unsigned long quests = 0;
    unsigned long story = 0;
'''
EDITS = {
 # C1: counters declared (and zeroed) before the clear
 'C1': [(CL + DECL, DECL + CL, None)],
 # C2: declared before, zeroed after (assignments)
 'C2': [(CL + DECL, DECL.replace(' = 0;', ';') + CL + ''.join('    %s = 0;\n' % n for n in ['basics','objects','towns','hero','combat','quests','story']), None)],
 # C3: declared before the clear, one per line, chained zero after
 'C3': [(CL + DECL, '    unsigned long basics = 0, objects = 0, towns = 0, hero = 0, combat = 0, quests = 0, story = 0;\n' + CL, None)],
 # C4: clear between first and rest
 'C4': [(CL + DECL, DECL.split('\n',1)[0] + '\n' + CL + DECL.split('\n',1)[1], None)],
 # C5: chained assignment after the clear
 'C5': [(CL + DECL, CL + '    unsigned long basics, objects, towns, hero, combat, quests, story;\n    basics = objects = towns = hero = combat = quests = story = 0;\n', None)],
 # C6: chained assignment before the clear
 'C6': [(CL + DECL, '    unsigned long basics, objects, towns, hero, combat, quests, story;\n    basics = objects = towns = hero = combat = quests = story = 0;\n' + CL, None)],
}
