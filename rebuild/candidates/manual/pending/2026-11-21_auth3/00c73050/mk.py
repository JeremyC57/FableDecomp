import sys
src=open('b0.cpp').read()
def v(name, *pairs):
    s=src
    for a,b in pairs:
        assert s.count(a)==1, (name,a)
        s=s.replace(a,b)
    open(name+'.cpp','w').write(s)

    if (type == 1 || type == 0x10)
        g_AudioContext.SourcePlay(SourceId);""","""    switch (sound->m_flags & 0x7f) {
    case 1:
    case 0x10:
        g_AudioContext.SourcePlay(SourceId);
    }"""))

    CircularQueue* queue = &sound->m_queue;""","""    CircularQueue* queue = &sound->m_queue;
    ApplySound(sound, now);"""))



        if (QueueNextBuffer(queue, now, looping) != 0)"""))
v('v6',("""    if (type == 1 || type == 0x10)
        g_AudioContext.SourcePlay(SourceId);""","""    if (type == 1)
        g_AudioContext.SourcePlay(SourceId);
    else if (type == 0x10)
        g_AudioContext.SourcePlay(SourceId);"""))
