nhss: nhss.c level.c move.c record.c headers
	gcc -o nhss nhss.c level.c move.c record.c -lncurses

headers: nhss.h level.h move.h record.h

nhss.1: nhss.pod
	pod2man -c '' -r '' nhss.pod nhss.1

distclean:
	rm -rf .svn
	rm -rf levels/.svn
	rm nhss
